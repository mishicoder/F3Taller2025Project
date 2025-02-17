#pragma once
#ifndef HOUSE_H
#define HOUSE_H

#include "../../core/game.h"
#include "../behaviours/daycycle.h"
#include "../behaviours/player.h"
#include "../levels/farm.h"

void DoorOnCollision(Game* game, GameLevel* level, ecs_entity_t entity, ecs_entity_t other);
void GiftOnCollision(Game* game, GameLevel* level, ecs_entity_t ent, ecs_entity_t other);
// funcion que se ejecuta cuando se crea el nivel
void HouseOnLoad(Game* game, GameLevel* level);

#endif // !HOUSE_H