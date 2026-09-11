cmake_minimum_required(VERSION 3.21)

project(ProjectTracker LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Place binaries inside build/bin/
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin")

find_package(Qt6 REQUIRED COMPONENTS Widgets Sql)

qt_standard_project_setup()

qt_add_executable(ProjectTracker
    src/main.cpp
    src/mainwindow.cpp
    src/mainwindow.hpp
    src/database.cpp
    src/database.hpp
    src/widgets/projectTitle.cpp
    src/widgets/projectTitle.hpp
)

target_link_libraries(ProjectTracker PRIVATE 
    Qt6::Widgets
    Qt6::Sql
)

# Launch without console
# set_target_properties(ProjectTracker PROPERTIES WIN32_EXECUTABLE ON)