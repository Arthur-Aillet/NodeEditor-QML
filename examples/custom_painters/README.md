# Customizing Node & Connections visuals

To configure the looks of nodes or connection, you can set your own QQmlComponents in the DataFlowContext:
```c++
  ...

  const auto context = new NodeEditor::DataFlowContext(graph);

  context->setNodePainter(std::make_unique<QQmlComponent>(&engine, "examples.custom_painters", "CustomNodePainter"));
  context->setConnectionPainter(std::make_unique<QQmlComponent>(&engine, "examples.custom_painters", "CustomConnectionPainter"));
```

The given node and connection **QQmlComponent** are expected to be, respectively, derived from the [**AbstractNodePainter**](../../src/nodes/AbstractNodePainter.qml) and the [**AbstractConnectionPainter**](../../src/connections/AbstractConnectionPainter.qml) components.

These base components define some variables that need to be overridden to let the rest of the NodeEditor-QML components know how to interact with your custom visuals. They also have some properties set at creation that provide access to necessary elements to create the visuals, notably, respectively, [**NodeObject**](../../src/nodes/NodeObject.qml) and [**ConnectionObject**](../../src/connections/ConnectionObject.qml). These objects are, among other things, responsible for handling the user interaction (drag, delete, hover).

![how does it looks](../../assets/custom_painters-examples.png)

To try out the custom_painters example, at the project root, run:

```sh
cmake -DBUILD_EXAMPLES=on -S . -B ./build -G "Unix Makefiles"

make --no-print-directory -C build

./build/examples/custom_painters/custom_painters
```


