#include "daycycle.h"

void ResetTimeOnNewDay(Game* game, GameLevel* level, ecs_entity_t entity)
{
	TimeController* controller = (TimeController*)GetEntityData(level, entity, 0);
	if (controller != NULL)
	{
		controller->currentHour = controller->initialDayHour;
		controller->currentMinute = controller->initialDayMinute;
		controller->totalTime = 360;
	}
}

void DayCycleOnCreate(Game* game, GameLevel* level, ecs_entity_t entity)
{
	TimeController* controller = (TimeController*)malloc(sizeof(TimeController));
	if (controller != NULL)
	{
		controller->currentHour = 6;
		controller->currentMinute = 0;
		controller->currentTime = 0.0f;
		controller->initialDayHour = 6;
		controller->initialDayMinute = 0;
		controller->totalTime = 360;
		controller->dayChangeCallbacks = NULL;
		controller->dayChangeCallbacksCount = 0;

		// agregar eventos por defecto
		OnDayChangeCallback* memTemp = (OnDayChangeCallback*)realloc(controller->dayChangeCallbacks, (size_t)(1) * sizeof(OnDayChangeCallback));
		if (memTemp != NULL)
		{
			controller->dayChangeCallbacks = memTemp;
			controller->dayChangeCallbacks[0] = ResetTimeOnNewDay;
			controller->dayChangeCallbacksCount = 1;
		}

		AddEntityData(level, entity, controller);
	}
}

void DayCycleOnUpdate(Game* game, GameLevel* level, ecs_entity_t entity)
{
	TimeController* controller = (TimeController*)GetEntityData(level, entity, 0);
	if (controller != NULL)
	{
		// actualizar los datos del tiempo y llamar a las funciones (OnDayChange registrados)
		controller->currentTime += GetFrameTime();
		if (controller->currentTime >= 0.625f) 
		{
			controller->totalTime += 1;
			controller->currentTime = 0.0f;
			controller->currentMinute += 1;
			if (controller->currentMinute >= 60)
			{
				controller->currentHour += 1;
				controller->currentMinute = 0;
				if (controller->currentHour == 5)
				{
					// lanzar eventos
					for(int i = 0; i < controller->dayChangeCallbacksCount; i++)
					{
						controller->dayChangeCallbacks[i](game, level, entity);
					}
				}
				else if (controller->currentHour == 24)
				{
					controller->currentHour = 0;
				}
			}
		}
	}
}

void DayCycleOnDestroyDataHandler(Game* game, GameLevel* level, ecs_entity_t entity)
{
	DayCycleOnUnloadDataHandler(game, level, entity);
}

void DayCycleOnUnloadDataHandler(Game* game, GameLevel* level, ecs_entity_t entity)
{
	TimeController* controller = GetEntityData(level, entity, 0);
	if (controller != NULL)
	{
		if (controller->dayChangeCallbacks != NULL)
		{
			free(controller->dayChangeCallbacks);
		}
		free(controller);
		printf("DATOS ELIMINADOS\n");
	}
}

void SetDayTime(Game* game, GameLevel* level, int hour, int minute)
{
	int nextHour = hour < 0 || hour > 24 ? 0 : hour;
	int nextMinute = minute < 0 || hour > 60 ? 0 : minute;

	ecs_entity_t entity = ecs_lookup(level->world, "daycycle");
	if (entity != 0)
	{
		TimeController* controller = (TimeController*)GetEntityData(level, entity, 0);
		if (controller != NULL)
		{
			controller->currentHour = nextHour;
			controller->currentMinute = nextMinute;
			controller->currentTime = 0.0f;
			controller->totalTime = (nextHour * 60) + nextMinute;
		}
	}
}

void RegisterOnDayChangeEvent(Game* game, GameLevel* level, OnDayChangeCallback callback)
{
	ecs_entity_t entity = ecs_lookup(level->world, "daycycle");
	if(entity != 0)
	{
		TimeController* controller = (TimeController*)GetEntityData(level, entity, 0);
		if (controller != NULL)
		{
			OnDayChangeCallback* memTemp = (OnDayChangeCallback*)realloc(controller->dayChangeCallbacks, (size_t)(controller->dayChangeCallbacksCount + 1) * sizeof(OnDayChangeCallback));
			if (memTemp != NULL)
			{
				controller->dayChangeCallbacks = memTemp;
				controller->dayChangeCallbacks[controller->dayChangeCallbacksCount] = callback;
				controller->dayChangeCallbacksCount += 1;
			}
		}
	}
}
