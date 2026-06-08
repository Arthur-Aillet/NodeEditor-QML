import QtQuick
import CutieDesignerModule

ShaderEffect {
    id: shader

    required property BlendNode node

    Component.onCompleted: {
        console.log("completed" + node.a + "  " + node.b);
    }

    Rectangle {
        id: a
        x: 100
        y: 100
        width: 100
        height: 100

        gradient: Gradient {
            GradientStop {
                position: 0.0
                color: "lightgreen"
            }
            GradientStop {
                position: 1.0
                color: "green"
            }
        }
        layer.enabled: true
    }

    Rectangle {
        id: b
        y: 150
        width: 100
        height: 100

        NumberAnimation on x {
            loops: Animation.Infinite
            duration: 2000
            from: 100
            to: 200
        }
        gradient: Gradient {
            orientation: Gradient.Horizontal
            GradientStop {
                position: 0.0
                color: "pink"
            }
            GradientStop {
                position: 1.0
                color: "red"
            }
        }

        // visible: false
        layer.enabled: true
    }

    property Item source: a
    property Item target: b

    property rect targetRect: target === null ? Qt.rect(0, 0, 0, 0) : Qt.rect(target.x, target.y, target.width, target.height)
    property rect sourceRect: source === null ? Qt.rect(0, 0, 0, 0) : Qt.rect(source.x, source.y, source.width, source.height)
    property int mode: 0

    Timer {
        interval: 200
        running: true
        repeat: true
        onTriggered: {
            shader.mode += 1;
            if (shader.mode == 22)
                shader.mode = 0;
        }
    }

    vertexShader: 'blend.vert.qsb'
    fragmentShader: 'blend.frag.qsb'
}
