cmake_minimum_required(VERSION 3.5.1)
project(dfw)
set(CMAKE_BUILD_TYPE Release)
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_compile_options(-Wall -Wextra -Wundef -Wcast-align -Wwrite-strings -Wlogical-op -Wmissing-declarations -Wredundant-decls -Wshadow -Woverloaded-virtual -Wno-deprecated -ansi -pedantic -fno-rtti)

option(BUILD_SHARED "Build a shared library" OFF)

set(LOG_DIR "${PROJECT_SOURCE_DIR}/../log/include/" CACHE STRING "Directory where the log repository is (-DLOG_DIR=xxx)")
set(DANSDL2_DIR "${PROJECT_SOURCE_DIR}/../libdansdl2/include/" CACHE STRING "Directory where the libdansdl2 is (-DDANSDL2_DIR")
set(DANSDL2_TOOLS_DIR "${PROJECT_SOURCE_DIR}/../libdansdl2-tools/include/" CACHE STRING "Directory where the libdansdl2-tools is (-DDANSDL2_TOOLS_DIR)")
set(TOOLS_DIR "${PROJECT_SOURCE_DIR}/../tools/include/" CACHE STRING "Directory where the tools lib is (-DTOOLS_DIR)")

include_directories("include" ${LOG_DIR} ${DANSDL2_DIR} ${DANSDL2_TOOLS_DIR} ${TOOLS_DIR})

set(SOURCE "")
add_subdirectory("${PROJECT_SOURCE_DIR}/lib")

if(${BUILD_SHARED})
	add_library(dfw SHARED ${SOURCE})
else()
	add_library(dfw STATIC ${SOURCE})
endif()

install(TARGETS dfw DESTINATION install/lib)
install(DIRECTORY include/ DESTINATION install/include)
