cmake_minimum_required(VERSION 3.5)

project(L_Variable LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

FILE(GLOB_RECURSE LibFiles "include/*.h")
add_custom_target(L_Variable_include SOURCES ${LibFiles})

add_library(L_Variable STATIC
	source/MDL_Reader.cpp
	source/Object_Constructor.cpp
	source/Type_Manager.cpp
	source/Variable_Base.cpp
	source/Builder_Stub.cpp
)

target_include_directories(L_Variable PUBLIC 
                            "${PROJECT_BINARY_DIR}" 
                            "${PROJECT_SOURCE_DIR}/include"
                            "${PROJECT_SOURCE_DIR}/../L_Utility/include"
                            )
