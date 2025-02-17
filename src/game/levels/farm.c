#include "farm.h"

void testing(Game* game, GameLevel* level, ecs_entity_t ent)
{
	if (IsKeyReleased(KEY_A))
	{
		//PopLevel(game);
		printf("TESTING\n");
	}
}

void FarmOnLoad(Game* game, GameLevel* level)
{
	ecs_entity_t mainCamera = CreateBlankEntity(level, "camera", "camera");
	AddComponentToEntity(game, level, mainCamera, C_CAMERA_2D_ID, "1,0,0,0,0,0,1.0");
	AddEntityBehaviour(game, level, mainCamera, NULL, testing, NULL, NULL, NULL, NULL, NULL);
}
      