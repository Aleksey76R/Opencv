cmake_minimum_required(VERSION 3.10.0)
project(Transformation_Photo_Editor VERSION 0.1.0)

include(CTest)
enable_testing()

find_package(OpenCV REQUIRED)

include_directories(${OpenCV_INCLUDE_DIRS})

add_executable(ShapePainter
    main.cpp 
    Shape.h
    Shape.cpp
    Line.h
    Line.cpp
    RectangleShape.h
    RectangleShape.cpp
    Circle.h
    circle.cpp
)

target_link_libraries(ShapePainter ${OpenCV_LIBS})

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)