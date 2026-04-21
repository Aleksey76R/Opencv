cmake_minimum_required(VERSION 3.10.0)
project(Image_Processor VERSION 0.1.0)

include(CTest)
enable_testing()

find_package(OpenCV REQUIRED)

include_directories(${OpenCV_INCLUDE_DIRS})

add_executable(Image_Processor main.cpp imageProcessor.cpp)

target_link_libraries(Image_Processor ${OpenCV_LIBS})

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)