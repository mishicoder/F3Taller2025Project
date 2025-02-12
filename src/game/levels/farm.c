#include "farm.h"

void FarmOnLoad(Game* game, GameLevel* level)
{
	ecs_entity_t daycycle = CreateBlankEntity(level, "daycycle", "daycycle");
	AddEntityBehaviour(game, level, daycycle, DayCycleOnCreate, NULL, DayCycleOnUpdate, NULL, NULL, DayCycleOnDestroyDataHandler, DayCycleOnUnloadDataHandler);

	ecs_entity_t mainCamra = CreateBlankEntity(level, "main camera", "main camera");
	AddComponentToEntity(game, level, mainCamra, C_CAMERA_2D_ID, "1,0.0,0.0,0.0,0.0,0.0,1.0");
}
