cmake_minimum_required(VERSION 3.15)
project(myjson)
set(CMAKE_CXX_STANDARD 17)

INCLUDE_DIRECTORIES(include)

#file(GLOB SRC_LIST "*.cpp" "protocol/*.cpp")
#自定义搜索规矩
AUX_SOURCE_DIRECTORY(src DIR_SRCS)
#aux_source_directory(dir VAR) 发现一个目录下所有的源代码文件并将列表存储在一个变量中。

add_library(json ${DIR_SRCS})
ADD_EXECUTABLE(myjson main.cpp)
target_link_libraries(json mjson)
