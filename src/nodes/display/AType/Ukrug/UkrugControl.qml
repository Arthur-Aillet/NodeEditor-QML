import QtQuick
import CutieDesignerModule
import CutieUiModule

GridController {
    id: root
    required property UkrugNode node

    object: node

    model: [
        {
            name: "K",
            type: GridController.Slider,
            property: "k",
            max: 0.4,
            ease: SliderField.Cubic
        },
        {
            name: "Smooth Factor",
            type: GridController.Slider,
            property: "smoothFactor",
            ease: SliderField.Quart
        },
        {
            name: "Scale",
            type: GridController.Slider,
            property: "scale",
            ease: SliderField.Quad
        },
        {
            name: "Fill",
            type: GridController.Slider,
            property: "fill"
        },
        {
            name: "Base Color",
            type: GridController.Color,
            property: "baseColor"
        }
    ]
}
