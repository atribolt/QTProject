TEMPLATE = lib

TARGET = lz4
DESTDIR = $${PWD}/bin/

message($$DESTDIR)

HEADERS += \
  include/lz4.h
  include/crc32.h
  
SOURCES += \
  src/lz4.c
