file(GLOB_RECURSE Sources "./*.cpp")

if(MSVC)
    add_compile_options(/bigobj)
endif(MSVC)

add_executable(L_Variable_Tests ${Sources})

target_include_directories(L_Variable_Tests PUBLIC
                            "${PROJECT_SOURCE_DIR}"
                            "${PROJECT_SOURCE_DIR}/include"
                            "${PROJECT_SOURCE_DIR}/L_Utility/include"
                            "${PROJECT_SOURCE_DIR}/L_Variable/include"
)


target_link_libraries(L_Variable_Tests PUBLIC
    L_Utility
    L_Variable
)

if (UNIX)
    target_link_libraries(L_Variable_Tests PUBLIC
        pthread
    )
endif (UNIX)
