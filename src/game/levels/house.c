#include "house.h"

void DoorOnCollision(Game* game, GameLevel* level, ecs_entity_t entity, ecs_entity_t other)
{
	//if (PushLevel(game, "farm", 0, 0, 0, FarmOnLoad) == 0)
	//{
	//	PushMemoryLevel(game, "farm");
	//}
	//LoadLevel(game, "farm", 0, 0, 0, FarmOnLoad);
	//printf("CANTIDAD DE NIVELES: %d\n", game->levelStackCount);
	PushLevel(game, "farm", 1, 0, 0, FarmOnLoad);
}


void GiftOnCollision(Game* game, GameLevel* level, ecs_entity_t ent, ecs_entity_t other)
{
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
	gridT->positionX = 0.0f;
	gridT->positionY = 0.0f;

	// centrar la camara al mapa
	TileMap* map = GetTileMap(&game->resourcesManager, "player_house");
	cameraTransform->positionX = gridT->positionX + ((map->mapTexture.texture.width * gridT->scaleX * game->config.globalScale) / 2.0f);
	cameraTransform->positionY = gridT->positionY + ((map->mapTexture.texture.height * gridT->scaleY * game->config.globalScale) / 2.0f);

	// gift
	ecs_entity_t gift = CreateBlankEntity(level, "gift", "gift");
	// 4,5
	AddComponentToEntity(game, level, gift, C_SPRITE_RENDER_ID, "giftanim,1,1.0,0,0");
	AddComponentToEntity(game, level, gift, C_SPRITE_ANIMATION_ID, "giftanim");
	AddComponentToEntity(game, level, gift, C_RECT_COLLIDER_ID, "0,0,64,64,0,1");
	AddEntityBehaviour(game, level, gift, NULL, NULL, NULL, NULL, GiftOnCollision, NULL, NULL);
	PlayAnimation(game, level, gift, "idle");
	C_Transform* gt = ecs_get(level->world, gift, C_Transform);
	//gt->scaleX = 4.0f;
	//gt->scaleY = 4.0f;
	gt->positionX = 256;
	gt->positionY = 320;

	//****************** muros *******************
	// left
	ecs_entity_t leftWall = CreateBlankEntity(level, "leftwall", "leftwall");
	AddComponentToEntity(game, level, leftWall, C_RECT_COLLIDER_ID, "0.0,0.0,64,448,1,1");
	// top
	ecs_entity_t topWall = CreateBlankEntity(level, "topWall", "topWall");
	AddComponentToEntity(game, level, topWall, C_RECT_COLLIDER_ID, "16.0,0.0,512,64,1,1");
	// right
	ecs_entity_t rightWall = CreateBlankEntity(level, "rightWall", "rightWall");
	AddComponentToEntity(game, level, rightWall, C_RECT_COLLIDER_ID, "144.0, 0.0, 64, 448, 1, 1");
	// down a
	ecs_entity_t downtWalla = CreateBlankEntity(level, "downtWalla", "downtWalla");
	AddComponentToEntity(game, level, downtWalla, C_RECT_COLLIDER_ID, "16.0, 96.0, 192, 64, 1, 1");
	// down b
	ecs_entity_t downtWallb = CreateBlankEntity(level, "downtWallb", "downtWallb");
	AddComponentToEntity(game, level, downtWallb, C_RECT_COLLIDER_ID, "80.0, 96.0, 256, 64, 1, 1");
	// 
	ecs_entity_t table = CreateBlankEntity(level, "table", "table");
	AddComponentToEntity(game, level, table, C_RECT_COLLIDER_ID, "62,16,144,48,1,1");
	// door
	ecs_entity_t door = CreateBlankEntity(level, "door", "door");
	AddComponentToEntity(game, level, door, C_RECT_COLLIDER_ID, "64.0,96.0,64,64,0,1");
	AddEntityBehaviour(game, level, door, NULL, NULL, NULL, NULL, DoorOnCollision, NULL, NULL);
	// cahir a
	ecs_entity_t chaira = CreateBlankEntity(level, "chaira", "chaira");
	AddComponentToEntity(game, level, chaira, C_RECT_COLLIDER_ID, "64,42,40,18,1,1");

	ecs_entity_t chairb = CreateBlankEntity(level, "chairb", "chairb");
	AddComponentToEntity(game, level, chairb, C_RECT_COLLIDER_ID, "80,42,38,18,1,1");

	ecs_entity_t player = CreateBlankEntity(level, "player", "player");
	AddComponentToEntity(game, level, player, C_SPRITE_RENDER_ID, "player,1,1.0,0,0");
	AddComponentToEntity(game, level, player, C_SPRITE_ANIMATION_ID, "player");
	AddComponentToEntity(game, level, player, C_RECT_COLLIDER_ID, "44.0,27.0,40,48,1,0");
	AddEntityBehaviour(game, level, player, PlayerOnCreate, PlayerOnInput, PlayerOnUpdate, NULL, PlayerOnCollision, PlayerOnDestroyDataHandler, PlayerOnUnloadDataHandler);
	C_Transform* pTransform = ecs_get(level->world, player, C_Transform);
	pTransform->scaleX = 1.0f;
	pTransform->scaleY = 1.0f;
	PlayAnimation(game, level, player, "idle");

	ecs_entity_t hair = AddChildToEntity(level, player, "hair", "hair");
	AddComponentToEntity(game, level, hair, C_SPRITE_RENDER_ID, "long_hair,1,1.0,0,0");
	AddComponentToEntity(game, level, hair, C_SPRITE_ANIMATION_ID, "long_hair");
	C_Transform* hairT = ecs_get(level->world, hair, C_Transform);
	//hairT->scaleX = 4.0f;
	//hairT->scaleY = 4.0f;
	PlayAnimation(game, level, hair, "idle");

	ecs_entity_t tools = AddChildToEntity(level, player, "tools", "tools");
	AddComponentToEntity(game, level, tools, C_SPRITE_RENDER_ID, "tools,1,1.0,0,0");
	AddComponentToEntity(game, level, tools, C_SPRITE_ANIMATION_ID, "tools");
	C_Transform* toolsT = ecs_get(level->world, tools, C_Transform);
	//toolsT->scaleX = 4.0f;
	//toolsT->scaleY = 4.0f;
	PlayAnimation(game, level, tools, "idle");

	// load UI
}
