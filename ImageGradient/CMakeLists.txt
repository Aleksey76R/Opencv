cmake_minimum_required(VERSION 3.10.0)
project(ImageGradient VERSION 0.1.0)

include(CTest)
enable_testing()

find_package(OpenCV REQUIRED)
include_directories(${OpenCV_INCLUDE_DIRS})

add_executable(ImageGradient 
               main.cpp
               imageGradient.h
               imageGradient.cpp)

target_link_libraries(ImageGradient ${OpenCV_LIBS})


set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)