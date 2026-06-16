qt_add_qml_module(CutieUiModule
  STATIC
  URI CutieUiModule
  QML_FILES
  GridController.qml
  ColorPicker.qml
  TextField.qml
  SliderField.qml

  SOURCES
  CutieWindow.hpp
  CutieWindow.cpp
  
  DEPENDENCIES QtQuick
)

target_include_directories(CutieUiModule PUBLIC
  ${CMAKE_CURRENT_SOURCE_DIR}
  ${LIB_TARGET_LINK_DIRECTORIES}
)

target_link_libraries(CutieUiModule
  PUBLIC Qt::Core Qt::Gui Qt::Qml Qt::Quick Qt::QuickControls2
)
