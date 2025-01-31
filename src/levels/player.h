#pragma once
#include "../core/game.h"

void OnPlayerInput(Game* game, GameLevel* level, ecs_entity_t entity);
void OnPlayerUpdate(Game* game, GameLevel* level, ecs_entity_t entity);