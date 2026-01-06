cmake_minimum_required(VERSION 3.15...3.26)

project(hnswlib LANGUAGES CXX)

include(GNUInstallDirs)
include(CheckCXXCompilerFlag)

# --------------------------------------------------
# Library (header-only)
# --------------------------------------------------
add_library(hnswlib INTERFACE)
add_library(hnswlib::hnswlib ALIAS hnswlib)

target_include_directories(hnswlib INTERFACE
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
    $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
)

target_compile_features(hnswlib INTERFACE cxx_std_11)

# --------------------------------------------------
# Install
# --------------------------------------------------
install(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/hnswlib
    DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
)

install(TARGETS hnswlib EXPORT hnswlibTargets)

install(EXPORT hnswlibTargets
    FILE hnswlibConfig.cmake
    NAMESPACE hnswlib::
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/hnswlib
)

# --------------------------------------------------
# Examples and tests
# --------------------------------------------------
if(CMAKE_PROJECT_NAME STREQUAL PROJECT_NAME)
    option(HNSWLIB_EXAMPLES "Build examples and tests." ON)
else()
    option(HNSWLIB_EXAMPLES "Build examples and tests." OFF)
endif()

if(HNSWLIB_EXAMPLES)

    # Default build type (important!)
    if(NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
        set(CMAKE_BUILD_TYPE Release CACHE STRING
            "Choose the type of build." FORCE)
    endif()

    # ----------------------------------------------
    # Compiler-specific flags
    # ----------------------------------------------
    if(CMAKE_CXX_COMPILER_ID MATCHES "Clang|GNU")

        # Common warnings
        add_compile_options(-Wall -Wextra)

        # Release-only optimizations
        add_compile_options(
            $<$<CONFIG:Release>:-Ofast>
            $<$<CONFIG:Release>:-ftree-vectorize>
            $<$<CONFIG:Release>:-fopenmp>
        )

        # Debug-friendly flags
        add_compile_options(
            $<$<CONFIG:Debug>:-O0>
            $<$<CONFIG:Debug>:-g>
            $<$<CONFIG:Debug>:-fno-omit-frame-pointer>
        )

        # Architecture tuning (Release only)
        check_cxx_compiler_flag("-march=native" HAS_MARCH_NATIVE)
        if(HAS_MARCH_NATIVE)
            add_compile_options($<$<CONFIG:Release>:-march=native>)
        endif()

    elseif(CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")

        add_compile_options(
            $<$<CONFIG:Release>:/O2>
            $<$<CONFIG:Debug>:/Od>
            $<$<CONFIG:Debug>:/Zi>
        )

        add_compile_options(/EHsc /openmp)

    endif()

    # ----------------------------------------------
    # Helper macro
    # ----------------------------------------------
    function(add_hnsw_example name)
        add_executable(${name} ${ARGN})
        target_link_libraries(${name} PRIVATE hnswlib)
    endfunction()

    # ----------------------------------------------
    # Examples
    # ----------------------------------------------
    add_hnsw_example(example_search examples/cpp/example_search.cpp)
    add_hnsw_example(example_epsilon_search examples/cpp/example_epsilon_search.cpp)
    add_hnsw_example(example_multivector_search examples/cpp/example_multivector_search.cpp)
    add_hnsw_example(example_filter examples/cpp/example_filter.cpp)
    add_hnsw_example(example_replace_deleted examples/cpp/example_replace_deleted.cpp)
    add_hnsw_example(example_mt_search examples/cpp/example_mt_search.cpp)
    add_hnsw_example(example_mt_filter examples/cpp/example_mt_filter.cpp)
    add_hnsw_example(example_mt_replace_deleted examples/cpp/example_mt_replace_deleted.cpp)

    # ----------------------------------------------
    # Tests
    # ----------------------------------------------
    add_hnsw_example(multivector_search_test tests/cpp/multivector_search_test.cpp)
    add_hnsw_example(epsilon_search_test tests/cpp/epsilon_search_test.cpp)
    add_hnsw_example(test_updates tests/cpp/updates_test.cpp)
    add_hnsw_example(searchKnnCloserFirst_test tests/cpp/searchKnnCloserFirst_test.cpp)
    add_hnsw_example(searchKnnWithFilter_test tests/cpp/searchKnnWithFilter_test.cpp)
    add_hnsw_example(multiThreadLoad_test tests/cpp/multiThreadLoad_test.cpp)
    add_hnsw_example(multiThread_replace_test tests/cpp/multiThread_replace_test.cpp)
    add_hnsw_example(getDataByLabel_test tests/cpp/getDataByLabel_test.cpp)
    add_hnsw_example(main tests/cpp/main.cpp tests/cpp/sift_1b.cpp)

endif()
