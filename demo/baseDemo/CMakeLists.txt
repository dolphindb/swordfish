cmake_minimum_required(VERSION 3.4.1)
set(DemoName baseDemo)

aux_source_directory(. SRC_LIST)

add_compile_options(-std=c++11 -DLINUX -D_GLIBCXX_USE_CXX11_ABI=0 -Wall -O0 -g -fPIC -Wl,-rpath,${PROJECT_SOURCE_DIR}/lib)

include_directories(
    ${PROJECT_SOURCE_DIR}/include
)
link_directories(
    ${PROJECT_SOURCE_DIR}/lib/
)

add_executable(${DemoName} ${SRC_LIST})

target_link_libraries(${DemoName}
    EmbeddedDolphinDB
)
