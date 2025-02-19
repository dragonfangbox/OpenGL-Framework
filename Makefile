CC=gcc

FLAGS=-Wall

compile: main.c
	$(CC) $(FLAGS) -o main main.c ./src/shaders.c ./src/sprite.c ./gladsrc/src/glad.c -I./src -lSDL2 -lcglm -lSDL2_image -lm 

run: main.c
	./main
