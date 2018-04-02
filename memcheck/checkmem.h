#ifndef CHECKMEM_H
#define CHECKMEM_H

#include <stddef.h>

/*
* 对malloc进行的封装, 添加了边界检测内存块
* (inline 原本分_DEBUG有宏处理, 后面没加等于没用)
* sz        : 申请内存长度
*            : 返回得到的内存首地址
*/
extern void* mc_malloc(size_t sz);

/*
 * 对calloc进行封装, 添加边界检测内存块
 * cut        : 申请的个数
 * sz        : 每个的大小
 */
extern inline void* mc_calloc(size_t cut, size_t sz);

/*
* 对relloc进行了封装, 同样添加了边间检测内存块
*/
extern void* mc_realloc(void* ptr, size_t sz);

/*
* 对内存检测, 看是否出错, 出错直接打印错误信息
* 只能检测, check_* 得到的内存
*/
extern inline void mc_check(void* ptr);


#endif // CHECKMEM_H
