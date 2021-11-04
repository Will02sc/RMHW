cmake_minimum_required(VERSION 3.15)
project(reprojection)


set(CMAKE_CXX_STANDARD 17)

find_package( OpenCV REQUIRED )
find_package(Eigen3)
include_directories(${OpenCV_INCLUDE_DIRS})

add_executable(reprojection main.cpp)
target_link_libraries(  reprojection ${OpenCV_LIBS}  )
target_link_libraries( reprojection MVSDK )
target_link_libraries( reprojection Eigen3::Eigen)