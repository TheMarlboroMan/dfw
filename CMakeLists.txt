cmake_minimum_required(VERSION 3.5.1)
project(dfw)

option(BUILD_DEBUG "Build a debug release" OFF)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
if(${BUILD_DEBUG})
	set(CMAKE_BUILD_TYPE Debug)

else()
	set(CMAKE_BUILD_TYPE Release)
endif()

add_compile_options(-Wall -Wextra -Wundef -Wcast-align -Wwrite-strings -Wlogical-op -Wmissing-declarations -Wredundant-decls -Wshadow -Woverloaded-virtual -Wno-deprecated -ansi -pedantic -fno-rtti)

#Add the include directories
include_directories("include")

set(SOURCE "")
add_subdirectory("${PROJECT_SOURCE_DIR}/lib")

add_library(dfw_static STATIC ${SOURCE})
set_target_properties(dfw_static PROPERTIES OUTPUT_NAME dfw)

add_library(dfw_shared SHARED ${SOURCE})
set_target_properties(dfw_shared PROPERTIES OUTPUT_NAME dfw)

IF(WIN32)
	target_compile_definitions(dfw_static PUBLIC -DWINBUILD)
	target_compile_definitions(dfw_shared PUBLIC -DWINBUILD)
	
	include_directories("${CMAKE_CURRENT_LIST_DIR}/../log/include/")
	include_directories("${CMAKE_CURRENT_LIST_DIR}/../libdansdl2/include/")
	include_directories("${CMAKE_CURRENT_LIST_DIR}/../libdansdl2-tools/include/")
	include_directories("${CMAKE_CURRENT_LIST_DIR}/../tools/include/")
	include_directories("${CMAKE_CURRENT_LIST_DIR}/../rapidjson/include/")

	add_library(lm SHARED IMPORTED)
	set_target_properties(lm PROPERTIES IMPORTED_LOCATION "C:/Program Files (x86)/lm/lib/liblm.dll")
	set_target_properties(lm PROPERTIES IMPORTED_IMPLIB "C:/Program Files (x86)/lm/lib/liblm.dll")
	
	add_library(dansdl2 SHARED IMPORTED)
	set_target_properties(dansdl2 PROPERTIES IMPORTED_LOCATION "C:/Program Files (x86)/libdansdl2/lib/libdansdl2.dll")
	set_target_properties(dansdl2 PROPERTIES IMPORTED_IMPLIB "C:/Program Files (x86)/libdansdl2/lib/libdansdl2.dll")
	
	add_library(ldtools SHARED IMPORTED)
	set_target_properties(ldtools PROPERTIES IMPORTED_LOCATION "C:/Program Files (x86)/libldtools/lib/libldtools.dll")
	set_target_properties(ldtools PROPERTIES IMPORTED_IMPLIB "C:/Program Files (x86)/libldtools/lib/libldtools.dll")
	
	add_library(tools SHARED IMPORTED)
	set_target_properties(tools PROPERTIES IMPORTED_LOCATION "C:/Program Files (x86)/tools/lib/libtools.dll")
	set_target_properties(tools PROPERTIES IMPORTED_IMPLIB "C:/Program Files (x86)/tools/lib/libtools.dll")
	
	target_link_libraries(dfw_static lm dansdl2 tools ldtools SDL2 SDL2_ttf SDL2_mixer SDL2_image freeglut glew32 glu32 opengl32 stdc++fs)
	target_link_libraries(dfw_shared lm dansdl2 tools ldtools SDL2 SDL2_ttf SDL2_mixer SDL2_image freeglut glew32 glu32 opengl32 stdc++fs)
endif()

install(TARGETS dfw_static DESTINATION lib)
install(TARGETS dfw_shared DESTINATION lib)
install(DIRECTORY include/ DESTINATION include)
