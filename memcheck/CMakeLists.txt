# first we add the executable that generates the table
# add the binary tree directory to the search path for include files
include_directories("${CMAKE_CURRENT_BINARY_DIR}")
#注意：加入${PROJECT_BINARY_DIR}是为了mysqrt.cxx能够引入TutorialConfig.h
include_directories("${PROJECT_BINARY_DIR}")

IF (CMAKE_BUILD_TYPE STREQUAL Debug)        #用IF控制，可以实现Release版本不输出debug信息
    ADD_DEFINITIONS(-D_DEBUG)
ENDIF()

#ADD_DEFINITIONS("-DLIBHELLO_BUILD")  
#修改库的编译输出位置,由于是动态库，所以需要将dll放到Project目录中
SET(LIBRARY_OUTPUT_PATH ${PROJECT_BINARY_DIR}/${CMAKE_BUILD_TYPE})  

SET(LIB_SRC 
    #	mysqrt.cxx ${CMAKE_CURRENT_BINARY_DIR}/Table.h
    memglobal.h memglobal.cpp
)  

#设置字符集  
add_definitions(-DUNICODE -D_UNICODE) 

#搜索目录下的文件名并存入变量（可能会放入多余文件名）
#aux_source_directory(. LIB_SRC)

# 生成动态库目标
add_library(memcheck SHARED ${LIB_SRC})
# 生成静态库目标
add_library(memcheck_static STATIC ${LIB_SRC})

#修改库的输出名称
#set_target_properties(memcheck PROPERTIES OUTPUT_NAME "libmathfunctions")

# 指定静态库的输出名称
#set_target_properties(person_static PROPERTIES OUTPUT_NAME "person")
# 使动态库和静态库同时存在
#set_target_properties(person PROPERTIES CLEAN_DIRECT_OUTPUT 1)
#set_target_properties(person_static PROPERTIES CLEAN_DIRECT_OUTPUT 1)

# 指定动态库版本
# VERSION 动态库版本
# SOVERSION API版本
#set_target_properties(person PROPERTIES VERSION 1.0 SOVERSION 1)

#要安装的头文件
set(head_files
        memglobal.h
)
#安装到CMAKE_INSTALL_PREFIX指定的位置
install (TARGETS memcheck DESTINATION bin)
install (FILES memglobal.h DESTINATION include)

#安装到指定位置
install(FILES ${head_files} DESTINATION  ${CMAKE_BINARY_DIR}/include)
install(TARGETS memcheck
        RUNTIME DESTINATION ${CMAKE_BINARY_DIR}/bin
        LIBRARY DESTINATION ${CMAKE_BINARY_DIR}/lib
        ARCHIVE DESTINATION ${CMAKE_BINARY_DIR}/lib/static)

#安装到指定位置
#install(TARGETS MathFunctions
#        RUNTIME DESTINATION ${PROJECT_BINARY_DIR})
