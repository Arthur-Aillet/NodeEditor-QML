# Calculator

![how does it looks](../../assets/calculator-example.gif)

To try out the calculator example, at the project root, run:

```sh
cmake -DBUILD_EXAMPLES=on -S . -B ./build -G "Unix Makefiles"

make --no-print-directory -C build

./build/examples/calculator/calculator
```
