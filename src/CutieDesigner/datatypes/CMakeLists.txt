qt_add_qml_module(datatypes
  URI CutieDesigner.Datatypes
  SHARED
  SOURCES
  ATypeCharacterData.hpp
  ColorData.hpp
  DecimalData.hpp
  DecimalData.cpp
  GradientData.hpp
  SurfaceData.cpp
  SurfaceData.hpp
  TextData.hpp
  Vec2Data.cpp
  Vec2Data.hpp
)

target_include_directories(datatypes PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

target_link_libraries(datatypes PUBLIC
  Qt6::Core
  Qt6::Gui
  glm
  NodeEditor
)