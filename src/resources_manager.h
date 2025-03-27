#pragma once
#ifndef RESOURCES_MANAGER_H
#define RESOURCES_MANAGER_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <raylib.h>
#include "sprite.h"
#include "tilemap.h"

typedef struct
{
  bool success;
  int textureIndex;
  int textureWidth;
  int textureHeight;
} TextureResult;

typedef struct
{
  int textureCount;
  Texture2D* textures;
  int* textureErrorIndexes;
  int textureErrors;
  SpriteNode* sprites;
  TilesetPackNode* tsPacks;
  TileMapNode* maps;
} ResourcesManager;

void InitResourcesManager(ResourcesManager* manager);

TextureResult AddTexture(ResourcesManager* manager, const char* filename);
void RemoveTextureIndexFromError(ResourcesManager* manager);
void PushTextureIndexForError(ResourcesManager* manager, int index);

void AddSprite(ResourcesManager* manager, ResSprite* sprite);
ResSprite* GetSprite(ResourcesManager* manager, const char* name);
ResFrame* GetSpriteFrame(ResSprite* sprite, int index);
ResAnimation* GetSpriteAnimation(ResSprite* sprite, const char* name);

void AddTilesetPack(ResourcesManager* manager, TilesetPack* pack);
TilesetPack* GetTilesetPack(ResourcesManager* manager, const char* name);

void AddTileMap(ResourcesManager* manager, TileMap* map);
TileMap* GetTileMap(ResourcesManager* manager, const char* name);

void UnloadResourcesManager(ResourcesManager* manager);

#endif // !RESOURCES_MANAGER_H
