main: main.c
	gcc -o main $< -lraylib -lGL -lm -lpthread -ldl -lrt -lX11

.PHONY: run
run: main
	./main

.PHONY: clean
clean:
	rm main
