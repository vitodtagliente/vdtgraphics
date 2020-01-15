cmake_minimum_required(VERSION 3.2)
project(vdtgraphics)

enable_testing()

set(CMAKE_CXX_STANDARD 17)

include_directories(
	include 
	vendor/vdtmath/include
)

file(GLOB_RECURSE PROJECT_HEADERS "include/vdtgraphics/*.h") 
file(GLOB_RECURSE PROJECT_SOURCES "source/*.cpp")

# Remove OpenGL on non supported platforms
if(NOT (WIN32 OR UNIX OR APPLE))
	list(FILTER PROJECT_HEADERS EXCLUDE REGEX ".*/opengl.*")
	list(FILTER PROJECT_SOURCES EXCLUDE REGEX ".*/opengl.*")
endif()

# Remove DirectX apis on non Windows platforms
if(NOT WIN32)
	list(FILTER PROJECT_HEADERS EXCLUDE REGEX ".*/directx.*")
	list(FILTER PROJECT_SOURCES EXCLUDE REGEX ".*/directx.*")
endif()

foreach(_source IN ITEMS ${PROJECT_HEADERS})
    get_filename_component(_source_path "${_source}" PATH)
    file(RELATIVE_PATH _source_path_rel "${PROJECT_SOURCE_DIR}" "${_source_path}")
    string(REPLACE "/" "\\" _group_path "${_source_path_rel}")
    source_group("${_group_path}" FILES "${_source}")
endforeach()

foreach(_source IN ITEMS ${PROJECT_SOURCES})
    get_filename_component(_source_path "${_source}" PATH)
    file(RELATIVE_PATH _source_path_rel "${PROJECT_SOURCE_DIR}" "${_source_path}")
    string(REPLACE "/" "\\" _group_path "${_source_path_rel}")
    source_group("${_group_path}" FILES "${_source}")
endforeach()

add_library(
    ${PROJECT_NAME} 
	STATIC
    ${PROJECT_HEADERS}
    ${PROJECT_SOURCES} 
	vdtmath
)

# Dependencies
add_subdirectory(vendor/vdtmath)