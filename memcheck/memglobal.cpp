#include "memglobal.h"
#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <stdlib.h>


//这个主要为了解决 引用了 头文件 memglobal.h 会造成递归调用. Linux上还有一种思路, 不包含这个头文件
//链接时候gcc 指定就可以.  但是 vs 是自动推导编译, 如果不引入它推导不出来. 后面就采用了上面通用的做法.

// 取消内置宏, 防止递归
#undef malloc
#undef calloc
#undef realloc
#undef free

// 控制台打印错误信息, fmt必须是双引号括起来的宏
#define IOERR(io, fmt, ...) \
    fprintf(io,"[%s:%s:%d][error %d:%s]" fmt "\r\n",\
         __FILE__, __func__, __LINE__, errno, strerror(errno), ##__VA_ARGS__)

// 全局内存计数, 系统第一次构造的时候为0
static int _mct;

#define _STR_MGTXT    "checkmem.log"

// mg内存监测退出时候, 记录一些信息
static void _mg_exit(void) {
    if (_mct == 0) return;

    // 先打印信息到控制台
    IOERR(stderr, "Detect memory leaks _mct = %d!!", _mct);

    //输出到文件
    FILE* txt = fopen(_STR_MGTXT, "a");
    if (txt == NULL) {
        IOERR(stderr, "fopen " _STR_MGTXT " a is error!");
        return;
    }
    IOERR(txt, "Detect memory leaks _mct = %d!!", _mct);
    fclose(txt);
}

/*
* 全局启动内存简单监测
*/
void mg_start(void) {
    // 注册退出监测事件
    atexit(_mg_exit);
}

/*
* 增加的全局计数的 malloc
* sz        : 待分配内存大小
*            : 返回分配的内存首地址
*/
void* mg_malloc(size_t sz) {
    void* ptr = malloc(sz);
    if (!ptr) return NULL;
    ++_mct;
    memset(ptr, 0x00, sz);
    return ptr;
}

/*
* 增加了全局计数的 calloc
* sc        : 分配的个数
* sz        : 每个分配大小
*            : 返回分配内存的首地址
*/
void* mg_calloc(size_t sc, size_t sz) {
    return mg_malloc(sc*sz);
}

/*
* 增加了计数的 realloc
* ptr        : 上一次分配的内存地址
* sz        : 待重新分配的内存大小
*            : 返回重新分配好的内存地址
*/
void* mg_realloc(void* ptr, size_t sz) {
    if (!ptr) return mg_malloc(sz);
    return realloc(ptr, sz);
}

/*
* 增加了计数处理的内存 free
* ptr        : 上面函数返回地址的指针
*/
void mg_free(void* ptr) {
    if (!ptr) return;
    --_mct;
    free(ptr);
}
