#include "../gladsrc/include/glad/glad.h"
#include <cglm/affine.h>
#include <cglm/cglm.h>
#include <cglm/mat4.h>
#include <cglm/util.h>
#include "sprite.h"

#define WINHEIGHT 960
#define WINWIDTH 1280

int vertexDataAmount = 24;

void InitSprite(Sprite *sprite, unsigned int *shaderProgram, float vertexData[24], unsigned int indiceData[6]) {
	sprite->shaderProgram = *shaderProgram;

	for(int i = 0; i < vertexDataAmount; i++) {
		sprite->vertexData[i] = vertexData[i];
	}

	for(int i = 0; i < 6; i++) {
		sprite->indiceData[i] = indiceData[i];
	}

	sprite->location[0] = 0; 
	sprite->location[1] = 0; 
	sprite->location[2] = 0; 

	sprite->scale[0] = 1; 
	sprite->scale[1] = 1; 
	sprite->scale[2] = 1; 

/*
		1.0, 1.0, 0.0, // top right  
		1.0, -1.0, 0.0, //bottom right
		-1.0, 1.0, 0.0, // top left
		-1.0, -1.0, 0.0// bottom left
					   
		0, 1, 3,// first triangle
		1, 2, 3	// second triangle
*/

	glGenVertexArrays(1, &sprite->VAO);
	glBindVertexArray(sprite->VAO);

	glGenBuffers(1, &sprite->VBO);
	glGenBuffers(1, &sprite->EBO);

	glBindBuffer(GL_ARRAY_BUFFER, sprite->VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(sprite->vertexData), sprite->vertexData, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, sprite->EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(sprite->indiceData), sprite->indiceData, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(0));
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glEnableVertexAttribArray(0);
}


void SetSpriteLocation(Sprite* sprite, float x, float y) {
	sprite->location[0] = x;
	sprite->location[1] = y;

	mat4 transform;
	vec3 transformVec = {x / WINWIDTH, y / WINHEIGHT, 0};
	glm_mat4_identity(transform);	
	glm_translate(transform, transformVec);

	unsigned int transformLoc = glGetUniformLocation(sprite->shaderProgram, "transform");
	glUniformMatrix4fv(transformLoc, 1, GL_FALSE, transform[0]);
}

void MoveSprite(Sprite *sprite, float x, float y) {
	sprite->location[0] += x;
	sprite->location[1] += y;

	mat4 transform;
	vec3 transformVec = {sprite->location[0] / WINWIDTH, sprite->location[1] / WINHEIGHT, 0};
	glm_mat4_identity(transform);	
	glm_translate(transform, transformVec);

	unsigned int transformLoc = glGetUniformLocation(sprite->shaderProgram, "transform");
	glUniformMatrix4fv(transformLoc, 1, GL_FALSE, transform[0]);
}

void ScaleSprite(Sprite *sprite, float x, float y) {
	sprite->scale[0] = x;
	sprite->scale[1] = y;
	mat4 scale;
	glm_mat4_identity(scale);	
	glm_scale(scale, sprite->scale);

	unsigned int scaleLoc = glGetUniformLocation(sprite->shaderProgram, "scale");
	glUniformMatrix4fv(scaleLoc, 1, GL_FALSE, scale[0]);
}

void DrawSprite(Sprite *sprite) {
	glUseProgram(sprite->shaderProgram);
	glBindVertexArray(sprite->VAO);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
}
