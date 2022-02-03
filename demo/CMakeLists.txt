# Add executable called "helloDemo" that is built from the source files

add_executable (demo demo.c)

include_directories(../portabledb)

target_link_libraries (demo portabledb)
