cmake_minimum_required(VERSION 3.15)
project(calib)


set(CMAKE_CXX_STANDARD 17)

find_package( OpenCV REQUIRED )

include_directories(${OpenCV_INCLUDE_DIRS})

add_executable(calib main.cpp)
target_link_libraries(  calib ${OpenCV_LIBS}  )
target_link_libraries( calib MVSDK )