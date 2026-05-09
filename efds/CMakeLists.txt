project(efds_project)
cmake_minimum_required(VERSION 2.8)
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# 1. Define the path (relative to your efds folder)
# If 'efds' and 'training' are both inside 'project/', use ../
set(TRAINING_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/../training/src)

INCLUDE_DIRECTORIES(

    ${CMAKE_CURRENT_SOURCE_DIR}/src
    ${CMAKE_CURRENT_SOURCE_DIR}/src/actors
    ${CMAKE_CURRENT_SOURCE_DIR}/src/graph
	${TRAINING_INCLUDE_DIR}
        ${PROJECT_SOURCE_DIR}/src/actors
        $ENV{UXWELTER}/lang/cpp/src/gems/actors/common
        $ENV{UXWELTER}/lang/cpp/src/gems/actors/basic
        $ENV{UXWELTER}/lang/cpp/src/tools/graph
        $ENV{UXWELTER}/lang/c/src/tools/runtime
        $ENV{UXWELTER}/lang/c/src/gems/actors/common
        $ENV{UXWELTER}/lang/c/src/gems/edges
	
	# WELTER C Headers
    $ENV{UXWELTERC}/src/gems/actors/basic
    $ENV{UXWELTERC}/src/gems/actors/common
    $ENV{UXWELTERC}/src/tools/runtime
    $ENV{UXWELTERC}/src/tools/graph
        )

add_subdirectory(src)
