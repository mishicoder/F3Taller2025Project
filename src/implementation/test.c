#include "test.h"

void TestOnChangeDay(Game* game, GameLevel* level, ecs_entity_t entity)
{
}

void TestOnCreate(Game* game, GameLevel* level, ecs_entity_t entity)
{
	RegisterOnDayChangeEvent(game, level, TestOnChangeDay);
}


