#include "../gladsrc/include/glad/glad.h"
#include <cglm/affine.h>
#include <cglm/cglm.h>
#include <cglm/mat4.h>
#include <cglm/util.h>

#define STB_IMAGE_IMPLEMENTATION
#include "../stb_image.h"

#include "sprite.h"

#define WINHEIGHT 960
#define WINWIDTH 1280

const int vertexDataAmount = 28;

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

	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 7 * sizeof(float), (void*)(0));
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 7 * sizeof(float), (void*)(2 * sizeof(float)));
	glEnableVertexAttribArray(1);

	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 7 * sizeof(float), (void*)(5 * sizeof(float)));
	glEnableVertexAttribArray(2);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glEnableVertexAttribArray(0);
}


static void SetModelMatrix(Sprite* sprite) {
	vec3 rotateVec = {1.0, 0.0, 0.0};
	glm_mat4_identity(sprite->model);

	vec3 translation = {sprite->location[0] / WINWIDTH, sprite->location[1] / WINHEIGHT, 0};

	glm_translate(sprite->model, translation);
	glm_rotate(sprite->model, glm_rad(sprite->rotation), rotateVec);
	glm_scale(sprite->model, sprite->scale);
}


static unsigned int LoadTexture(const char* filename) {
	int width;
	int height;
	int colorChannels;

	stbi_set_flip_vertically_on_load(true);
	unsigned char* image = stbi_load(filename, &width, &height, &colorChannels, 0);

	if(!image) {
		return 0;
	}

	unsigned int texture;
	glGenTextures(1, &texture);

	glBindTexture( GL_TEXTURE_2D,texture);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
	glGenerateMipmap(GL_TEXTURE_2D);
	stbi_image_free(image);

	return texture;
}


void SetSpriteTexture(Sprite* sprite, const char* filename) {
	unsigned int texture = LoadTexture(filename);
	
	if(texture < 1) {
		printf("texture failed ohhh noooo\n");
	}

	sprite->Texture = texture;
}

void SetSpriteLocation(Sprite* sprite, float x, float y) {
	sprite->location[0] = x;
	sprite->location[1] = y;

//mat4 transform;
//	vec3 transformVec = {x / WINWIDTH, y / WINHEIGHT, 0};
//	glm_mat4_identity(transform);	
//	glm_translate(transform, transformVec);

//	unsigned int transformLoc = glGetUniformLocation(sprite->shaderProgram, "transform");
//	glUniformMatrix4fv(transformLoc, 1, GL_FALSE, transform[0]);
}


void MoveSprite(Sprite *sprite, float x, float y) {
	sprite->location[0] += x - sprite->location[0];
	sprite->location[1] += y - sprite->location[1];

//	mat4 transform;
//	vec3 transformVec = {sprite->location[0] / WINWIDTH, sprite->location[1] / WINHEIGHT, 0};
//	glm_mat4_identity(transform);	
//	glm_translate(transform, transformVec);

//	unsigned int transformLoc = glGetUniformLocation(sprite->shaderProgram, "transform");
//	glUniformMatrix4fv(transformLoc, 1, GL_FALSE, transform[0]);
}


void ScaleSprite(Sprite *sprite, float x, float y) {
	sprite->scale[0] = x;
	sprite->scale[1] = y;
	sprite->scale[2] = 0;

//mat4 scale;
//	glm_mat4_identity(scale);	
//	glm_scale(scale, sprite->scale);

//	unsigned int scaleLoc = glGetUniformLocation(sprite->shaderProgram, "scale");
//	glUniformMatrix4fv(scaleLoc, 1, GL_FALSE, scale[0]);
}


void DrawSprite(Sprite *sprite) {
	SetModelMatrix(sprite);

	glUseProgram(sprite->shaderProgram);

	unsigned int modelLoc = glGetUniformLocation(sprite->shaderProgram, "model");
	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, sprite->model[0]);

	unsigned int texLoc = glGetUniformLocation(sprite->shaderProgram, "Texture1");
	glUniform1i(texLoc, 0);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, sprite->Texture);	

	glBindVertexArray(sprite->VAO);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
}
