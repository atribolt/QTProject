add_library(lz4-lite STATIC
  ${LIB_HEADERS}/lz4-lite/lz4-lite.h
  ${LIB_HEADERS}/lz4-lite/crc32.h
  src/lz4-lite.c
)

target_include_directories(lz4-lite
  PUBLIC ${LIB_HEADERS}
)
