#include "player.h"

float scale = 1.0f;

void OnPlayerInput(Game* game, GameLevel* level, ecs_entity_t entity)
{
	ECS_COMPONENT(level->world, C_Movement);
	ECS_COMPONENT(level->world, C_Color);

	C_Movement* movement = ecs_get(level->world, entity, C_Movement);
	if (movement == 0) return;

	if (IsKeyPressed(KEY_C))
	{
		if (!ecs_has(level->world, entity, C_Color))
		{
			ecs_add(level->world, entity, C_Color);
			ecs_set(level->world, entity, C_Color, { 33, 33, 33 });
		}
	}

	if (IsKeyDown(KEY_D) || IsKeyDown(KEY_RIGHT))
		movement->directionX = 1;
	if (IsKeyDown(KEY_A))
		movement->directionX = -1;
	if (IsKeyDown(KEY_W))
		movement->directionY = -1;
	if (IsKeyDown(KEY_S))
		movement->directionY = 1;

	if (IsKeyReleased(KEY_D) || IsKeyReleased(KEY_RIGHT))
		movement->directionX = 0;
	if (IsKeyReleased(KEY_A))
		movement->directionX = 0;
	if (IsKeyReleased(KEY_W))
		movement->directionY = 0;
	if (IsKeyReleased(KEY_S))
		movement->directionY = 0;
}

void OnPlayerUpdate(Game* game, GameLevel* level, ecs_entity_t entity)
{
	ECS_COMPONENT(level->world, C_Transform);
	ECS_COMPONENT(level->world, C_Movement);

	C_Movement* movement = ecs_get(level->world, entity, C_Movement);
	C_Transform* transform = ecs_get(level->world, entity, C_Transform);
	if (movement == 0) return;

	scale += 1.5f * GetFrameTime();
	transform->scaleX = scale;
	transform->scaleY = scale;

	if (scale >= 3.0f)
		scale = 0.0f;

	transform->rotation += 20.0f * GetFrameTime();
	transform->positionX += (movement->speed * movement->directionX) * GetFrameTime();
	transform->positionY += (movement->speed * movement->directionY) * GetFrameTime();
}