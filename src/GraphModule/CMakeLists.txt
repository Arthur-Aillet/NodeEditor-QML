qt_add_qml_module(GraphModule
    URI GraphModule
    QML_FILES
    GraphView.qml
    DEPENDENCIES QtQuick
    SOURCES
    GraphModel.cpp
    GraphModel.hpp
    MyNodeEditor.hpp
    MyNodeEditor.cpp
)

target_include_directories(GraphModule PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

target_link_libraries(GraphModule
    PUBLIC Qt6::Quick QtNodes::QtNodes
)
