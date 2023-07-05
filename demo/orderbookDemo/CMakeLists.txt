set(DemoName orderbookDemo)

aux_source_directory(. SRC_LIST)

add_executable(${DemoName} ${SRC_LIST})

include_directories(/home/shepherd/swordfish/include)

target_link_libraries(${DemoName}
    Swordfish
)