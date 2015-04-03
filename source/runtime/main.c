#include "gc.h"

int main(int argc, char** argv) {
    extern int ATC_Main(int argc, char** argv);
    uintptr_t ret_code;
    gc_init(&ret_code);
    ret_code = ATC_Main(argc, argv);
    gc_shutdown();
    return (int)ret_code;
}

