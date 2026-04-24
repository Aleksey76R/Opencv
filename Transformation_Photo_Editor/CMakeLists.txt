cmake_minimum_required(VERSION 3.10.0)
project(Transformation_Photo_Editor VERSION 0.1.0)

include(CTest)
enable_testing()

find_package(OpenCV REQUIRED)

include_directories(${OpenCV_INCLUDE_DIRS})

add_executable(Transformation_Photo_Editor main.cpp ImageTransformer.cpp ImageTransformer.h)

target_link_libraries(Transformation_Photo_Editor ${OpenCV_LIBS})

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
