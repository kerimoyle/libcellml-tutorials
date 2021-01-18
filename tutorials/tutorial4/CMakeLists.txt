cmake_minimum_required(VERSION 3.10.2)
set(PROJECT_NAME tutorial4)

project(${PROJECT_NAME} VERSION 0.1.0)

find_package(libCellML REQUIRED)

#   1.b, c Note that you will need to adjust the file name here to match the one that you generated
#          in Tutorial 3.  You will also need to change its extension to be *.cpp instead of *.c in
#          order for CMake to accept it.
set (PROJECT_SRC
        ${PROJECT_NAME}.cpp
        utilities.cpp
        PredatorPrey.cpp  # This is the file generated in Tutorial 3
     )

add_executable(${PROJECT_NAME} ${PROJECT_SRC})
target_link_libraries(${PROJECT_NAME} PUBLIC cellml)

#   1.b, c Note that you will need to adjust the file name here to match the one that you generated
#          in Tutorial 3.  You will also need to change its extension to be *.cpp instead of *.c in
#          order for CMake to accept it.
set(PROJECT_SRC 
        ${PROJECT_NAME}_complete.cpp
        utilities.cpp
        PredatorPrey.cpp  # This is the file generated in Tutorial 3
        )

add_executable(${PROJECT_NAME}_complete ${PROJECT_SRC})
target_link_libraries(${PROJECT_NAME}_complete PUBLIC cellml)
