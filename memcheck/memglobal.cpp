#include "memglobal.h"

//这个主要为了解决 引用了 头文件 memglobal.h 会造成递归调用. Linux上还有一种思路, 不包含这个头文件
//链接时候gcc 指定就可以.  但是 vs 是自动推导编译, 如果不引入它推导不出来. 后面就采用了上面通用的做法.

// 取消内置宏, 防止递归
#undef malloc
#undef calloc
#undef realloc
#undef free

#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <stdlib.h>


#include "DebugOutput.h"

#ifdef WIN32
//模拟linux下的getpagesize,获取每个页面大小

//通用调试信息输出库
size_t getpagesize(void)
{
    static size_t sz = 0;
    if(sz > 0){//提升执行效率
        return sz;
    }
    SYSTEM_INFO sysInfo;
    ::ZeroMemory( &sysInfo, sizeof( SYSTEM_INFO ) );
    ::GetSystemInfo( &sysInfo );
    sz = sysInfo.dwPageSize;
    return sz;
}
#else
#include <unistd.h>
#endif



// 插入字节块的个数
#define _INT_CHECK        (1<<4)

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
 *对malloc进行的封装, 添加了边界检测内存块,增加的全局计数的 malloc
* sz        : 申请内存长度
*            : 返回得到的内存首地址
*/
void* mg_malloc(size_t sz) {   
    // 头和尾都加内存检测块, 默认0x00
    int head_size = _INT_CHECK;
    char* ptr = (char*)calloc(1, sz + 2 * _INT_CHECK);
    if (NULL == ptr) {
        CERR_EXIT("malloc sz + sizeof struct check is error!");
    }
    ++_mct;
    //前四个字节保存 最后一个内存块地址 大小
    size_t* iptr = (size_t*)ptr;
    *iptr = sz + _INT_CHECK;

#ifdef WIN32
    //Memory Protection Constants
    //https://msdn.microsoft.com/en-us/library/windows/desktop/aa366786(v=vs.85).aspx
    DWORD oldFlag;
    void* dataPtr = (void*)ptr;
    void* tailPtr = ptr + _INT_CHECK + sz;
    //修改页都保护属性
    DWORD dwOldProtect;
    MEMORY_BASIC_INFORMATION mbi;
    VirtualQuery(tailPtr, &mbi, sizeof(mbi));
    int i = getpagesize();
    //VirtualProtectEx(GetCurrentProcess(), lpAddr, sizeof(DWORD), PAGE_READWRITE, &dwOldProtect)
    if (VirtualProtect(tailPtr,_INT_CHECK,PAGE_READONLY,&oldFlag)){
        int i = 0;
        i =i + 1;
    }
#endif

    void* ptr_ret = ptr + _INT_CHECK;
    return ptr_ret;
}

/*
* 对calloc进行封装, 添加边界检测内存块,增加了全局计数的 calloc
* sc        : 分配的个数
* sz        : 每个分配大小
*            : 返回分配内存的首地址
*/
void* mg_calloc(size_t sc, size_t sz) {
    return mg_malloc(sc*sz);
}

// 检测内存是否错误, 错误返回 true, 在控制台打印信息
static void _iserror(char* s, char* e) {
    while (s < e) {
        char a = *s;
        char b = *e;
        if (*s) {
            CERR_EXIT("Need to debug test!!! ptr is : (%p, %p).check is %d!",s, e, *s);
        }
        ++s;
    }
}

/*
* 对内存检测, 看是否出错, 出错直接打印错误信息
* 只能检测, check_* 得到的内存
*/
void mg_check(void* ptr) {
    char *sptr = (char*)ptr - _INT_CHECK;

    //先检测头部
    char* s = sptr + sizeof(size_t);
    char* e = sptr + _INT_CHECK;
    _iserror(s, e);

    //后检测尾部
    size_t sz = *(size_t*)sptr;
    s = sptr + sz;
    e = s + _INT_CHECK;
    _iserror(s, e);
}


/*
* 增加了计数的 realloc
* ptr        : 上一次分配的内存地址
* sz        : 待重新分配的内存大小
*            : 返回重新分配好的内存地址
*/
void* mg_realloc(void* ptr, size_t sz) {
    if (!ptr)
        return mg_malloc(sz);

    // 先检测一下内存
    mg_check(ptr);

    // 重新申请内存
    char* cptr = (char*)ptr - _INT_CHECK;
    void* nptr = (char*)mg_malloc(sz) - _INT_CHECK;
    if (NULL == nptr) {
        CERR_EXIT("realloc is error:%p.", ptr);
    }
    // 旧数据复制
    size_t* bsz = (size_t*)cptr;//第一个字节为数据长度
    memcpy(nptr, ptr, *bsz < sz ? *bsz : sz);

    free(cptr);
    return nptr;
}

/*
* 增加了计数处理的内存 free
* ptr        : 上面函数返回地址的指针
*/
void mg_free(void* ptr) {
    if (!ptr) return;
    --_mct;
    free((char*)ptr - _INT_CHECK);
}
