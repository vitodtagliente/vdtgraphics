cmake_minimum_required(VERSION 3.2)
get_filename_component(ProjectId ${CMAKE_CURRENT_SOURCE_DIR} NAME)
string(REPLACE " " "_" ProjectId ${ProjectId})
project(${ProjectId})

set(CMAKE_CXX_STANDARD 17)

include_directories(
    . 
	../../vendor/imgui
)

file(GLOB PROJECT_HEADERS "*.h") 
file(GLOB PROJECT_SOURCES "*.cpp")
file(GLOB VENDORS_SOURCES 
	../../vendor/imgui/examples/imgui_impl_glfw.cpp
	../../vendor/imgui/examples/imgui_impl_opengl3.cpp
	../../vendor/imgui/imgui.cpp
	../../vendor/imgui/imgui_draw.cpp
	../../vendor/imgui/imgui_tables.cpp
	../../vendor/imgui/imgui_widgets.cpp
) 

source_group("Headers" FILES ${PROJECT_HEADERS})
source_group("Sources" FILES ${PROJECT_SOURCES})
source_group("Vendors" FILES ${VENDORS_SOURCES})

add_executable(
    ${PROJECT_NAME} 
    ${PROJECT_HEADERS}
    ${PROJECT_SOURCES} 
    ${VENDORS_SOURCES}
)

if(MSVC)
	target_compile_options(${PROJECT_NAME} PRIVATE "/MP")
endif()

target_link_libraries(
    ${PROJECT_NAME} 
)

add_subdirectory(../../ vdtgraphics)
add_subdirectory(../../vendor/glfw glfw)

target_link_libraries(${PROJECT_NAME} PUBLIC glfw)
target_link_libraries(${PROJECT_NAME} PUBLIC vdtgraphics)
