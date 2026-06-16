import QtQuick
import CutieUiModule
import CutieDesigner.Nodes.Input

GridController {
    required property Vec2InputNode node
    object: node

    model: [
        {
            name: "x",
            type: GridController.Number,
            property: "x"
        },
        {
            name: "y",
            type: GridController.Number,
            property: "y"
        },
    ]
}
