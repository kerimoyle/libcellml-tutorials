cmake_minimum_required(VERSION 3.10.2)
set(PROJECT_NAME tutorial1)

project(${PROJECT_NAME} VERSION 0.1.0)

find_package(libCellML REQUIRED)

set(PROJECT_SRC ${PROJECT_NAME}.cpp)

add_executable(${PROJECT_NAME} ${PROJECT_SRC})
target_link_libraries(${PROJECT_NAME} PUBLIC cellml)

set(PROJECT_SRC ${PROJECT_NAME}_complete.cpp)

add_executable(${PROJECT_NAME}_complete ${PROJECT_SRC})
target_link_libraries(${PROJECT_NAME}_complete PUBLIC cellml)
