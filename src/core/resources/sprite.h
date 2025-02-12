#pragma once
#ifndef SPRITE_H
#define SPRITE_H

#include <stdio.h>
#include <stdlib.h>
#include <raylib.h>

typedef struct Animation
{
	char* name;
	unsigned int fromIndex;
	unsigned int toIndex;
	unsigned int loop;
	unsigned int speed;
}SpriteAnimation;

typedef struct Frame
{
	int x;
	int y;
	int width;
	int height;
}SpriteFrame;

typedef struct Sprite
{
	char* name;
	unsigned int textureIndex;
	unsigned int x;
	unsigned int y;
	int width;
	int height;
	Vector2 origin;

	unsigned int framesCount;
	unsigned int nextFrame;
	SpriteFrame** frames;

	unsigned int animationsCount;
	SpriteAnimation** animations;
} Sprite;

typedef struct SpriteNode
{
	Sprite* sprite;
	struct SpriteNode* left;
	struct SpriteNode* right;
}SpriteNode;

Sprite* CreateSprite(const char* name, unsigned int textureIndex, unsigned int x, unsigned int y, unsigned int width, unsigned int height, Vector2 origin, unsigned int framesCount);
int AddSpriteFrame(Sprite* sprite, unsigned int x, unsigned int y, unsigned int width, unsigned int height);
int AddSpriteAnimation(Sprite* sprite, const char* name, unsigned int fromIndex, unsigned int toIndex, unsigned int loop, unsigned int speed);
SpriteNode* InsertSpriteNode(SpriteNode* node, Sprite* sprite);
Sprite* TreeSpriteSearch(SpriteNode* node, const char* name);
void UnloadTreeSpriteNode(SpriteNode* node);

#endif // !SPRITE_H
