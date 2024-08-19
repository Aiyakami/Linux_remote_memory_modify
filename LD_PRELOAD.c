/*
编译为动态连接库并使用LD_PRELOAD加载能实现类似于windows上的hook效果
gcc -fPIC -shared -o mymalloc.so mymalloc.c -ldl
gcc -o test_program test_program.c LD_PRELOAD=./mymalloc.so ./test_program
*/
#include <stdio.h>
#include <stdlib.h>
#include <dlfcn.h>

//实现hook malloc，每次调用时打印分配内存大小信息
void* malloc(size_t size) {
    static void* (*real_malloc)(size_t) = NULL;
    if (!real_malloc) {
        // 获取真正的 malloc 函数地址
        real_malloc = (void* (*)(size_t))dlsym(RTLD_NEXT, "malloc");
    }
    // 打印分配的大小
    printf("malloc(%zu) called\n", size);
    // 调用真正的 malloc
    return real_malloc(size);
}
