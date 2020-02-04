cmake_minimum_required(VERSION 3.1.0)

project(ShmServer)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_CXX_FLAGS "-fPIC")
set(THREADS_PREFER_PTHREAD_FLAG ON)


find_package(Threads REQUIRED)
find_package(Qt5 COMPONENTS Xml REQUIRED)

set (SOURCE_FILES concrete_client_receiver.h macro_defines.h messagestructures.h server_transport.h shmcircularbuffer.h concrete_client_receiver.cpp server_transport.cpp shmcircularbuffer.cpp main.cpp )


add_executable(ShmServer ${SOURCE_FILES})


# specify which libraries to connect
target_link_libraries(${PROJECT_NAME} Qt5::Xml)
target_link_libraries(${PROJECT_NAME} rt)
target_link_libraries(${PROJECT_NAME} Threads::Threads)
