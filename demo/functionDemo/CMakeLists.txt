set(DemoName functionDemo)

aux_source_directory(. SRC_LIST)

add_executable(${DemoName} ${SRC_LIST})

target_link_libraries(${DemoName}
    Swordfish
)
