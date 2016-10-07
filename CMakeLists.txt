cmake_minimum_required(VERSION 2.8)
project(Redsql)

# output dir
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/bin)
set(CMAKE_PDB_OUTPUT_DIRECTORY     ${CMAKE_CURRENT_BINARY_DIR}/bin)

# src file path
set(SRC_PATH
        src
)

# include search path
set(INC_PATH
        /usr/local/include
        include
        src
)


#lib search path
set(LIB_PATH
        /usr/local/lib
)

#lib objects
set(LIB_OBJ
        odbc
        hiredis
        )

#src files
set(SOURCE_FILES
        ${SRC_PATH}/odbcsql.cpp
        ${SRC_PATH}/test_main.cpp
        ${SRC_PATH}/redissql.cpp include/redissql.h
)



include_directories( ${INC_PATH} )

add_executable(Redsql ${SOURCE_FILES})

#dependcy
target_link_libraries(Redsql ${LIB_OBJ})
