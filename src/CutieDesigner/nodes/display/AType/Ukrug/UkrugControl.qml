import QtQuick
import CutieDesigner.Nodes.Display
import CutieUiModule

GridController {
    required property UkrugNode node

    object: node

    model: [
        {
            name: "Inner Color",
            type: GridController.Color,
            property: "innerColor"
        },
        {
            name: "Outer Color",
            type: GridController.Color,
            property: "baseColor"
        },
        {
            name: "Font Size",
            type: GridController.Number,
            property: "fontSize",
            min: 1,
            max: 500
        },
        {
            name: "K",
            type: GridController.Slider,
            property: "k",
            max: 0.4,
            ease: SliderField.Cubic
        },
        {
            name: "Circle Scale",
            type: GridController.Slider,
            property: "circleScale",
            ease: SliderField.Quad
        },
        {
            name: "Smooth Factor",
            type: GridController.Slider,
            property: "smoothFactor",
            ease: SliderField.Quart
        },
        {
            name: "Points Scale",
            type: GridController.Slider,
            property: "pointsScale",
            ease: SliderField.Quad
        },
        {
            name: "Points ⇔",
            type: GridController.Slider,
            property: "pointsDistance"
        },
        {
            name: "Fill",
            type: GridController.Slider,
            property: "fill",
            ease: SliderField.Quad
        },
        {
            name: "Substract",
            type: GridController.CheckBox,
            property: "substraction"
        },
        {
            name: "Box X",
            type: GridController.Slider,
            property: "boxLimitX",
            min: 0,
            max: 2
        },
        {
            name: "Box Y",
            type: GridController.Slider,
            property: "boxLimitY",
            min: 0,
            max: 2
        },
        {
            name: "Box Radius",
            type: GridController.Slider,
            property: "boxRadius",
            ease: SliderField.Quad
        },
        {
            name: "Vanish Speed (ms)",
            type: GridController.Number,
            property: "animationOpacitySpeed",
            min: 0
        },
        {
            name: "Close Speed (ms)",
            type: GridController.Number,
            property: "animationWidthSpeed",
            min: 0
        },
        {
            name: "Change Speed (ms)",
            type: GridController.Number,
            property: "animationTransformSpeed",
            min: 0
        },
        {
            name: "In Out Factor",
            type: GridController.Slider,
            property: "inOutFactor",
            min: 0,
            max: 100,
            ease: SliderField.Quad
        },
    ]
}
