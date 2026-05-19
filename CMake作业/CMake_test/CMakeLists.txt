cmake_minimum_required(VERSION 3.10)
project(Test)
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
add_executable(server main.cpp)
add_executable(client main.cpp)
find_package(OpenCV REQUIRED)
add_subdirectory(common)
add_subdirectory(modules)

target_link_libraries(
    server PUBLIC M1 M2 math ${OpenCV_LIBS})
target_include_directories(
    server PUBLIC ${Common_INCLUDE_DIRS} ${Modules_INCLUDE_DIRS} ${OpenCV_INCLUDE_DIRS}
)

target_link_libraries(
    client PUBLIC M1 M2 math ${OpenCV_LIBS})
target_include_directories(
    client PUBLIC ${Common_INCLUDE_DIRS} ${Modules_INCLUDE_DIRS} ${OpenCV_INCLUDE_DIRS}
)