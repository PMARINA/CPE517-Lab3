all: build/sim buildtest

build/sim: src/shell.c src/pipe.c
	mkdir -p build
	gcc -g -O2 $^ -o $@

buildtest: utils/Mars4_5.jar utils/build_test.py
	python3 utils/build_test.py

.PHONY: clean
clean:
	rm -rf build
