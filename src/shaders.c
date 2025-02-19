#include <stdio.h>
#include <stdlib.h>

#include "../gladsrc/include/glad/glad.h"
#include "shaders.h"


const char* ReadShader(char fileName[]) {
	FILE *file = fopen(fileName, "r");
	if(!file) {
		printf("errmm no file with that name??? stoopid.");
	}

	fseek(file, 0, SEEK_END);
	int fileSize = ftell(file);
	fseek(file, 0, SEEK_SET);

	char* shaderSource = malloc(fileSize + 1);
	fread(shaderSource, 1, fileSize, file);
	shaderSource[fileSize] = '\0';

	fclose(file);
	return shaderSource;
}

void CreateShader(unsigned int *shader, const char* shaderSource, GLenum shaderType) {
	*shader = glCreateShader(shaderType);
	glShaderSource(*shader, 1, &shaderSource, NULL);
	glCompileShader(*shader);
}

void CreateShaderProgram(unsigned int *program, unsigned int vertShader, unsigned int fragShader) {
	*program = glCreateProgram();
	glAttachShader(*program, vertShader);
	glAttachShader(*program, fragShader);
	glLinkProgram(*program);

	glDeleteShader(vertShader);
	glDeleteShader(fragShader);
}
