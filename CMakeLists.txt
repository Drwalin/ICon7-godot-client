cmake_minimum_required(VERSION 3.19)
project(ICon7-godot-client)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_subdirectory(godot-cpp)
add_subdirectory(ICon7)

include_directories(ICon7/bitscpp/include)
include_directories(ICon7/include)
include_directories(godot-cpp/include)
include_directories(include)

# Library compilation

aux_source_directory(./include/icon7-godot-client/ ICon7_godot_include)
aux_source_directory(./src/ ICon7_godot_src)
add_library(icon7_godot_client SHARED
	${ICon7_godot_include}
	${ICon7_godot_src}
)
target_link_libraries(icon7_godot_client PUBLIC icon7)
target_link_libraries(icon7_godot_client PUBLIC godot::cpp)
