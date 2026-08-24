cmake_minimum_required(VERSION 3.10..4.0)

set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_STANDARD 17)
set(QT_QML_GENERATE_QMLLS_INI ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON)

qt_policy(SET QTP0001 NEW)
qt_policy(SET QTP0004 NEW)
qt_policy(SET QTP0005 NEW)

qt_add_executable(calculator
  main.cpp
)

target_link_libraries(calculator PUBLIC
  Qt6::Core
  Qt6::Gui
  Qt6::Quick
  Qt6::Qml
  NodeEditor
)

qt_add_qml_module(calculator
  URI examples.calculator

  QML_FILES
  App.qml
  DisplayControl.qml
  InputControl.qml

  SOURCES
  AdditionNode.hpp
  DecimalData.cpp
  DecimalData.hpp
  DisplayNode.cpp
  DisplayNode.hpp
  DivisionNode.hpp
  InputNode.cpp
  InputNode.hpp
  MathOperationNodeModel.cpp
  MathOperationNodeModel.hpp
  MultiplicationNode.hpp
  SubtractionNode.hpp

  IMPORTS
    TARGET NodeEditor
)
