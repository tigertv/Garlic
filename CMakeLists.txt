cmake_minimum_required(VERSION 3.16.0)

include(CMakeDependentOption)

project(Garlic)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CSharp_FLAGS "/langversion:8.0")

#default settings
set(BUILD_EXAMPLES OFF CACHE BOOL "Build examples")
set(BUILD_PROGRAMS OFF CACHE BOOL "Build programs")
set(BUILD_SHARED_LIBS OFF CACHE BOOL "Build shared libraries")
set(BUILD_STATIC_LIBS ON CACHE BOOL "Build static library")
set(BUILD_TESTING OFF CACHE BOOL "Build the testing tree")

#Garlic settings
option(GAR_BUILD_TUNIC "Build the Tunic framework" ON)
CMAKE_DEPENDENT_OPTION(GAR_BUILD_BULB "Build the Bulb editor (requries Tunic)" OFF "GAR_BUILD_TUNIC" OFF)
option(GAR_BUILD_EXAMPLES OFF)
option(GAR_ENABLE_CLANG_TIDY "Enable static analysis with clang-tidy" OFF)

#Clove settings
option(GAR_CLV_ENABLE_PROFILING "Enables the Clove profiler which will print function timings to a file" OFF)
option(GAR_CLV_ENABLE_MEMORY_DEBUGGING "Enables certain memory debugging tools such as extra logs" OFF)
CMAKE_DEPENDENT_OPTION(GAR_TRACK_MEMORY_ALLOCATIONS "Enables custom new / delete operators to track allocations" OFF "GAR_CLV_ENABLE_MEMORY_DEBUGGING" OFF)
option(GAR_CLV_IMGUI_COMPATIBLE "Links Glad publicly so users can initialise Dear ImGui properly" OFF)

set_property(GLOBAL PROPERTY USE_FOLDERS ON)

#Clove
add_subdirectory(Clove)

#Tunic
if(GAR_BUILD_TUNIC)
	add_subdirectory(Tunic)
endif()

#Builb
if(MSVC AND GAR_BUILD_BULB) #not currently supported without MSVC
	add_subdirectory(Bulb)
endif()

if(ENABLE_CLANG_TIDY)
	find_program(CLANGTIDY clang-tidy)
	if(CLANGTIDY)
		set(CMAKE_CXX_CLANG_TIDY ${CLANGTIDY})
	else()
    	message(SEND_ERROR "clang-tidy requested but executable not found")
	endif()
endif()