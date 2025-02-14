#pragma once
#ifndef PLAYER_H
#define PLAYER_H

#include "../../core/game.h"

typedef struct PlayerMovement 
{
	int dirX;
	int dirY;
	int speed;
	int inRoll;
	int vulnerable;
} PlayerMovement;

void PlayerOnCreate(Game* game, GameLevel* level, ecs_entity_t player);
void PlayerOnInput(Game* game, GameLevel* level, ecs_entity_t player);
void PlayerOnUpdate(Game* game, GameLevel* level, ecs_entity_t player);
void PlayerOnCollision(Game* game, GameLevel* level, ecs_entity_t player, ecs_entity_t other);

void PlayerOnDestroyDataHandler(Game* game, GameLevel* level, ecs_entity_t player);
void PlayerOnUnloadDataHandler(Game* game, GameLevel* level, ecs_entity_t player);

#endif // !PLAYER_H
