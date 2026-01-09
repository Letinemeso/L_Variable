cmake_minimum_required(VERSION 3.5)

project(L_Variable LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

FILE(GLOB_RECURSE L_Variable_include "include/*.h")
add_custom_target(_L_Variable_include SOURCES ${L_Variable_include})

file(GLOB_RECURSE Sources "source/*.cpp")
add_library(L_Variable STATIC ${Sources} )

target_include_directories(L_Variable PUBLIC 
                            "${PROJECT_BINARY_DIR}" 
                            "${PROJECT_SOURCE_DIR}/include"
                            "${PROJECT_SOURCE_DIR}/../L_Utility/include"
                            "${PROJECT_SOURCE_DIR}/../L_Utility/glm"
                            )
