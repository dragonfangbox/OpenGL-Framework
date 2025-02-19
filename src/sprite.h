#pragma once
#include <cglm/cglm.h>
#include <cglm/types.h>
#include "../gladsrc/include/glad/glad.h"

typedef struct Sprite {
	float vertexData[24];
	unsigned int indiceData[6];

	unsigned int VAO;
	unsigned int VBO;
	unsigned int EBO;
	unsigned int shaderProgram;

	vec2 location;
	vec2 size;
	vec3 scale;

} Sprite;

void InitSprite(Sprite *sprite, unsigned int *shaderProgram, float vertexData[24], unsigned int indiceData[6]);

void DrawSprite(Sprite *sprite);

void MoveSprite(Sprite *sprite, float x, float y);
void ScaleSprite(Sprite *sprite, float x, float y);

