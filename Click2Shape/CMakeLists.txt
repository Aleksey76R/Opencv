cmake_minimum_required(VERSION 3.10.0)
project(Click2Shape VERSION 0.1.0)

include(CTest)
enable_testing()

find_package(OpenCV REQUIRED)

include_directories(${OpenCV_INCLUDE_DIRS})

add_executable(Click2Shape
    main.cpp 
    Shape.h
    Shape.cpp
    Circle.h
    Circle.cpp
    Rectangle.h
    Rectangle.cpp
    AppState.h
    AppState.cpp
)

target_link_libraries(Click2Shape ${OpenCV_LIBS})

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)