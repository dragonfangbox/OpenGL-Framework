#include <cglm/cam.h>
#include <stdio.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <cglm/cglm.h>
#include "gladsrc/include/glad/glad.h"

#include "./src/shaders.h"
#include "./src/sprite.h"

#define WINHEIGHT 960.0
#define WINWIDTH 1280.0

int main() {
	const char* fragmentShaderSource = ReadShader("./src/shaders/FragShader.frag");
	const char* vertexShaderSource = ReadShader("./src/shaders/VertShader.vert");

	if(SDL_Init(SDL_INIT_EVERYTHING) < 0) {
		printf("sdl no init ruh roh");
		return -1;
	}

	SDL_Window *window = SDL_CreateWindow("hi", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WINWIDTH, WINHEIGHT, SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN);
	SDL_GLContext GLContext = SDL_GL_CreateContext(window);
	SDL_GL_MakeCurrent(window, GLContext);

	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 0);
	
	if (!gladLoadGLLoader((GLADloadproc)SDL_GL_GetProcAddress)) {
		printf("glad load fail opakhdfhF");
		SDL_GL_DeleteContext(GLContext);
		SDL_DestroyWindow(window);
		SDL_Quit();
	    return -1;
	}

	glEnable(GL_TEXTURE_2D);
	glEnable(GL_BLEND);


	unsigned int vertShader;
	CreateShader(&vertShader, vertexShaderSource, GL_VERTEX_SHADER);
	unsigned int fragShader;
	CreateShader(&fragShader, fragmentShaderSource, GL_FRAGMENT_SHADER);
	
	unsigned int shaderProgram;
	CreateShaderProgram(&shaderProgram, vertShader, fragShader);

	int success;
	char infoLog[512];

	glGetShaderiv(vertShader, GL_COMPILE_STATUS, &success);
	if (!success) {
	 glGetShaderInfoLog(vertShader, 512, NULL, infoLog);
	 printf("ERROR::SHADER::VERTEX::COMPILATION_FAILED\n%s\n", infoLog);
	}

	glGetShaderiv(fragShader, GL_COMPILE_STATUS, &success);
	if (!success) {
		glGetShaderInfoLog(fragShader, 512, NULL, infoLog);
	    printf("ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n%s\n", infoLog);
	}

	glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
	if (!success) {
		 glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
	    printf("ERROR::SHADER::PROGRAM::LINKING_FAILED\n%s\n", infoLog);
	}	
	glDeleteShader(vertShader);
	glDeleteShader(fragShader);

	glViewport(0, 0, WINWIDTH, WINHEIGHT);

	float vertexData[24] = {
	 1.0,  1.0,  0.0, 1.0, 0.0, 0.0,
	  1.0, -1.0,0.0, 0.0, 1.0, 0.0,
	-1.0, 1.0,  0.0, 0.0, 0.0, 1.0,
	-1.0, -1.0, 0.0, 1.0, 1.0, 1.0
	};

	unsigned int indiceData[6] = {
		0,1,2,
		2,1,3
	};

	Sprite test;
	InitSprite(&test, &shaderProgram, vertexData, indiceData);

	int running = 1;
	while(running) {
		SDL_Event event;
		while(SDL_PollEvent(&event)) {
			if(event.type == SDL_QUIT) {
				running = 0;
			}
		}
		glClearColor(0.3f, 0.2f, 0.5f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		mat4 projection;
		glm_mat4_identity(projection);
		glm_ortho_default(WINWIDTH / WINHEIGHT, projection);

		mat4 model;
		glm_mat4_identity(model);

		mat4 view;
		vec3 cameraPos = {0.0, 0.0, -4.0};
		glm_mat4_identity(view);
		glm_translate(view, cameraPos);

		glUseProgram(shaderProgram);
		 
		unsigned int modelLoc = glGetUniformLocation(shaderProgram, "model");
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, model[0]);

		unsigned int viewLoc = glGetUniformLocation(shaderProgram, "view");
		glUniformMatrix4fv(viewLoc, 1, GL_FALSE, view[0]);

		unsigned int projectionLoc = glGetUniformLocation(shaderProgram, "projection");
	 	glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, projection[0]);

		MoveSprite(&test, 0, 0);
		ScaleSprite(&test, 0.2, 0.2);
		DrawSprite(&test);


		SDL_GL_SwapWindow(window);
	}

	SDL_DestroyWindow(window);
	SDL_GL_DeleteContext(GLContext);
	SDL_Quit();
	return 0;
}
