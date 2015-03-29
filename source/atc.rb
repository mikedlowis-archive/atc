#!/usr/bin/ruby

require 'stringio'

#------------------------------------------------------------------------------
class AlgebraicType
    def initialize(name, vars=[], &block)
        @name = name
        @constructors = {}
        @typevars = vars
        instance_eval(&block)
    end

    def print
        values = @constructors.values
        if values.compact.length == 0
            generate_enum()
        elsif ((values.length == 1) && (values.compact.length == 1))
            generate_struct()
        else
            generate_union()
        end
    end

    def method_missing(name, *args, &block)
        if args.empty? or args == [nil]
            @constructors[name] = nil
        else
            @constructors[name] = args
        end
    end

    private

    def generate_enum()
        Header.puts "typedef enum {\n    " +
                    @constructors.keys.map{|v| v.to_s }.join(",\n    ") +
                    "\n} #{@name};\n\n"
    end

    def generate_struct()
        constr = @constructors.keys.first
        fields = @constructors.values.first
        # Declare the type
        Header.puts "typedef struct {"
        fields.each_with_index do |v,idx|
            Header.puts "    #{v} v#{idx};"
        end
        Header.puts "} #{@name};\n\n"
        # Define the constructor
        args = fields.each_with_index.map{|v,i| "#{v} a#{i}"}
        Header.puts "#{@name}* #{constr}(#{args.join(', ')});\n\n"
        Source.puts "#{@name}* #{constr}(#{args.join(', ')}) {"
        Source.puts "    #{@name}* value = (#{@name}*)malloc(sizeof(#{@name}));"
        fields.each_with_index do |v,i|
            Source.puts "    value->v#{i} = a#{i};"
        end
        Source.puts "    return value;"
        Source.puts "}\n\n"
    end

    def generate_union()
        Header.puts "typedef union #{@name} {"
        # Generate the union tags
        Header.puts "    enum #{@name}Tag {"
        Header.puts @constructors.map{|k,v| "        t#{k}"}.join(",\n")
        Header.puts "    } tag;"
        # Generate the union variants
        @constructors.each_pair do |name,fields|
            if not fields.nil?
                Header.puts "    struct {"
                Header.puts "        enum #{@name}Tag tag;"
                fields.each_with_index do |v,idx|
                    if v == @name
                        Header.puts "        union #{v}* v#{idx};"
                    else
                        Header.puts "        #{v} v#{idx};"
                    end
                end
                Header.puts "    } #{name};"
            end
        end
        Header.puts "} #{@name};\n\n"
        @constructors.each_pair do |name,fields|
            if not fields.nil?
                args = fields.each_with_index.map{|v,i| "#{v} a#{i}"}
                Header.puts "#{@name}* #{name}(#{args.join(', ')});\n\n"
                Source.puts "#{@name}* #{name}(#{args.join(', ')}) {"
                Source.puts "    #{@name}* value;"
                Source.puts "    value = (#{@name}*)malloc(sizeof(value->#{name}));"
                Source.puts "    value->tag = t#{name};"
                fields.each_with_index do |v,i|
                    Source.puts "    value->#{name}.v#{i} = a#{i};"
                end
                Source.puts "    return value;"
                Source.puts "}\n\n"
            else
                Header.puts "extern #{@name}* #{name};\n\n"
                Source.puts "#{@name}* #{name} = &(union #{@name}){ .tag = t#{name} };"
                #Source.puts "    #{@name}* value;"
                #Source.puts "    value = (#{@name}*)malloc(sizeof(value->tag));"
                #Source.puts "    value->tag = t#{name};"
                #Source.puts "    return value;"
                #Source.puts "}\n\n"
            end
        end
    end
end

def type(name, vars=[], &block)
    AlgebraicType.new(name, vars, &block).print
end

#------------------------------------------------------------------------------
Typedefs = {
    :Int  => "intptr_t",
    :Uint => "uintptr_t"
}

# If we dont have an input file then exit
if ARGV.length != 1
    puts "Usage: atc.rb INFILE"
    exit 1
end

# Open the source files for writing
Header = File.open(ARGV[0].gsub(/\.[^.]+$/,'') + '.h', "w")
Source = File.open(ARGV[0].gsub(/\.[^.]+$/,'') + '.c' , "w")
Header.puts "#ifndef #{ARGV[0].gsub('.','_').upcase}"
Header.puts "#define #{ARGV[0].gsub('.','_').upcase}\n\n"
Source.puts "#include \"#{ARGV[0].gsub(/\.[^.]+$/,'') + '.h'}\"\n\n"
Source.puts "#include <stdlib.h>\n\n"
at_exit do
    Header.puts "#endif"
    Header.close
    Source.close
end

# Load the input file
load ARGV[0]

