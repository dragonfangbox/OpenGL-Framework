#pragma once
#include <cglm/cglm.h>
#include <cglm/types.h>
#include "../gladsrc/include/glad/glad.h"

typedef struct Sprite {
	float vertexData[28];
	unsigned int indiceData[6];
	

	unsigned int VAO;
	unsigned int VBO;
	unsigned int EBO;
	unsigned int Texture;
	unsigned int shaderProgram;

	vec3 location;
	vec3 scale;

	mat4 model;

	float rotation;

} Sprite;

void InitSprite(Sprite *sprite, unsigned int *shaderProgram, float vertexData[24], unsigned int indiceData[6]);
void SetSpriteTexture(Sprite* sprite, const char* filename);

void DrawSprite(Sprite *sprite);

void SetSpriteLocation(Sprite* sprite, float x, float y);
void MoveSprite(Sprite *sprite, float x, float y);
void ScaleSprite(Sprite *sprite, float x, float y);

