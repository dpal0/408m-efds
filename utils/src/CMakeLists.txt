SET(CMAKE_CXX_STANDARD 14)
SET(source_list
	EvalMetrics.cpp)
ADD_LIBRARY(evalmetrics ${source_list})
INSTALL(TARGETS evalmetrics Destination ${PROJECT_SOURCE_DIR}/utils/src)
