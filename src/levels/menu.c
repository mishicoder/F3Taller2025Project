#include "menu.h"

void OnInput(Game* game, GameLevel* level, ecs_entity_t entity)
{
	ECS_COMPONENT(level->world, C_Transform);
	C_Transform* transform = ecs_get(level->world, entity, C_Transform);

	if (IsKeyDown(KEY_D))
		transform->positionX += 200 * GetFrameTime();
	if (IsKeyDown(KEY_A))
		transform->positionX -= 200 * GetFrameTime();
	if (IsKeyDown(KEY_W))
		transform->positionY -= 200 * GetFrameTime();
	if (IsKeyDown(KEY_S))
		transform->positionY += 200 * GetFrameTime();
}

void OnUpdateCamera(Game* game, GameLevel* level, ecs_entity_t entity)
{
	ECS_COMPONENT(level->world, C_Camera2D);
	ECS_COMPONENT(level->world, C_MapRender);
	ECS_COMPONENT(level->world, C_SpriteRender);
	ECS_COMPONENT(level->world, C_SpriteAnimation);
	ECS_COMPONENT(level->world, C_Transform);

	ecs_entity_t icon = ecs_lookup(level->world, "icon");
	C_Transform* iconTransform = ecs_get(level->world, icon, C_Transform);
	iconTransform->scaleX = 4.0f;
	iconTransform->scaleY = 4.0f;
	C_SpriteRender* iconSprite = ecs_get(level->world, icon, C_SpriteRender);
	Sprite* sprite = GetSprite(&game->resourcesManager, iconSprite->spriteName);
	if (ecs_has(level->world, icon, C_SpriteAnimation))
	{
	}

	C_Transform* cameraTransform = ecs_get(level->world, entity, C_Transform);
	C_Camera2D* camera = ecs_get(level->world, entity, C_Camera2D);

	if (sprite != NULL)
	{
		camera->offsetX = GetRenderWidth() / 2.0f;
		camera->offsetY = GetRenderHeight() / 2.0f;
		cameraTransform->positionX = iconTransform->positionX + ((sprite->width * iconTransform->scaleX) / 2.0f);
		cameraTransform->positionY = iconTransform->positionY + ((sprite->height * iconTransform->scaleY) / 2.0f);
	}

	DrawText(TextFormat("x: %02.02f\ny: %02.02f", iconTransform->positionX, iconTransform->positionY), 10, 10, 20, WHITE);
}

void MenuOnLoad(Game* game, GameLevel* level)
{
	ECS_COMPONENT(level->world, C_Camera2D);
	ECS_COMPONENT(level->world, C_Behaviour);
	ECS_COMPONENT(level->world, C_MapRender);
	ECS_COMPONENT(level->world, C_Transform);
	ECS_COMPONENT(level->world, C_SpriteAnimation);

	ecs_entity_t mainCamera = CreateBlankEntity(level, "camera", "main_camera");
	AddComponentToEntity(game, level, mainCamera, C_CAMERA_2D_ID, "1,0.0,0.0,0.0,0.0,0.0,1.0");
	AddEntityBehaviour(game, level, mainCamera, NULL, NULL, OnUpdateCamera, NULL, NULL);

	if (level->isComponentsAdded == 0)
	{

		ecs_entity_t iconSpt = CreateBlankEntity(level, "icon", "icon");
		AddComponentToEntity(game, level, iconSpt, C_SPRITE_RENDER_ID, "player,1,1.0");
		AddComponentToEntity(game, level, iconSpt, "animation", "player");
		AddEntityBehaviour(game, level, iconSpt, NULL, OnInput, NULL, NULL, NULL);
		C_Transform* itransform = ecs_get(level->world, iconSpt, C_Transform);
		itransform->rotation = 0;

		ecs_entity_t playerHouse = CreateBlankEntity(level, "house", "player_house");
		AddComponentToEntity(game, level, playerHouse, C_MAP_RENDER_ID, "player_house");
		C_Transform* phTransform = ecs_get(level->world, playerHouse, C_Transform);
		phTransform->scaleX = 4.0f;
		phTransform->scaleY = 4.0f;

		

		level->isComponentsAdded = 1;
	}
}
