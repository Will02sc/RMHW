cmake_minimum_required(VERSION 3.15)
project(hw2)


set(CMAKE_CXX_STANDARD 17)

find_package( OpenCV REQUIRED )

include_directories(${OpenCV_INCLUDE_DIRS})

add_executable(hw2 main.cpp)
target_link_libraries(  hw2 ${OpenCV_LIBS}  )
target_link_libraries( hw2 MVSDK )