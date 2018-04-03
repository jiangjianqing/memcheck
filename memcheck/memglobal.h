#ifndef MEMGLOBAL_H
#define MEMGLOBAL_H

#include <stddef.h>
#include <stdlib.h>

#ifdef WIN32
//当在windows下编译时，会自动生成${PROJECT_NAME}_EXPORT的定义
    #ifdef memcheck_EXPORT
    #define DLLAPI __declspec(dllexport)
    #else
    #define DLLAPI __declspec(dllimport)
    #endif
#else
    #define DLLAPI extern "C"
#endif

//思路: 先启动 全局内存监测功能, 再通过特殊宏,替代原先的申请和释放函数. 来达到目的.

/*
 * 全局启动内存简单监测
 */
DLLAPI void mg_start(void);

DLLAPI void mg_check(void* ptr);

/*
 * 增加的全局计数的 malloc
 * sz        : 待分配内存大小
 *            : 返回分配的内存首地址
 */
DLLAPI void* mg_malloc(size_t sz);

/*
 * 增加了全局计数的 calloc
 * sc        : 分配的个数
 * sz        : 每个分配大小
 *            : 返回分配内存的首地址
 */
DLLAPI void* mg_calloc(size_t sc, size_t sz);

/*
 * 增加了计数的 realloc
 * ptr        : 上一次分配的内存地址
 * sz        : 待重新分配的内存大小
 *            : 返回重新分配好的内存地址
 */
DLLAPI void* mg_realloc(void* ptr, size_t sz);

/*
 * 增加了计数处理的内存 free
 * ptr        : 上面函数返回地址的指针
 */
DLLAPI void mg_free(void* ptr);

// 在测试模式下开启 全局内存使用计数
#if defined(_DEBUG)
#    define malloc        mg_malloc
#    define calloc        mg_calloc
#    define realloc        mg_realloc
#    define free            mg_free
#else
#    define malloc        malloc
#    define calloc        calloc
#    define realloc        realloc
#    define free            free
#endif


#endif // MEMGLOBAL_H
