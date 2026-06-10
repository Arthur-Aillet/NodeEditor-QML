qt_add_qml_module(stats
  STATIC
  URI CutieDesigner.Stats
  QML_FILES
  StatsView.qml
  SimpleFpsCounter.qml
  PaneBackground.qml
  DEPENDENCIES QtQuick
  SOURCES
  ./FrequencyMonitor/FrequencyMonitor.cpp
  ./FrequencyMonitor/FrequencyMonitor.hpp
  RESOURCES
  ./InnerShadow/InnerShadow.qml
  ./InnerShadow/innershadow.vert.qsb
  ./InnerShadow/innershadow.frag.qsb
  ./InnerShadow/BlurHelper.qml
  ./InnerShadow/bluritems.frag.qsb
  ./InnerShadow/bluritems.vert.qsb
  ./loading.svg
)

target_include_directories(stats PUBLIC
  ${CMAKE_CURRENT_SOURCE_DIR}
  ${LIB_TARGET_LINK_DIRECTORIES}
  ./FrequencyMonitor
)

target_link_libraries(stats
  PUBLIC Qt::Core Qt::Gui Qt::Qml Qt::Quick Qt::QuickControls2 NodeEditor
)
