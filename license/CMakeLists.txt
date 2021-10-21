cmake_minimum_required(VERSION 3.15)
project(license)


set(CMAKE_CXX_STANDARD 17)

find_package( OpenCV REQUIRED )

include_directories(${OpenCV_INCLUDE_DIRS})

add_executable(license main.cpp)
target_link_libraries(  license ${OpenCV_LIBS}  )
target_link_libraries( license MVSDK )