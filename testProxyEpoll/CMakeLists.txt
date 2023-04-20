cmake_minimum_required(VERSION 3.1.0)

project(testProxyEpoll)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON)

set (SOURCE_FILES common.h handler.h logger.h server.h parser.h handler.cpp logger.cpp server.cpp parser.cpp handler.cpp main.cpp )

add_executable(testProxyEpoll ${SOURCE_FILES})

