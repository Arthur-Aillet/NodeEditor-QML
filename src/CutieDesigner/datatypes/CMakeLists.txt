qt_add_qml_module(datatypes
  URI CutieDesigner.Datatypes
  SHARED
  SOURCES
  ATypeCharacterData.hpp
  ColorData.hpp
  DecimalData.hpp
  GradientData.hpp
  SurfaceData.cpp
  SurfaceData.hpp
  TextData.hpp
)

target_include_directories(datatypes PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

target_link_libraries(datatypes PUBLIC
  Qt6::Core
  Qt6::Gui
  NodeEditor
)