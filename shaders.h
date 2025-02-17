#pragma once
#include "./gladsrc/include/glad/glad.h"

const char* ReadShader(char fileName[]);
void CreateShader(unsigned int *shader, const char* shaderSource, GLenum shaderType);
void CreateShaderProgram(unsigned int *program, unsigned int vertShader, unsigned int fragShader);
