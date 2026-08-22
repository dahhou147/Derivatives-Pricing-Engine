cmake_minimum_required(VERSION 3.20)
project(Pricer CXX)

# C++20 standard
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Wpedantic")

# Output directories
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)

# Find Boost (required)
find_package(Boost REQUIRED)

# Find QuantLib via pkg-config (conda install doesn't provide CMake config)
find_package(PkgConfig QUIET)
if(PkgConfig_FOUND)
    pkg_check_modules(QUANTLIB QUIET quantlib)
    pkg_check_modules(NLOPT QUIET nlopt)
endif()

find_path(NLOPT_INCLUDE_DIR NAMES nlopt.hpp)
find_library(NLOPT_LIBRARY NAMES nlopt)
find_library(NLOPT_CXX_LIBRARY NAMES nlopt_cxx)

# Main executable
add_executable(pricer
    src/main.cpp
    src/models/BlackScholes.cpp
    src/models/HestonModel.cpp
    src/greeks/BlackScholesGreeks.cpp
    src/greeks/HestonGreeks.cpp
    src/calibration/VolatilitySurfaceCalibrator.cpp
)

# Include dirs for custom headers
target_include_directories(pricer PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/src/types
    ${CMAKE_CURRENT_SOURCE_DIR}/src/models
    ${CMAKE_CURRENT_SOURCE_DIR}/src/greeks
    ${CMAKE_CURRENT_SOURCE_DIR}/src/utils
    ${CMAKE_CURRENT_SOURCE_DIR}/src/calibration
    ${Boost_INCLUDE_DIRS}
)

if(QUANTLIB_FOUND)
    target_include_directories(pricer PRIVATE ${QUANTLIB_INCLUDE_DIRS})
    target_link_directories(pricer PRIVATE ${QUANTLIB_LIBRARY_DIRS})
    target_link_libraries(pricer PRIVATE ${QUANTLIB_LIBRARIES})
    target_compile_definitions(pricer PRIVATE USE_QUANTLIB)
    message(STATUS "QuantLib ${QUANTLIB_VERSION} found via pkg-config")
else()
    message(STATUS "QuantLib not found - building without QuantLib support")
endif()

if(NLOPT_INCLUDE_DIR AND NLOPT_LIBRARY AND NLOPT_CXX_LIBRARY)
    target_include_directories(pricer PRIVATE ${NLOPT_INCLUDE_DIR})
    target_link_libraries(pricer PRIVATE ${NLOPT_CXX_LIBRARY} ${NLOPT_LIBRARY})
    message(STATUS "NLOpt found")
else()
    message(STATUS "NLOpt not found - optimization support disabled")
endif()

# Link Boost (always required)
target_link_libraries(pricer PRIVATE ${Boost_LIBRARIES})

find_program(CLANG_FORMAT_EXE clang-format)
if(CLANG_FORMAT_EXE)
    file(GLOB_RECURSE PRICER_FORMAT_SOURCES CONFIGURE_DEPENDS
        "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp"
        "${CMAKE_CURRENT_SOURCE_DIR}/src/*.hpp"
        "${CMAKE_CURRENT_SOURCE_DIR}/src/*.h"
    )

    add_custom_target(format
        COMMAND ${CLANG_FORMAT_EXE} -i ${PRICER_FORMAT_SOURCES}
        WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
        COMMENT "Formatting C++ sources with clang-format"
        VERBATIM
    )

    add_custom_target(format-check
        COMMAND ${CLANG_FORMAT_EXE} --dry-run --Werror ${PRICER_FORMAT_SOURCES}
        WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
        COMMENT "Checking C++ formatting with clang-format"
        VERBATIM
    )
else()
    message(WARNING "clang-format not found; format targets disabled")
endif()

# Optional: Add tests subdirectory if tests exist
if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/tests")
    enable_testing()
    add_subdirectory(tests)
endif()
