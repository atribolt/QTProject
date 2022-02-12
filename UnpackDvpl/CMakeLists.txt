find_package(Qt5 COMPONENTS Widgets Gui Core)

set(CMAKE_AUTOMOC yes)
set(CMAKE_AUTOUIC yes)
set(CMAKE_AUTORCC yes)

add_executable(DvplToolGui
  ActionDecode.h
  ActionEncode.h
  ActionDecode.cpp
  ActionEncode.cpp

  DvplSign.h
  DvplSign.cpp

  MassDvplTool.h
  MassDvplTool.cpp

  Dvpl.h
  Dvpl.cpp

  DvplData.h
  DvplData.cpp

  mainwindow.h
  mainwindow.cpp

  main.cpp

  mainwindow.ui
  PacketUnpack.ui
)

target_include_directories(DvplToolGui PUBLIC ${LIB_HEADERS})

target_link_libraries(DvplToolGui
  Qt5::Gui
  Qt5::Core
  Qt5::Widgets
  lz4-lite
)
