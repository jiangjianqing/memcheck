#ifndef DEBUGOUTPUT_H
#define DEBUGOUTPUT_H

#ifdef WIN32
//为了能够在Windows下的QT输出窗口显示调试信息，使用OutputDebugStringA
#include <Windows.h>
#endif

// 控制台打印错误信息, fmt必须是双引号括起来的宏
#ifdef WIN32
#define IOERR(io,fmt, ...) \
{\
    if(io == stderr) {\
    char info[MAX_PATH];\
    sprintf(info,"[%s:%s:%d][error %d:%s]" fmt "\r\n",\
    __FILE__, __func__, __LINE__, errno, strerror(errno), ##__VA_ARGS__);\
    OutputDebugStringA(info);    \
    }\
}
#else
#define IOERR(io,fmt, ...) \
    fprintf(io,"[%s:%s:%d][error %d:%s]"  fmt  "\r\n",\
     __FILE__, __func__, __LINE__, errno, strerror(errno), ##__VA_ARGS__);\

#endif

#ifdef WIN32
#define CERR(fmt, ...) \
{\
    char info[MAX_PATH];\
    sprintf(info,"[%s:%s:%d][error %d:%s]" fmt "\r\n",\
    __FILE__, __func__, __LINE__, errno, strerror(errno), ##__VA_ARGS__);\
    OutputDebugStringA(info);    \
}
#else
// 控制台打印错误信息, fmt必须是双引号括起来的宏
#define CERR(fmt, ...) \
    fprintf(stderr,"[%s:%s:%d][error %d:%s]"  fmt  "\r\n",\
         __FILE__, __func__, __LINE__, errno, strerror(errno), ##__VA_ARGS__)
#endif



//控制台打印错误信息并退出, t同样fmt必须是 ""括起来的字符串常量
#define CERR_EXIT(fmt,...) \
    CERR(fmt,##__VA_ARGS__);        \
    exit(EXIT_FAILURE);     \


#endif // DEBUGOUTPUT_H
