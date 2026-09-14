cmake_minimum_required(VERSION 3.21)

project(ProjectTracker LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Place binaries inside build/bin/
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin")

find_package(Qt6 REQUIRED COMPONENTS Widgets Sql)

qt_standard_project_setup()

set(SOURCE_FILES
    src/main.cpp
    src/mainwindow.cpp
    src/mainwindow.hpp
    src/database.cpp
    src/database.hpp

    src/widgets/projecttitle.cpp
    src/widgets/projecttitle.hpp

    src/widgets/projectdescription.cpp
    src/widgets/projectdescription.hpp
)

set(LINK_LIBRARIES
    Qt6::Widgets
    Qt6::Sql
)

# GUI executable
qt_add_executable(ProjectTracker ${SOURCE_FILES})
target_link_libraries(ProjectTracker PRIVATE ${LINK_LIBRARIES})
set_target_properties(ProjectTracker PROPERTIES WIN32_EXECUTABLE ON)

# Console executable
qt_add_executable(ProjectTrackerConsole ${SOURCE_FILES})
target_link_libraries(ProjectTrackerConsole PRIVATE ${LINK_LIBRARIES})