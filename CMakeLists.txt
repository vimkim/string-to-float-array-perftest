cmake_minimum_required(VERSION 3.10)
project(vector_parser_perf_test CXX)

include(ExternalProject)

# Set C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Enable optimization flags for Release build
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -O3")

# RapidJSON configuration
set(RAPIDJSON_TARGET rapidjson)
set(WITH_RAPIDJSON_URL "https://github.com/CUBRID/3rdparty/raw/develop/rapidjson/v1.1.0.tar.gz")

# Set up RapidJSON as external project
ExternalProject_Add(${RAPIDJSON_TARGET}
    URL                  ${WITH_RAPIDJSON_URL}
    URL_HASH            ${WITH_RAPIDJSON_URL_HASH}
    LOG_CONFIGURE       TRUE
    LOG_BUILD           TRUE
    LOG_INSTALL         TRUE
    TIMEOUT             600
    DOWNLOAD_NO_PROGRESS 1
    INSTALL_COMMAND     ""
    CMAKE_CACHE_ARGS    -DRAPIDJSON_BUILD_TESTS:STRING=off
                        -DRAPIDJSON_BUILD_DOC:STRING=off
                        -DRAPIDJSON_BUILD_EXAMPLES:STRING=off
)

# Set RapidJSON include path
ExternalProject_Get_Property(${RAPIDJSON_TARGET} SOURCE_DIR)
set(RAPIDJSON_INCLUDES ${SOURCE_DIR}/include)

# Manual implementation library
add_library(vector_parser_manual
    src/db_vector.cpp
)
target_include_directories(vector_parser_manual
    PUBLIC ${CMAKE_SOURCE_DIR}/include
)

# RapidJSON implementation library
add_library(vector_parser_rapidjson
    src/db_vector_rapidjson.cpp
)
target_include_directories(vector_parser_rapidjson
    PUBLIC ${CMAKE_SOURCE_DIR}/include
    PUBLIC ${RAPIDJSON_INCLUDES}
)
add_dependencies(vector_parser_rapidjson ${RAPIDJSON_TARGET})

# Performance test executable for manual implementation
add_executable(perf_test_manual
    src/perf_test.cpp
)
target_link_libraries(perf_test_manual
    PRIVATE vector_parser_manual
)

# Performance test executable for RapidJSON implementation
add_executable(perf_test_rapidjson
    src/perf_test.cpp
)
target_link_libraries(perf_test_rapidjson
    PRIVATE vector_parser_rapidjson
)

# Create directories
file(MAKE_DIRECTORY ${CMAKE_SOURCE_DIR}/include)
file(MAKE_DIRECTORY ${CMAKE_SOURCE_DIR}/src)

# Ninja-compatible custom targets for running tests
add_custom_target(run_manual
    COMMAND ${CMAKE_COMMAND} -E echo "Running manual implementation test..."
    COMMAND perf_test_manual
    DEPENDS perf_test_manual
)

add_custom_target(run_rapidjson
    COMMAND ${CMAKE_COMMAND} -E echo "Running RapidJSON implementation test..."
    COMMAND perf_test_rapidjson
    DEPENDS perf_test_rapidjson
)

add_custom_target(run_perf_tests
    COMMAND ${CMAKE_COMMAND} -E echo "Running both performance tests..."
    COMMAND ${CMAKE_COMMAND} --build ${CMAKE_BINARY_DIR} --target run_manual
    COMMAND ${CMAKE_COMMAND} -E echo "\\n"
    COMMAND ${CMAKE_COMMAND} --build ${CMAKE_BINARY_DIR} --target run_rapidjson
)
