cmake_minimum_required(VERSION 2.8)

project(demos)

#######################################################################################################################
# Check compiler: require g++-4.8.5
#######################################################################################################################

message(STATUS "Compiler: ${CMAKE_CXX_COMPILER} ${CMAKE_CXX_COMPILER_ID} ${CMAKE_CXX_COMPILER_VERSION}")

if (NOT "${CMAKE_CXX_COMPILER_ID}" MATCHES "GNU")
    message(FATAL_ERROR "${PROJECT_NAME} only support g++ now, but current compiler is ${CMAKE_CXX_COMPILER_ID}.")
endif ()

if (NOT CMAKE_CXX_COMPILER_VERSION VERSION_EQUAL 4.8.5)
    message(FATAL_ERROR "${PROJECT_NAME} requires g++ 4.8.5, but current g++ version is ${CMAKE_CXX_COMPILER_VERSION}.")
endif ()


#######################################################################################################################
# Compile options
#######################################################################################################################

add_compile_options(
    -std=c++11 -D_GLIBCXX_USE_CXX11_ABI=0
    -DLINUX
    -Wall
    -g -O0  # debug build
    -Wl,-rpath,${PROJECT_SOURCE_DIR}/lib
)
include_directories( ${PROJECT_SOURCE_DIR}/include )
link_directories( ${PROJECT_SOURCE_DIR}/lib/ )


#######################################################################################################################
# Config output path
#######################################################################################################################

set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/build/bin/)

configure_file("${PROJECT_SOURCE_DIR}/asset/dolphindb.lic" "${EXECUTABLE_OUTPUT_PATH}/dolphindb.lic" COPYONLY)
configure_file("${PROJECT_SOURCE_DIR}/asset/dolphindb.dos" "${EXECUTABLE_OUTPUT_PATH}/dolphindb.dos" COPYONLY)


#######################################################################################################################
# Add sub directory
#######################################################################################################################

FILE(GLOB children RELATIVE ${PROJECT_SOURCE_DIR}/demo ${PROJECT_SOURCE_DIR}/demo/*)
FOREACH(child ${children})
    IF(IS_DIRECTORY ${PROJECT_SOURCE_DIR}/demo/${child})
        add_subdirectory(${PROJECT_SOURCE_DIR}/demo/${child})
    ENDIF()
ENDFOREACH()

