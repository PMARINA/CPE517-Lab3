build/sim: src/shell.c src/pipe.c
	mkdir -p build
	gcc -g -O2 $^ -o $@

.PHONY: clean
clean:
	rm -rf build
