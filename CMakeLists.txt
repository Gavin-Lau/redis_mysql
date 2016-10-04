cmake_minimum_required(VERSION 2.8)

project (redMysql)

# system check
if(UNIX)
    message("current platform is unix like!")
    add_definitions(-D_LINUX)
    set(OS_DIR "Linux")
else(WIN32)
    message("current platform is windows!")
    set(OS_DIR "Windows")
else()
    message(FATAL_ERROR "current platform is unknown!")
endif()



# output dir
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/bin)
set(CMAKE_PDB_OUTPUT_DIRECTORY     ${CMAKE_CURRENT_BINARY_DIR}/bin)

set(Src_DIR src)
set(Test_DIR test)
    
#include dir
include_directories(
        include
		src
        )
#libs
set(PROJECT_LIBS,
	odbc
	)		
		
# src files
set(PROJECT_SRC
        ${Src_DIR}/main.cpp
    )


#enable debug info
add_definitions (-g)

add_executable(redMysql ${PROJECT_SRC})


#dependcy
target_link_libraries(redMysql PUBLIC debug ${PROJECT_LIBS})








