#include "farm.h"

void FarmOnLoad(Game* game, GameLevel* level)
{
	ECS_COMPONENT(level->world, C_Transform);
	ECS_COMPONENT(level->world, C_Camera2D);
	ECS_COMPONENT(level->world, C_MapController);

	ecs_entity_t daycycle = CreateBlankEntity(level, "daycycle", "daycycle");
	AddEntityBehaviour(game, level, daycycle, DayCycleOnCreate, NULL, DayCycleOnUpdate, NULL, NULL, DayCycleOnDestroyDataHandler, DayCycleOnUnloadDataHandler);

	ecs_entity_t mainCamera = CreateBlankEntity(level, "main camera", "main camera");
	AddComponentToEntity(game, level, mainCamera, C_CAMERA_2D_ID, "1,0.0,0.0,0.0,0.0,0.0,1.0");
	C_Transform* cameraTransform = ecs_get(level->world, mainCamera, C_Transform);
	C_Camera2D* cameraComp = ecs_get(level->world, mainCamera, C_Camera2D);
	cameraComp->offsetX = GetRenderWidth() / 2.0f;
	cameraComp->offsetY = GetRenderHeight() / 2.0f;

	ecs_entity_t grid = CreateBlankEntity(level, "grid", "grid");
	AddComponentToEntity(game, level, grid, C_MAP_RENDER_ID, "player_house");
	AddComponentToEntity(game, level, grid, C_MAP_CONTROLLER_ID, "player_house");
	C_Transform* gridT = ecs_get(level->world, grid, C_Transform);
	C_MapController* mapController = ecs_get(level->world, grid, C_MapController);
	mapController->showGrid = 1;
	gridT->scaleX = 4.0f;
	gridT->scaleY = 4.0f;
	gridT->positionX = 0.0f;
	gridT->positionY = 0.0f;

	// centrar la camara al mapa
	TileMap* map = GetTileMap(&game->resourcesManager, "player_house");
	cameraTransform->positionX = gridT->positionX + ((map->mapTexture.texture.width * gridT->scaleX) / 2.0f);
	cameraTransform->positionY = gridT->positionY + ((map->mapTexture.texture.height * gridT->scaleY) / 2.0f);

	ecs_entity_t player = CreateBlankEntity(level, "player", "player");
	AddComponentToEntity(game, level, player, C_SPRITE_RENDER_ID, "player,1,1.0,0,0");
	AddComponentToEntity(game, level, player, C_SPRITE_ANIMATION_ID, "player");
	C_Transform* pTransform = ecs_get(level->world, player, C_Transform);
	pTransform->scaleX = 4.0f;
	pTransform->scaleY = 4.0f;
	PlayAnimation(game, level, player, "idle");

	ecs_entity_t tools = AddChildToEntity(level, player, "tools", "tools");
	AddComponentToEntity(game, level, tools, C_SPRITE_RENDER_ID, "tools,1,1.0,0,0");
	AddComponentToEntity(game, level, tools, C_SPRITE_ANIMATION_ID, "tools");
	C_Transform* toolsT = ecs_get(level->world, tools, C_Transform);
	toolsT->scaleX = 4.0f;
	toolsT->scaleY = 4.0f;
	PlayAnimation(game, level, tools, "idle");

	/*
	PFNode* path = FindPath((Vector2) { 0, 0 }, (Vector2){4, 6}, mapController);
	if (path != NULL)
	{
		printf("CAMINO ENCONTRADO\n");
	}
	free(path);
	*/
}
