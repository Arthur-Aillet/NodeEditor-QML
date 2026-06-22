qt_add_qml_module(time
  URI CutieDesigner.Time
  SHARED
  QML_FILES

  SOURCES

  DEPENDENCIES QtQuick QtQuick.Layout NodeEditor CutieUiModule CutieDesigner.Datatypes
)

target_link_directories(time
  PUBLIC
  ${CMAKE_CURRENT_SOURCE_DIR}
  ${LIB_TARGET_LINK_DIRECTORIES}
  ${GSTREAMER_LIBRARY_DIRS}
)

target_include_directories(time
  PUBLIC
  ${GSTREAMER_INCLUDE_DIRS}

  ColorInput/
  GradientInput/
  NumberInput/
  Vec2Input/
  MouseInput/
  SceneInput/
  WindowInput/
  TimeInput/
)

target_link_libraries(time PUBLIC
  Qt6::Core
  Qt6::Gui
  Qt6::Quick
  Qt6::QuickWidgets
  Qt6::Qml
  NodeEditor
  CutieUiModule
  vivid
  datatypes
  ${GSTREAMER_LIBRARIES}
)

target_compile_definitions(time PRIVATE GLM_ENABLE_EXPERIMENTAL)
