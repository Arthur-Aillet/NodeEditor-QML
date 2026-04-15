qt6_add_library(graph STATIC)

qt_add_qml_module(graph
    URI CutieDesigner.Graph
    QML_FILES
    Graph.qml
    DEPENDENCIES QtQuick
    SOURCES
    GraphModel.cpp
    GraphModel.hpp
)

include_directories(.)

target_link_libraries(graph
    PRIVATE Qt6::Quick QtNodes::QtNodes
)
