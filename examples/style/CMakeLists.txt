cmake_minimum_required(VERSION 3.10..4.0)

set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_STANDARD 17)
set(QT_QML_GENERATE_QMLLS_INI ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON)

qt_policy(SET QTP0001 NEW)
qt_policy(SET QTP0004 NEW)
qt_policy(SET QTP0005 NEW)

qt_add_executable(style
  main.cpp
)

target_link_libraries(style PUBLIC
  Qt6::Core
  Qt6::Gui
  Qt6::Quick
  Qt6::Qml
  NodeEditor
)

qt_add_qml_module(style
  URI examples.style

  QML_FILES
  Main.qml

  SOURCES
  OutNode.hpp
  InNode.hpp
  SimpleData.hpp

  IMPORTS
    TARGET NodeEditor
)
