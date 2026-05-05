qt_add_qml_module(StatsModule
  URI StatsModule
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

target_include_directories(StatsModule PUBLIC
  ${CMAKE_CURRENT_SOURCE_DIR}
  ${LIB_TARGET_LINK_DIRECTORIES}
  ./FrequencyMonitor
)

target_link_libraries(StatsModule
  PUBLIC Qt::Core Qt::Gui Qt::Qml Qt::Quick Qt::QuickControls2 QtNodes::QtNodes NodeModule
)
