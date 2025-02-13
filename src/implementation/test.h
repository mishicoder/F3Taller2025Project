#pragma once
#ifndef TEST_H
#define TEST_H

#include "../game/behaviours/daycycle.h"

void TestOnChangeDay(Game* game, GameLevel* level, ecs_entity_t entity);
void TestOnCreate(Game* game, GameLevel* level, ecs_entity_t entity);

#endif