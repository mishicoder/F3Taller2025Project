#pragma once
#ifndef DAYCYCLE_H
#define DAYCYCLE_H

#include "../../core/game.h"

typedef void (*OnDayChangeCallback)(Game* game, GameLevel* level, ecs_entity_t entity);

typedef struct TimeController
{
	float currentTime;
	int currentHour;
	int currentMinute;
	int initialDayHour;
	int initialDayMinute;
	int totalTime;

	// eventos que se dan al cambiar de día
	OnDayChangeCallback* dayChangeCallbacks;
	int dayChangeCallbacksCount;
}TimeController;

void ResetTimeOnNewDay(Game* game, GameLevel* level, ecs_entity_t entity);

void DayCycleOnCreate(Game* game, GameLevel* level, ecs_entity_t entity);
void DayCycleOnUpdate(Game* game, GameLevel* level, ecs_entity_t entity);
void DayCycleOnDestroyDataHandler(struct Game* game, struct GameLevel* level, ecs_entity_t entity);
void DayCycleOnUnloadDataHandler(struct Game* game, struct GameLevel* level, ecs_entity_t entity);

void SetDayTime(Game* game, GameLevel* level, int hour, int minute);
void RegisterOnDayChangeEvent(Game* game, GameLevel* level, OnDayChangeCallback callback);

#endif // !DAYCYCLE_H
