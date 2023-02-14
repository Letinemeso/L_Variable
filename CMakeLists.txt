cmake_minimum_required(VERSION 3.10)

project(L_Variable)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED True)

add_library(L_Variable STATIC
    source/Type_Manager.cpp
    source/MDL_Reader.cpp
    source/Variable_Base.cpp
)

target_include_directories(L_Variable PUBLIC 
                            "${PROJECT_BINARY_DIR}" 
                            "${PROJECT_SOURCE_DIR}/include"
                            "${PROJECT_SOURCE_DIR}/../Utility/include"
                            )
