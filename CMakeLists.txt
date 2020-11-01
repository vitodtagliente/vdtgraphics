cmake_minimum_required(VERSION 3.2)
if(TARGET vdtgraphics)
	return()
endif()
project(vdtgraphics)

set(CMAKE_CXX_STANDARD 17)

# recursively include all source files
file(GLOB_RECURSE PROJECT_SOURCES "src/*.h" "src/*.cpp") 
# Remove OpenGL on non supported platforms
if(NOT (WIN32 OR UNIX OR APPLE))
	list(FILTER PROJECT_SOURCES EXCLUDE REGEX ".*/opengl.*")
endif()

# Remove DirectX apis on non Windows platforms
if(NOT WIN32)
	list(FILTER PROJECT_SOURCES EXCLUDE REGEX ".*/directx.*")
endif()

foreach(_source IN ITEMS ${PROJECT_SOURCES})
    get_filename_component(_source_path "${_source}" PATH)
    file(RELATIVE_PATH _source_path_rel "${PROJECT_SOURCE_DIR}" "${_source_path}")
    string(REPLACE "/" "\\" _group_path "${_source_path_rel}")
    source_group("${_group_path}" FILES "${_source}")
endforeach()

add_library(
    ${PROJECT_NAME} 
	STATIC
    ${PROJECT_SOURCES}
)

target_include_directories(${PROJECT_NAME} PUBLIC 
	src/
)

if(NOT TARGET glfw)
    add_subdirectory(vendor/vdtmath)
endif()

target_link_libraries(${PROJECT_NAME} PUBLIC vdtmath)