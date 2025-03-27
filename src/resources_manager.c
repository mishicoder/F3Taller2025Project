#include "resources_manager.h"

void InitResourcesManager(ResourcesManager *manager)
{
  manager->textureCount = 0;
  manager->textures = NULL;
  manager->sprites = NULL;
  manager->tsPacks = NULL;
  manager->maps = NULL;
}

TextureResult AddTexture(ResourcesManager *manager, const char *filename)
{
  TextureResult result = { 0 };
  result.success = false;

  Texture2D texture = LoadTexture(filename);
  if(texture.id == 0) return result;

  int index = -1;
  if(manager->textureErrors > 0)
  {
    index = manager->textureErrorIndexes[0];
    RemoveTextureIndexFromError(manager);
  }

  if(index == -1)
  {

    Texture2D* memTemp = (Texture2D*)realloc(manager->textures, (size_t)(manager->textureCount + 1)* sizeof(Texture2D));
    if(!memTemp){ return result; }
  
    manager->textures = memTemp;
    manager->textures[manager->textureCount] = texture;
    manager->textureCount += 1;

  }
  else
  {
    manager->textures[index] = texture;
  }

  result.success = true;
  result.textureIndex = index == -1 ? manager->textureCount - 1 : index;
  result.textureHeight = texture.height;
  result.textureWidth = texture.width;
  
  return result;
}

void PushTextureIndexForError(ResourcesManager *manager, int index)
{
  int* memTemp = (int*)realloc(manager->textureErrorIndexes, (size_t)(manager->textureErrors + 1) * sizeof(int));
  if(!memTemp) return;

  UnloadTexture(manager->textures[index]);
  manager->textures[index].id = 0;

  manager->textureErrorIndexes = memTemp;
  manager->textureErrorIndexes[manager->textureErrors] = index;
  manager->textureErrors += 1;
}

void RemoveTextureIndexFromError(ResourcesManager *manager)
{
  if(manager->textureErrors <= 0) return;

  int* memTemp = (int*)realloc(manager->textureErrorIndexes, (size_t)(manager->textureErrors - 1) * sizeof(int));
  if(!memTemp) return;

  for(int i = 0; i < manager->textureErrors; i++)
  {
    manager->textureErrorIndexes[i] = manager->textureErrorIndexes[i + 1];
    manager->textureErrorIndexes[i + 1] = -1;
  }
  manager->textureErrorIndexes = memTemp;
}

void AddSprite(ResourcesManager *manager, ResSprite *sprite)
{
  manager->sprites = InsertSpriteNode(manager->sprites, sprite);
}

ResSprite* GetSprite(ResourcesManager *manager, const char *name)
{
  return SearchSpriteInTree(manager->sprites, name);
}

ResFrame* GetSpriteFrame(ResSprite *sprite, int index)
{
  if(!sprite) return NULL;
  if(sprite->framesCount <= 0) return NULL;
  if(index < 0 || index >= sprite->framesCount) return NULL;
  return sprite->frames[index];
}

ResAnimation* GetSpriteAnimation(ResSprite *sprite, const char *name)
{
  if(!sprite) return NULL;
  if(sprite->animCount <= 0) { printf("El sprite <%s> no tiene animaciones\n", sprite->name); return NULL; }

  for(int i = 0; i < sprite->animCount; i++)
  {
    if(strcmp(sprite->animations[i]->name, name) == 0)
    {   
      //printf("Animacion <%s> encontrada\n", name);
      return sprite->animations[i];
    }
  } 
  
  //printf("Animacion <%s> no encontrada\n", name);
  return NULL;
}

void AddTilesetPack(ResourcesManager *manager, TilesetPack *pack)
{
  manager->tsPacks = InsertTilesetPackNode(manager->tsPacks, pack);
}

TilesetPack* GetTilesetPack(ResourcesManager *manager, const char *name)
{
  return SearchTilesetPackInTree(manager->tsPacks, name);
}

void AddTileMap(ResourcesManager *manager, TileMap *map)
{
  manager->maps = InsertTileMapNode(manager->maps, map);
}

TileMap* GetTileMap(ResourcesManager *manager, const char *name)
{
  return SearchTileMapInTree(manager->maps, name);
}

void UnloadResourcesManager(ResourcesManager *manager)
{
  for(int i = 0; i < manager->textureCount; i++)
  {
    if(manager->textures[i].id != 0)
      UnloadTexture(manager->textures[i]);
  }
  if(manager->textures) free(manager->textures);

  UnloadSpriteNode(manager->sprites);
  UnloadTilesetPackNode(manager->tsPacks);
  UnloadTileMapNode(manager->maps);
  manager->sprites = NULL;
  manager->tsPacks = NULL;
  manager->maps = NULL;
}
