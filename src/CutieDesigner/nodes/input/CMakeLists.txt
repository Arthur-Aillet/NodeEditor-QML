qt_add_qml_module(input
  URI CutieDesigner.Nodes.Input
  SHARED
  QML_FILES

  ColorInput/ColorInputControl.qml
  GradientInput/GradientInputControl.qml
  NumberInput/NumberInputControl.qml
  Vec2Input/Vec2InputControl.qml

  SOURCES
  ColorInput/ColorInputNode.hpp
  ColorInput/ColorInputNode.cpp
  GradientInput/GradientInputNode.hpp
  GradientInput/GradientInputNode.cpp
  GradientInput/GradientInputList.hpp
  GradientInput/GradientInputList.cpp
  NumberInput/NumberInputNode.hpp
  NumberInput/NumberInputNode.cpp
  Vec2Input/Vec2InputNode.hpp
  Vec2Input/Vec2InputNode.cpp
  MouseInput/MouseInputNode.hpp
  MouseInput/MouseInputNode.cpp
  SceneInput/SceneInputNode.hpp
  SceneInput/SceneInputNode.cpp
  WindowInput/WindowInputNode.hpp
  WindowInput/WindowInputNode.cpp
  TimeInput/TimeInputNode.hpp
  TimeInput/TimeInputNode.cpp

  DEPENDENCIES QtQuick QtQuick.Layout NodeEditor CutieUiModule CutieDesigner.Datatypes CutieDesigner.Time
)

target_link_directories(input
  PUBLIC
  ${CMAKE_CURRENT_SOURCE_DIR}
  ${LIB_TARGET_LINK_DIRECTORIES}
  ${GSTREAMER_LIBRARY_DIRS}
)

target_include_directories(input
  PUBLIC
  ${GSTREAMER_INCLUDE_DIRS}
  ${LIB_TARGET_LINK_DIRECTORIES}

  ColorInput/
  GradientInput/
  NumberInput/
  Vec2Input/
  MouseInput/
  SceneInput/
  WindowInput/
  TimeInput/
)

target_link_libraries(input PUBLIC
  Qt6::Core
  Qt6::Gui
  Qt6::Quick
  Qt6::QuickWidgets
  Qt6::Qml
  time
  NodeEditor
  CutieUiModule
  vivid
  datatypes
  ${GSTREAMER_LIBRARIES}
)

target_compile_definitions(input PRIVATE GLM_ENABLE_EXPERIMENTAL)
