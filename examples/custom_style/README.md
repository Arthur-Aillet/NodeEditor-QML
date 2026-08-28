To try out the custom_style example, at the project root, run:

```sh
cmake -DBUILD_EXAMPLES=on -S . -B ./build -G "Unix Makefiles"

make --no-print-directory -C build

./build/examples/custom_style/custom_style
```
