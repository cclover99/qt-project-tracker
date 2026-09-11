cmake_minimum_required(VERSION 3.21)

project(ProjectTracker LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt6 REQUIRED COMPONENTS Widgets)

qt_standard_project_setup()

qt_add_executable(ProjectTracker
    src/main.cpp
    src/mainwindow.cpp
    src/mainwindow.hpp
)

target_link_libraries(ProjectTracker PRIVATE Qt6::Widgets)

# Launch without console
# set_target_properties(ProjectTracker PROPERTIES WIN32_EXECUTABLE ON)