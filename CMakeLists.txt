project(wxAnsiConv)
cmake_minimum_required(VERSION 2.8)

set(CMAKE_CXX_FLAGS "-Wall -Wextra")

# find_package(wxWidgets 3.0 COMPONENTS core base REQUIRED)
find_package(wxWidgets COMPONENTS core base REQUIRED)
include("${wxWidgets_USE_FILE}")

aux_source_directory(./src ${PROJECT_NAME}_SOURCES)

add_executable(${PROJECT_NAME} ${${PROJECT_NAME}_SOURCES})

target_link_libraries(
    ${PROJECT_NAME}
    ${wxWidgets_LIBRARIES}
    )
