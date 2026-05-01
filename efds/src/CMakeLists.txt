

# Paths within this project
set(EFDS_ACTORS ${CMAKE_CURRENT_SOURCE_DIR}/actors)
set(EFDS_GRAPH  ${CMAKE_CURRENT_SOURCE_DIR}/graph)

# Training module — provides StrongClassifier, WeakClassifier, HaarFeature
set(TRAINING_SRC ${CMAKE_CURRENT_SOURCE_DIR}/../../training/src)

set(EFDS_SRCS
#    ${EFDS_ACTORS}/classifierActor.cpp
    ${EFDS_ACTORS}/integral_image.cpp
    ${EFDS_ACTORS}/iis_source.cpp
#    ${EFDS_GRAPH}/efds_graph.cpp
    ${EFDS_ACTORS}/iis_sink.cpp
#    ${EFDS_ACTORS}/iis_source.cpp
     ${EFDS_ACTORS}/test_source_sink.cpp
#    ${TRAINING_SRC}/HaarFeature.cpp
#    ${TRAINING_SRC}/WeakClassifier.cpp
#    ${TRAINING_SRC}/StrongClassifier.cpp
)

add_library(efds ${EFDS_SRCS})
target_link_libraries(efds
    $ENV{UXWELTER}/lang/cpp/lib/libwelt_cpp.a
    $ENV{UXWELTERC}/lib/libwelt_c.a
)

add_executable(test_source_sink.exe ${EFDS_ACTORS}/test_source_sink.cpp)
target_link_libraries(test_source_sink.exe efds)
# Install the efds library to the root project directory
INSTALL(TARGETS efds DESTINATION ${PROJECT_SOURCE_DIR})

# Install the test executable to the root project directory
INSTALL(TARGETS test_source_sink.exe DESTINATION ${PROJECT_SOURCE_DIR})
