cmake_minimum_required(VERSION 3.2)
if(TARGET vdtgraphics)
	return()
endif()
project(vdtgraphics)

set(CMAKE_CXX_STANDARD 17)

if(ASAN_ENABLED)
	string(REGEX REPLACE "/RTC(su|[1su])" "" CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}")
	message(STATUS "CMAKE_CXX_FLAGS: ${CMAKE_CXX_FLAGS}\n")
	add_compile_options(-fsanitize=address -fsanitize=fuzzer)
endif()

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

if(MSVC)
	target_compile_options(${PROJECT_NAME} PRIVATE "/MP")
endif()

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