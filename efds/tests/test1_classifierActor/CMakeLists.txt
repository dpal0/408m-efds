CMAKE_MINIMUM_REQUIRED(VERSION 2.8 FATAL_ERROR)
PROJECT(TestClassifierActor)

SET(CMAKE_CXX_STANDARD 14)
SET(CMAKE_EXPORT_COMPILE_COMMANDS ON)

FIND_PACKAGE(OpenCV REQUIRED)

INCLUDE_DIRECTORIES(
    ../../src/actors
    ../../src
    ../../../training/src
    $ENV{UXWELTERC}/src/gems/actors/basic
    $ENV{UXWELTERC}/src/gems/actors/common
    $ENV{UXWELTERC}/src/gems/edges
    $ENV{UXWELTERC}/src/tools/runtime
    $ENV{UXWELTERC}/src/tools/graph
    $ENV{UXWELTER}/lang/cpp/src/gems/actors/common
    $ENV{UXWELTER}/lang/cpp/src/gems/actors/basic
    $ENV{UXWELTER}/lang/cpp/src/tools/graph
    $ENV{UXWELTER}/lang/cpp/src/tools/runtime
    $ENV{UXWELTER}/lang/cpp/src/apps/basic
    ${OpenCV_INCLUDE_DIRS}
)

LINK_DIRECTORIES(
    $ENV{WELTERGEN}
    $ENV{UXTMP}
)

ADD_EXECUTABLE(
    test1
    test1.cpp
    ../../src/actors/classifierActor.cpp
    ../../src/actors/integral_image.cpp
    ../../../training/src/StrongClassifier.cpp
    ../../../training/src/WeakClassifier.cpp
    ../../../training/src/HaarFeature.cpp
)

TARGET_LINK_LIBRARIES(
    test1
    ${OpenCV_LIBS}
    welt_c_edges
    welt_c_runtime
    welt_cpp_actors_basic
    welt_cpp_actor
    welt_cpp_runtime
    welt_cpp_utils
    welt_cpp_graph
)
