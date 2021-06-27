QT += core gui widgets
CONFIG += c++17

LIBPATH = $${PWD}/../libs

INCLUDEPATH += $${LIBPATH}/lz4/include
LIBS += -L$${LIBPATH}/lz4/bin -lliblz4

SOURCES += \
    Unpack.cpp \
    main.cpp \
    mainwindow.cpp

HEADERS += \
    Unpack.h \
    mainwindow.h

FORMS += \
    PacketUnpack.ui \
    mainwindow.ui
