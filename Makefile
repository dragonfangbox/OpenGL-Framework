CC=gcc

FLAGS=-Wall

compile: main.c
	$(CC) $(FLAGS) -o main main.c ./shaders.c ./sprite.c ./gladsrc/src/glad.c -lSDL2 -lcglm -lSDL2_image -lm 

run: main.c
	./main
