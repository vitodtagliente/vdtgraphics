cmake_minimum_required(VERSION 3.2)
if(TARGET vdtgraphics)
	return()
endif()
project(vdtgraphics)

set(CMAKE_CXX_STANDARD 17)

file(GLOB PROJECT_HEADERS "include/vdtgraphics/*.h") 
file(GLOB PROJECT_SOURCES "source/*.cpp")
file(GLOB VENDORS_SOURCES "vendor/glad/src/glad.c")

source_group("Headers" FILES ${PROJECT_HEADERS})
source_group("Sources" FILES ${PROJECT_SOURCES})
source_group("Vendors" FILES ${VENDORS_SOURCES})

add_library(
    ${PROJECT_NAME} 
	STATIC
    ${PROJECT_HEADERS}
    ${PROJECT_SOURCES}
	${VENDORS_SOURCES}
)

target_include_directories(${PROJECT_NAME} PUBLIC 
	include
    vendor/glad/include/
)

if(NOT TARGET vdtmath)
    add_subdirectory(vendor/vdtmath)
endif()
if(NOT TARGET freetype)
    add_subdirectory(vendor/freetype EXCLUDE_FROM_ALL)
endif()

target_link_libraries(
    ${PROJECT_NAME} 
    PUBLIC vdtmath
    PRIVATE freetype
)