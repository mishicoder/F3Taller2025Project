#include "house.h"

void DoorOnCollision(Game* game, GameLevel* level, ecs_entity_t entity, ecs_entity_t other)
{
	PushLevel(game, "farm", 0, 0, 0, FarmOnLoad);
}

void HouseOnLoad(Game* game, GameLevel* level)
{

	ECS_COMPONENT(level->world, C_Transform);
	ECS_COMPONENT(level->world, C_Camera2D);
	ECS_COMPONENT(level->world, C_MapController);
	ECS_COMPONENT(level->world, C_RectCollider);

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
	mapController->showGrid = 0;
	gridT->scaleX = 4.0f;
	gridT->scaleY = 4.0f;
	gridT->positionX = 0.0f;
	gridT->positionY = 0.0f;

	// centrar la camara al mapa
	TileMap* map = GetTileMap(&game->resourcesManager, "player_house");
	cameraTransform->positionX = gridT->positionX + ((map->mapTexture.texture.width * gridT->scaleX) / 2.0f);
	cameraTransform->positionY = gridT->positionY + ((map->mapTexture.texture.height * gridT->scaleY) / 2.0f);

	//****************** muros *******************
	// left
	ecs_entity_t leftWall = CreateBlankEntity(level, "leftwall", "leftwall");
	AddComponentToEntity(game, level, leftWall, C_RECT_COLLIDER_ID, "0.0,0.0,64,448,1,1");
	// top
	ecs_entity_t topWall = CreateBlankEntity(level, "topWall", "topWall");
	AddComponentToEntity(game, level, topWall, C_RECT_COLLIDER_ID, "64.0,0.0,512,64,1,1");
	// right
	ecs_entity_t rightWall = CreateBlankEntity(level, "rightWall", "rightWall");
	AddComponentToEntity(game, level, rightWall, C_RECT_COLLIDER_ID, "576.0, 0.0, 64, 448, 1, 1");
	// down a
	ecs_entity_t downtWalla = CreateBlankEntity(level, "downtWalla", "downtWalla");
	AddComponentToEntity(game, level, downtWalla, C_RECT_COLLIDER_ID, "64.0, 384.0, 192, 64, 1, 1");
	// down b
	ecs_entity_t downtWallb = CreateBlankEntity(level, "downtWallb", "downtWallb");
	AddComponentToEntity(game, level, downtWallb, C_RECT_COLLIDER_ID, "320.0, 384.0, 256, 64, 1, 1");
	// door
	ecs_entity_t door = CreateBlankEntity(level, "door", "door");
	AddComponentToEntity(game, level, door, C_RECT_COLLIDER_ID, "256.0,384.0,64,64,0,1");
	AddEntityBehaviour(game, level, door, NULL, NULL, NULL, NULL, DoorOnCollision, NULL, NULL);

	ecs_entity_t player = CreateBlankEntity(level, "player", "player");
	AddComponentToEntity(game, level, player, C_SPRITE_RENDER_ID, "player,1,1.0,0,0");
	AddComponentToEntity(game, level, player, C_SPRITE_ANIMATION_ID, "player");
	AddComponentToEntity(game, level, player, C_RECT_COLLIDER_ID, "176.0,108.0,40,48,1,0");
	AddEntityBehaviour(game, level, player, PlayerOnCreate, PlayerOnInput, PlayerOnUpdate, NULL, PlayerOnCollision, PlayerOnDestroyDataHandler, PlayerOnUnloadDataHandler);
	C_Transform* pTransform = ecs_get(level->world, player, C_Transform);
	pTransform->scaleX = 4.0f;
	pTransform->scaleY = 4.0f;
	PlayAnimation(game, level, player, "idle");

	ecs_entity_t hair = AddChildToEntity(level, player, "hair", "hair");
	AddComponentToEntity(game, level, hair, C_SPRITE_RENDER_ID, "long_hair,1,1.0,0,0");
	AddComponentToEntity(game, level, hair, C_SPRITE_ANIMATION_ID, "long_hair");
	C_Transform* hairT = ecs_get(level->world, hair, C_Transform);
	hairT->scaleX = 4.0f;
	hairT->scaleY = 4.0f;
	PlayAnimation(game, level, hair, "idle");

	ecs_entity_t tools = AddChildToEntity(level, player, "tools", "tools");
	AddComponentToEntity(game, level, tools, C_SPRITE_RENDER_ID, "tools,1,1.0,0,0");
	AddComponentToEntity(game, level, tools, C_SPRITE_ANIMATION_ID, "tools");
	C_Transform* toolsT = ecs_get(level->world, tools, C_Transform);
	toolsT->scaleX = 4.0f;
	toolsT->scaleY = 4.0f;
	PlayAnimation(game, level, tools, "idle");

}
