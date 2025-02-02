#pragma once
#ifndef RESOURCES_H
#define RESUURCES_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <raylib.h>
#include "resources/sprite.h"
#include "resources/tilemap.h"

typedef struct TextureResult
{
	int status;
	unsigned int textureIndex;
	unsigned int textureWidth;
	unsigned int textureHeight;
}TextureResult;

typedef struct ResourcesManager
{
	unsigned int textureCount;
	Texture2D* textures;
	SpriteNode* spritesRoot;
	TilesetPackNode* tilesetsPacksRoot;
	TileMapNode* tilemapsRoot;
}ResourcesManager;

void InitResourcesManager(ResourcesManager* manager);
TextureResult AddTexture(ResourcesManager* manager, const char* filename);

void AddSprite(ResourcesManager* manager, Sprite* sprite);
Sprite* GetSprite(ResourcesManager* manager, const char* name);
SpriteFrame* GetSpriteFrame(Sprite* sprite, unsigned int index);
SpriteAnimation* GetSpriteAnimation(Sprite* sprite, const char* name);

void AddTilesetsPack(ResourcesManager* manager, TilesetPack* pack);
TilesetPack* GetTilesetPack(ResourcesManager* manager, const char* name);

void AddTilemap(ResourcesManager* manager, TileMap* map);
TileMap* GetTileMap(ResourcesManager* manager, const char* name);

void UnloadTreeSpriteNode(SpriteNode* node);
void UnloadResourcesManager(ResourcesManager* manager);

#endif // !RESOURCES_H
