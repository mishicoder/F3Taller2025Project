
#ifndef TEST_LEVEL_H
#define TEST_ELVEL_H

#include <stdio.h>
#include <flecs.h>
#include "../core/game.h"

void OnPlayerInput(Game* game, GameLevel* level, ecs_entity_t entity);
void OnPlayerUpdate(Game* game, GameLevel* level, ecs_entity_t entity);

void TL_Run(Game* game, GameLevel* level);

#endif // !TEST_LEVEL_H

