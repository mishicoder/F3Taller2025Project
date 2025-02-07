#include "resources_manager.h"

void InitResourcesManager(ResourcesManager* manager)
{
	manager->textureCount = 0;
	manager->textures = NULL;
	manager->spritesRoot = NULL;
	manager->tilesetsPacksRoot = NULL;
	manager->tilemapsRoot = NULL;
}

TextureResult AddTexture(ResourcesManager* manager, const char* filename)
{
	TextureResult result = { 0 };
	result.status = -1;

	Texture2D texture = LoadTexture(filename);
	if (texture.id == 0)
	{
		printf("Error al cargar la textura en la ruta: \'%s\'.\n", filename);
		return result;
	}

	manager->textureCount += 1;
	Texture2D* memTemp = (Texture2D*)realloc(manager->textures, manager->textureCount * sizeof(Texture2D));
	if (memTemp == NULL)
	{
		printf("Error al asignar memoria para la textura \'%s\'.\n", filename);
		manager->textureCount -= 1;
		return result;
	} 

	manager->textures = memTemp;
	manager->textures[manager->textureCount - 1] = texture;

	result.status = 0;
	result.textureIndex = manager->textureCount - 1;
	result.textureWidth = texture.width;
	result.textureHeight = texture.height;

	return result;
}

void AddSprite(ResourcesManager* manager, Sprite* sprite)
{
	manager->spritesRoot = InsertSpriteNode(manager->spritesRoot, sprite);
}

Sprite* GetSprite(ResourcesManager* manager, const char* name)
{
	return TreeSpriteSearch(manager->spritesRoot, name);
}

SpriteFrame* GetSpriteFrame(Sprite* sprite, unsigned int index)
{
	if (sprite == NULL) return NULL;
	if (sprite->framesCount <= 0) return NULL;
	if (index < 0 || index >= sprite->framesCount) return NULL;
	return sprite->frames[index];
}

SpriteAnimation* GetSpriteAnimation(Sprite* sprite, const char* name)
{
	if (sprite == NULL) return NULL;
	if (sprite->animationsCount <= 0) return NULL;

	for (int i = 0; i < sprite->animationsCount; i++)
	{
		if(sprite->animations[i]->name != NULL)
		{
			if (strcmp(name, sprite->animations[i]->name) == 0)
				return sprite->animations[i];
		}
	}

	return NULL;
}

void AddTilesetsPack(ResourcesManager* manager, TilesetPack* pack)
{
	manager->tilesetsPacksRoot = InsertTilesetPackNode(manager->tilesetsPacksRoot, pack);
}

TilesetPack* GetTilesetPack(ResourcesManager* manager, const char* name)
{
	return TreeTilesetPackSearch(manager->tilesetsPacksRoot, name);
}

void AddTilemap(ResourcesManager* manager, TileMap* map)
{
	manager->tilemapsRoot = InsertTilesetPackNode(manager->tilemapsRoot, map);
}

TileMap* GetTileMap(ResourcesManager* manager, const char* name)
{
	return TreeTileMapSearch(manager->tilemapsRoot, name);
}

void UnloadResourcesManager(ResourcesManager* manager)
{
	if (manager == NULL) 
	{
		printf("La expresión es nula\n");
		return;
	}
	if (manager->textureCount > 0)
	{
		for (int i = 0; i < manager->textureCount; i++)
		{
			if(manager->textures[i].id != 0)
				UnloadTexture(manager->textures[i]);
		}
		free(manager->textures);
	}

	UnloadTreeSpriteNode(manager->spritesRoot);
	UnloadTreeTilesetsPackNode(manager->tilesetsPacksRoot);
	UnloadTreeTileMapNode(manager->tilemapsRoot);
	manager->spritesRoot = NULL;
}
