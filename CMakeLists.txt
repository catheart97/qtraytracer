cmake_minimum_required(VERSION 3.23)

project(qtraytracer VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt6 REQUIRED COMPONENTS Core Widgets Gui)
qt_standard_project_setup()

include_directories(
    include
    third_party/eigen
)

file(GLOB SOURCES src/*.cpp)
file(GLOB HEADERS include/*.h)

add_executable(qtraytracer
    ${HEADERS}
    ${SOURCES}
)

target_link_libraries(qtraytracer PRIVATE Qt6::Core Qt6::Widgets Qt6::Gui)