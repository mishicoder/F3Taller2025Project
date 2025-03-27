#include "sprite.h"

ResSprite* CreateSprite(const char *name, int textureIndex, int x, int y, int width, int height, Vector2 origin, int framesCount)
{
  ResSprite* sprite = (ResSprite*)malloc(sizeof(ResSprite));
  if(!sprite) return NULL;

  sprite->name = _strdup(name);
  sprite->textureIndex = textureIndex;
  sprite->x = x;
  sprite->y = y;
  sprite->width = width;
  sprite->height = height;
  sprite->origin = origin;
  sprite->framesCount = framesCount;
  sprite->nextFrame = 0;
  sprite->frames = NULL;
  sprite->animCount = 0;
  sprite->animations = NULL;

  if(sprite->framesCount > 0)
  {
    sprite->frames = (ResFrame**)malloc(sprite->framesCount * sizeof(ResFrame*));
    if(!sprite->frames)
    {
      free(sprite->name);
      free(sprite);
      return NULL;
    }
  }
 
  return sprite;
}

bool AddFrameToSprite(ResSprite *sprite, int x, int y, int width, int height)
{
  if(!sprite) return false;
  if(sprite->framesCount <= 0 || sprite->nextFrame >= sprite->framesCount) return false;

  ResFrame* frame = (ResFrame*)malloc(sizeof(ResFrame));
  if(!frame) return false;

  frame->x = x;
  frame->y = y;
  frame->width = width;
  frame->height = height;

  sprite->frames[sprite->nextFrame] = frame;
  sprite->nextFrame += 1;

  return true;
}

bool AddAnimationToSprite(ResSprite *sprite, const char *name, int fIndex, int tIndex, bool loop, int speed)
{
  if(!sprite) return false;
  if(sprite->framesCount <= 0) { printf("No se han encontrado los frames\n"); return false;}
  if(!(fIndex >= 0 && fIndex < sprite->framesCount && tIndex >= 0 && tIndex < sprite->framesCount)) { printf("ERROR EN LOS FRAMES\n"); return false;}

  ResAnimation* animation = (ResAnimation*)malloc(sizeof(ResAnimation));
  if(!animation) { printf("Error al asignar memoria para la animacion\n"); return false;}

  animation->name = _strdup(name);
  if(!animation->name){ printf("Error al asignar nombre para la animacion\n"); free(animation); return false; }
  animation->fIndex = fIndex;
  animation->tIndex = tIndex;
  animation->loop = loop;
  animation->speed = speed;

  ResAnimation** memTemp = (ResAnimation**)realloc(sprite->animations, (size_t)(sprite->animCount + 1) * sizeof(ResAnimation*));
  if(!memTemp){ printf("Error al reasignar memoria para la animacion\n"); free(animation->name); free(animation); return false; }

  sprite->animations = memTemp;
  sprite->animations[sprite->animCount] = animation;
  sprite->animCount += 1;

  return true;
}

SpriteNode* InsertSpriteNode(SpriteNode *node, ResSprite *sprite)
{
  if(!node)
  {
    SpriteNode* newNode = (SpriteNode*)malloc(sizeof(SpriteNode));
    if(!newNode){ return NULL; }
    newNode->sprite = sprite;
    newNode->left = NULL;
    newNode->right = NULL;
    return newNode;
  }

  int result = strcmp(sprite->name, node->sprite->name);
  if(result < 0)
    node->left = InsertSpriteNode(node->left, sprite);
  else if(result > 0)
    node->right = InsertSpriteNode(node->right, sprite);

  return node;
}

ResSprite* SearchSpriteInTree(SpriteNode *node, const char *name)
{
  if(node == NULL) return NULL;

  int result = strcmp(name, node->sprite->name);
  if(result == 0)
    return node->sprite;
  else if(result < 0)
    return SearchSpriteInTree(node->left, name);
  else
    return SearchSpriteInTree(node->right, name);

  return NULL;
}

void UnloadSpriteNode(SpriteNode *node)
{
  if (!node) return;

  UnloadSpriteNode(node->left);
  UnloadSpriteNode(node->right);

  // eliminar frames
  if(node->sprite->framesCount > 0)
  {
    for(int i = 0; i < node->sprite->framesCount; i++)
    {
      free(node->sprite->frames[i]);
    }
    free(node->sprite->frames);
  }
  // eliminar animaciones
  if(node->sprite->animCount > 0)
  {
    for(int i = 0; i < node->sprite->animCount; i++)
    {
      free(node->sprite->animations[i]->name);
      node->sprite->animations[i]->name = NULL;
      free(node->sprite->animations[i]);
    }
    free(node->sprite->animations);
  }
  free(node->sprite->name);
  free(node->sprite);
  free(node);
}
