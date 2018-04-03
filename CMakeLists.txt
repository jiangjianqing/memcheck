cmake_minimum_required (VERSION 2.6)
set (TARGET MemCheck-Demo)
project (${TARGET})

# The version number.
set (Tutorial_VERSION_MAJOR 1)
set (Tutorial_VERSION_MINOR 0)

#------------------------------option section--------------------------------
# should we use our own math functions?
# option必须在configure_file前设置
option (USE_MYMATH "Use tutorial provided math implementation" ON) 
#option(USE_32BITS "Compile for 32bits processors(linux only)" OFF)
#option(BUILD_USE_64BITS "Compile for 64bits processors" ON)

#------------------------------check section---------------------------------

option(CMAKE_ENCODING_UTF8 "Use UTF-8 encoding internally (experimental)." ON)
mark_as_advanced(CMAKE_ENCODING_UTF8)
if(CMAKE_ENCODING_UTF8)
  set(KWSYS_ENCODING_DEFAULT_CODEPAGE CP_UTF8)
endif()

if(WIN32 AND KWSYS_ENCODING_DEFAULT_CODEPAGE MATCHES CP_UTF8)
  add_definitions(-DKWSYS_CP_UTF8)
  message(STATUS "注意: vc 编译器UTF-8")
endif()


#---------------------------重要设置区--------------------------------------

#设置字符集
add_definitions(-DUNICODE -D_UNICODE)
message(STATUS "设置字符集为 UNICODE . ")

include(CheckCXXCompilerFlag)
CHECK_CXX_COMPILER_FLAG("-std=c++14" COMPILER_SUPPORTS_CXX14)
CHECK_CXX_COMPILER_FLAG("-std=c++0x" COMPILER_SUPPORTS_CXX0X)
if(COMPILER_SUPPORTS_CXX14)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14")
    message(STATUS "The compiler ${CMAKE_CXX_COMPILER}  support C++14 . ")
elseif(COMPILER_SUPPORTS_CXX0X)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++0x")
else()
    #如果是vc编译器，可能不准确
    IF (NOT MSVC)
        message(STATUS "The compiler ${CMAKE_CXX_COMPILER} has no C++14 support. Please use a different C++ compiler.")
    ELSE()
        #add_compile_options命令添加的编译选项是针对所有编译器的(包括c和c++编译器)，而set命令设置CMAKE_C_FLAGS或CMAKE_CXX_FLAGS变量则是分别只针对c和c++编译器的。
        #举一反三，我们就可以想到，add_definitions这个命令也是同样针对所有编译器
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14")
        message(STATUS " 注意: QT Creator 对vc 编译器检查 c++特性的结果可能不正确")
    ENDIF ()
endif()

IF (MSVC)
    message(STATUS "极其重要的发现：VC默认情况下需要每个源码文件设置为UTF-8 with signature，vs2015update2之后可以加入-source-charset:utf-8标志修改")
    add_compile_options(-source-charset:utf-8)
    #set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -source-charset:utf-8")
endif()

IF (CMAKE_BUILD_TYPE STREQUAL Debug)        #用IF控制，可以实现Release版本不输出debug信息
    ADD_DEFINITIONS(-D_DEBUG -DDEBUG)
ENDIF()


# does this system provide the log and exp functions?
#include (CheckFunctionExists)
#引用module
include (${CMAKE_ROOT}/Modules/CheckFunctionExists.cmake)
check_function_exists (log HAVE_LOG)
check_function_exists (exp HAVE_EXP)

#option和check 两个section都是负责生成一些变量
#---------------------------------------------------------------

# configure a header file to pass some of the CMake settings
# to the source code
configure_file (
	"${PROJECT_SOURCE_DIR}/TutorialConfig.h.in"
	"${PROJECT_BINARY_DIR}/TutorialConfig.h"
)

# add the MathFunctions library?
#
if (USE_MYMATH)
  include_directories ("${PROJECT_SOURCE_DIR}/memcheck")
  add_subdirectory (memcheck)
  set (EXTRA_LIBS ${EXTRA_LIBS} memcheck)
endif (USE_MYMATH)
 
# add the binary tree to the search path for include files
# so that we will find TutorialConfig.h
include_directories("${PROJECT_BINARY_DIR}")
 
# add the executable

add_executable (${TARGET} main.cpp)

target_link_libraries (${TARGET} ${EXTRA_LIBS})

#-----------------------------------Install section-----------------------------------
# The CMake variable CMAKE_INSTALL_PREFIX is used to determine the root of where the files will be installed. 
#重要：windows下CMAKE_INSTALL_PREFIX默认指向C:\Program Files (x86)，需要用管理员账户启动visual studio
#此处改为本地目录，可以规避上述问题
set (CMAKE_INSTALL_PREFIX "${PROJECT_BINARY_DIR}/install")

# add the install targets
install (TARGETS ${TARGET} DESTINATION bin)
install (FILES "${PROJECT_BINARY_DIR}/TutorialConfig.h"        
         DESTINATION include)

#----------------------------------Test section---------------------------------------------
# enable testing
enable_testing ()

# does the application run
#add_test (TutorialRuns Tutorial 25)

# does the usage message work?
#add_test (TutorialUsage Tutorial)
#set_tests_properties (TutorialUsage PROPERTIES PASS_REGULAR_EXPRESSION "Usage:.*number")

#define a macro to simplify adding tests, then use it
macro (do_test arg result)
  add_test (TutorialComp${arg} ${TARGET} ${arg})
  set_tests_properties (TutorialComp${arg}
    PROPERTIES PASS_REGULAR_EXPRESSION ${result})
endmacro (do_test)

# do a bunch of result based tests
do_test (25 "25 is 5")
do_test (-25 "-25 is 0")
do_test (0.0001 "0.0001 is 0.01")

#----------------------------------Pack section---------------------------------------------
#windows下使用nsis完成打包(nsis需要手动安装)
# build a CPack driven installer package
include (InstallRequiredSystemLibraries)
#set (CPACK_RESOURCE_FILE_LICENSE "${CMAKE_CURRENT_SOURCE_DIR}/License.txt")
set (CPACK_PACKAGE_VERSION_MAJOR "${Tutorial_VERSION_MAJOR}")
set (CPACK_PACKAGE_VERSION_MINOR "${Tutorial_VERSION_MINOR}")
include (CPack)



#注:使用下一行#include(CTest)语句会加入太多vs项目
# enable dashboard scripting
#include(CTest)
