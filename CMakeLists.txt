cmake_minimum_required(VERSION 3.5)
project(Point_in_polygon_App
        VERSION 0.0.1
        DESCRIPTION "Point in Polygon"
        LANGUAGES CXX C)

add_executable($ENV{PROJECT_NAME}
               src/PointInPolygon.cpp
               src/Graphic/Renderer.cpp
               src/Graphic/simage.cpp
               src/Window/Window.cpp
               src/Objects/Object.cpp
               src/Objects/Point.cpp
               src/Objects/Rectangle.cpp
               src/Objects/Image.cpp
               src/Objects/MouseArea.cpp
               src/Objects/Button.cpp
               src/Objects/Polygon.cpp
               src/Objects/Text.cpp
               src/Config/Settings.cpp
               External/glfw/3.3.8/src/glad_gl.c
)

target_compile_features($ENV{PROJECT_NAME} PRIVATE cxx_std_20)
find_package(OpenGL REQUIRED)

target_include_directories($ENV{PROJECT_NAME} PRIVATE 
        ${CMAKE_CURRENT_SOURCE_DIR}/src
        ${CMAKE_CURRENT_SOURCE_DIR}/External/glfw/3.3.8/include
        ${CMAKE_CURRENT_SOURCE_DIR}/External/glm/0.9.8.0/include
        ${CMAKE_CURRENT_SOURCE_DIR}/External/image
        ${CMAKE_CURRENT_SOURCE_DIR}/External/freetype/include
)

target_link_directories($ENV{PROJECT_NAME} PRIVATE 
        /f/Windows/_env/msys64/mingw64/bin/
        External/freetype/x64/
        ${CMAKE_CURRENT_SOURCE_DIR}/External/glfw/3.3.8/x64
        ${CMAKE_CURRENT_SOURCE_DIR}/External/freetype/x64/
)

target_link_libraries($ENV{PROJECT_NAME} LINK_PUBLIC opengl32 glfw3 freetype)

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)