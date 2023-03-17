cmake_minimum_required(VERSION 3.5)

project(demos)

set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/build/bin/)

add_subdirectory(demo/baseDemo)
add_subdirectory(demo/functionDemo)
add_subdirectory(demo/objectDemo)
add_subdirectory(demo/crudDemo)
add_subdirectory(demo/streamEngineRunDemo)
add_subdirectory(demo/streamEngineDemo)
add_subdirectory(demo/streamSubDemo)

configure_file("${PROJECT_SOURCE_DIR}/asset/dolphindb.lic" "${EXECUTABLE_OUTPUT_PATH}/dolphindb.lic" COPYONLY)
configure_file("${PROJECT_SOURCE_DIR}/asset/dolphindb.dos" "${EXECUTABLE_OUTPUT_PATH}/dolphindb.dos" COPYONLY)
