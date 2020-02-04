cmake_minimum_required(VERSION 3.1.0)

project(ShmClient)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_CXX_FLAGS "-fPIC")
set(THREADS_PREFER_PTHREAD_FLAG ON)


find_package(Threads REQUIRED)
find_package(Qt5 COMPONENTS Xml REQUIRED)

set (SOURCE_FILES client_transport.h concrete_client_sender.h config.h macro_defines.h messagestructures.h shmcircularbuffer.h client_transport.cpp concrete_client_sender.cpp config.cpp main.cpp shmcircularbuffer.cpp)

add_executable(ShmClient ${SOURCE_FILES})


# specify which libraries to connect
target_link_libraries(${PROJECT_NAME} Qt5::Xml)
target_link_libraries(${PROJECT_NAME} rt)
target_link_libraries(${PROJECT_NAME} Threads::Threads)
