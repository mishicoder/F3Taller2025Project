#include "sprite.h"

Sprite* CreateSprite(const char* name, unsigned int textureIndex, unsigned int x, unsigned int y, unsigned int width, unsigned int height, Vector2 origin, unsigned int framesCount)
{
	Sprite* sprite = (Sprite*)malloc(sizeof(Sprite));
	if (!sprite)
	{
		printf("Error al asignar memoria para el sprite \'%s\'.\n", name);
		return NULL;
	}

	sprite->name = name;
	sprite->textureIndex = textureIndex;
	sprite->x = x;
	sprite->y = y;
	sprite->width = width;
	sprite->height = height;
	sprite->origin = origin;
	sprite->framesCount = framesCount;
	sprite->animationsCount = 0;
	sprite->frames = NULL;
	sprite->nextFrame = 0;
	sprite->animations = NULL;

	if (sprite->framesCount > 0)
	{
		sprite->frames = (SpriteFrame**)malloc(sprite->framesCount * sizeof(SpriteFrame*));
		if (!sprite->frames)
		{
			free(sprite->name);
			free(sprite);
			printf("Error al asignar memoria para los frames del sprite \'%s\'.", name);
			return NULL;
		}
	}

	return sprite;
}

int AddSpriteFrame(Sprite* sprite, unsigned int x, unsigned int y, unsigned int width, unsigned int height)
{
	if (sprite == NULL) return 0;

	if (sprite->framesCount <= 0) return 0;
	if (sprite->nextFrame >= sprite->framesCount) return 0;

	SpriteFrame* frame = (SpriteFrame*)malloc(sizeof(SpriteFrame));
	if (frame == NULL) return 0;

	frame->x = x;
	frame->y = y;
	frame->width = width;
	frame->height = height;

	sprite->frames[sprite->nextFrame] = frame;
	sprite->nextFrame += 1; // hacer al compilador feliz :D

	return 1;
}

int AddSpriteAnimation(Sprite* sprite, const char* name, unsigned int fromIndex, unsigned int toIndex, unsigned int loop, unsigned int speed)
{
	if (sprite == NULL) return 0;
	if (sprite->framesCount <= 0) return 0;
	if (!(fromIndex >= 0 && fromIndex < sprite->framesCount && toIndex >= 0 && toIndex < sprite->framesCount)) return 0;

	SpriteAnimation* animation = (SpriteAnimation*)malloc(sizeof(SpriteAnimation));
	if (animation == NULL) return 0;

	animation->name = strdup(name);
	if (!animation->name)
	{
		printf("Error al cargar la animación\n");
		free(animation);
		return 0;
	}

	animation->fromIndex = fromIndex;
	animation->toIndex = toIndex;
	animation->loop = loop;
	animation->speed = speed;

	sprite->animationsCount += 1;
	SpriteAnimation** memTemp = (SpriteAnimation**)realloc(sprite->animations, sprite->animationsCount * sizeof(SpriteAnimation));
	if (memTemp == NULL)
	{
		sprite->animationsCount -= 1;
		free(animation->name);
		free(animation);
		return 0;
	}

	sprite->animations = memTemp;
	sprite->animations[sprite->animationsCount - 1] = animation;
	return 1;
}

SpriteNode* InsertSpriteNode(SpriteNode* node, Sprite* sprite)
{
	if (node == NULL)
	{
		SpriteNode* newNode = (SpriteNode*)malloc(sizeof(SpriteNode));
		if (!newNode)
		{
			return NULL;
		}

		newNode->sprite = sprite;
		newNode->left = NULL;
		newNode->right = NULL;
		return newNode;
	}

	int cmp = strcmp(sprite->name, node->sprite->name);
	if (cmp < 0)
		node->left = InsertSpriteNode(node->left, sprite);
	else if (cmp > 0)
		node->right = InsertSpriteNode(node->right, sprite);
	else
		printf("El sprite %s ya existe.\n", sprite->name);

	return node;
}

Sprite* TreeSpriteSearch(SpriteNode* node, const char* name)
{
	if (node == NULL) return NULL;

	int cmp = strcmp(name, node->sprite->name);
	if (cmp == 0)
		return node->sprite;
	else if (cmp < 0)
		return TreeSpriteSearch(node->left, name);
	else
		return TreeSpriteSearch(node->right, name);

	return NULL;
}

void UnloadTreeSpriteNode(SpriteNode* node)
{
	if (node == NULL) 
	{
		printf("El nodo es nulo, imposible eliminar\n");
		return;
	}

	// liberar sub-arboles
	UnloadTreeSpriteNode(node->left);
	UnloadTreeSpriteNode(node->right);
	
	// eliminar los frames
	if (node->sprite->framesCount > 0)
	{
		for (int i = 0; i < node->sprite->framesCount; i++)
		{
			free(node->sprite->frames[i]);
		}
		free(node->sprite->frames);
	}
	// eliminar las animaciones
	if (node->sprite->animationsCount > 0)
	{
		for (int i = 0; i < node->sprite->animationsCount; i++)
		{
			free(node->sprite->animations[i]->name);
			node->sprite->animations[i]->name = NULL;
			free(node->sprite->animations[i]);
		}
		free(node->sprite->animations);
	}
	free(node->sprite);
	free(node);
}
