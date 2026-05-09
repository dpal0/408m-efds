# Paths within this project
set(EFDS_ACTORS ${CMAKE_CURRENT_SOURCE_DIR}/actors)
set(EFDS_GRAPH  ${CMAKE_CURRENT_SOURCE_DIR}/graph)
set(TRAINING_SRC ${CMAKE_CURRENT_SOURCE_DIR}/../../training/src)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
# Tell the linker where the Welter .a files live
LINK_DIRECTORIES($ENV{WELTERGEN})

# Source files for the efds library
# (Classifier + training stuff commented out until those compile cleanly)
set(EFDS_SRCS
    ${EFDS_ACTORS}/iis_source.cpp
    ${EFDS_ACTORS}/iis_sink.cpp
    ${EFDS_ACTORS}/integral_image.cpp
    ${EFDS_ACTORS}/classifierActor.cpp
    ${EFDS_GRAPH}/efds_graph.cpp
    ${TRAINING_SRC}/HaarFeature.cpp
    ${TRAINING_SRC}/WeakClassifier.cpp
    ${TRAINING_SRC}/StrongClassifier.cpp
)

# Build the efds static library (this produces libefds.a, like libcount_bright_pixels_src.a)
add_library(efds ${EFDS_SRCS})

# Build the test executable
add_executable(test_source_sink.exe ${EFDS_ACTORS}/test_source_sink.cpp)
add_executable(efds_graph_driver.exe ${EFDS_GRAPH}/efds_graph_driver.cpp)

# Link the test exe against efds + the actual Welter libs that exist on the server
target_link_libraries(test_source_sink.exe
    efds
    welt_cpp_graph
    welt_cpp_actor
    welt_cpp_actors_basic
    welt_cpp_actors_image
    welt_cpp_runtime
    welt_cpp_utils
    welt_cpp_graph_basic
    welt_c_app_basic
    welt_c_actors_basic
    welt_c_actors_common
    welt_c_edges
    welt_c_runtime
    welt_c_graph_common
    #welt_c_dsg_actor
    welt_c_dsg_actors_common
    welt_c_dsg_scheduler
    welt_c_add_cdsg
    #welt_c_util
    welt_c_ref_actor
    welt_c_router_basic
)

target_link_libraries(efds_graph_driver.exe
    efds
    welt_cpp_graph
    welt_cpp_actor
    welt_cpp_actors_basic
    welt_cpp_actors_image
    welt_cpp_runtime
    welt_cpp_utils
    welt_cpp_graph_basic
    welt_c_app_basic
    welt_c_actors_basic
    welt_c_actors_common
    welt_c_edges
    welt_c_runtime
    welt_c_graph_common
    welt_c_dsg_actors_common
    welt_c_dsg_scheduler
    welt_c_add_cdsg
    welt_c_ref_actor
    welt_c_router_basic
)

# Install both to the project root (so libefds.a appears next to src/, like the welter example)
INSTALL(TARGETS efds DESTINATION ${PROJECT_SOURCE_DIR})
INSTALL(TARGETS test_source_sink.exe DESTINATION ${PROJECT_SOURCE_DIR}/tests)
INSTALL(TARGETS efds_graph_driver.exe DESTINATION ${PROJECT_SOURCE_DIR}/tests)
