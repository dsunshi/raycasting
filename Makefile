main: main.c
	gcc -o main $< -Wall -Wextra -Wpedantic -Wconversion -Wshadow -Wformat=2 -lraylib -lGL -lm -lpthread -ldl -lrt -lX11

.PHONY: run
run: main
	./main

.PHONY: clean
clean:
	rm main
