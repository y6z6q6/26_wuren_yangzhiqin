cmake_minimum_required(VERSION 3.10)
project(test)
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
add_executable(test main.cpp)
find_package(OpenCV REQUIRED)
add_subdirectory(common)
add_subdirectory(modules)
target_link_libraries(
    test PUBLIC M1 M2 math ${OpenCV_LIBS})
target_include_directories(
    test PUBLIC ${Common_INCLUDE_DIRS} ${Modules_INCLUDE_DIRS} ${OpenCV_INCLUDE_DIRS}
)