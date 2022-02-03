
set(LIBRARY_SOURCES
  dbtypes.h
  database.c
  tables.c
)

add_library(database ${LIBRARY_SOURCES})

target_link_libraries(database PRIVATE btree)
