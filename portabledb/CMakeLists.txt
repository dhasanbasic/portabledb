
add_subdirectory(btree)
add_subdirectory(database)

add_library(portabledb INTERFACE)

target_link_libraries(portabledb PRIVATE INTERFACE btree database)
