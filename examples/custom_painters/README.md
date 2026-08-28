# Customizing Node & Connections visuals

To configure

```c++
  ...

  const auto context = new NodeEditor::DataFlowContext(graph);

  context->setNodePainter(std::make_unique<QQmlComponent>(&engine, "examples.custom_painters", "CustomNodePainter"));
  context->setConnectionPainter(std::make_unique<QQmlComponent>(&engine, "examples.custom_painters", "CustomConnectionPainter"));
```

To try out the custom_painters example, at the project root, run:

```sh
cmake -DBUILD_EXAMPLES=on -S . -B ./build -G "Unix Makefiles"

make --no-print-directory -C build

./build/examples/custom_painters/custom_painters
```


