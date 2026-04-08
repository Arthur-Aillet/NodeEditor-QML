##
## Project : RType
## File : Makefile
##

all:	build

build:
	cmake -DDEBUG_MODE=on -S . -B ./build
	make --no-print-directory -C build

release:
	cmake -DDEBUG_MODE=off -S . -B ./build
	make --no-print-directory -C build

debug:
	cmake -S . -B ./build -DCMAKE_BUILD_TYPE=Debug
	make --no-print-directory -C build

run:
	cmake -DRUN=on -DDEBUG_MODE=on -S . -B ./build
	make --no-print-directory -C build
	make --no-print-directory -C build run

run_release:
	cmake -DRUN=on -DDEBUG_MODE=off -S . -B ./build
	make --no-print-directory -C build
	make --no-print-directory -C build run

clean:
	rm -rf ./build/

fclean: clean
	rm -rf ./lib/
	rm -rf ./bin/

re: fclean build

.PHONY: all build release run run_release clean fclean re