cmake_minimum_required(VERSION 3.10)
project(training)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS "-Wall")

# Shared include paths
set(TRAINING_SRC ${CMAKE_CURRENT_SOURCE_DIR}/src)
set(EFDS_ACTORS ${CMAKE_CURRENT_SOURCE_DIR}/../efds/src/actors)

include_directories(${TRAINING_SRC} ${EFDS_ACTORS})

# Shared source files used across multiple tests
set(COMMON_SRCS
    ${TRAINING_SRC}/HaarFeature.cpp
    ${TRAINING_SRC}/WeakClassifier.cpp
    ${TRAINING_SRC}/StrongClassifier.cpp
    ${EFDS_ACTORS}/integral_image.cpp
)

# test-haar_features
add_executable(test-haar_features
    ${CMAKE_CURRENT_SOURCE_DIR}/test/test-haar_features/HaarFeatureDriver.cpp
    ${TRAINING_SRC}/HaarFeature.cpp
    ${EFDS_ACTORS}/integral_image.cpp
)

# test-weak-classifier
#add_executable(test-weak-classifier
 #   ${CMAKE_CURRENT_SOURCE_DIR}/test/test-weak-classifier/WeakClassifierDriver.cpp
 #  ${TRAINING_SRC}/test01WeakClass.cpp
 #   ${TRAINING_SRC}/test02WeakClass.cpp
 #   ${TRAINING_SRC}/HaarFeature.cpp
 #   ${EFDS_ACTORS}/integral_image.cpp
#)

# test-strong_classifier
# add_executable(test-strong_classifier
#     ${CMAKE_CURRENT_SOURCE_DIR}/test/test-strong_classifier/StrongClassifierDriver.cpp
#     ${COMMON_SRCS}
# )
