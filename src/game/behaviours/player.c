#include "player.h"

void PlayerOnCreate(Game* game, GameLevel* level, ecs_entity_t player)
{
	PlayerMovement* movement = (PlayerMovement*)malloc(sizeof(PlayerMovement));
	if (movement != NULL)
	{
		movement->inRoll = 0;
		movement->dirX = 0;
		movement->dirY = 0;
		movement->speed = 200;
		movement->vulnerable = 0;
		AddEntityData(level, player, movement);
	}
}

void PlayerOnInput(Game* game, GameLevel* level, ecs_entity_t player)
{
	ECS_COMPONENT(level->world, C_SpriteRender);

	PlayerMovement* movement = (PlayerMovement*)GetEntityData(level, player, 0);
	// hair
	ecs_entity_t hair = GetChildFromIndex(level, player, 0);
	// tools
	ecs_entity_t tools = GetChildFromIndex(level, player, 1);
	// sprite render
	C_SpriteRender* spriteRender = ecs_get(level->world, player, C_SpriteRender);

	if (movement != NULL)
	{
		if (IsKeyDown(KEY_D))
		{
			movement->dirX = 1;
			PlayAnimation(game, level, player, "walking");
			if (hair != 0) 
			{
				C_SpriteRender* hairSr = ecs_get(level->world, hair, C_SpriteRender);
				hairSr->flipX = 0;
				PlayAnimation(game, level, hair, "walking");
			}
			if (tools != 0) 
			{
				C_SpriteRender* toolsSr = ecs_get(level->world, tools, C_SpriteRender);
				toolsSr->flipX = 0;
				PlayAnimation(game, level, tools, "walking");
			}
			spriteRender->flipX = 0;
		}
		else if (IsKeyDown(KEY_A))
		{
			movement->dirX = -1;
			PlayAnimation(game, level, player, "walking");
			if (hair != 0) 
			{
				C_SpriteRender* hairSr = ecs_get(level->world, hair, C_SpriteRender);
				hairSr->flipX = 1;
				PlayAnimation(game, level, hair, "walking");
			}
			if (tools != 0) 
			{
				C_SpriteRender* toolsSr = ecs_get(level->world, tools, C_SpriteRender);
				toolsSr->flipX = 1;
				PlayAnimation(game, level, tools, "walking");
			}
			spriteRender->flipX = 1;
		}
		if (IsKeyDown(KEY_W))
		{
			movement->dirY = -1;
			PlayAnimation(game, level, player, "walking");
			if (hair != 0)
			{
				PlayAnimation(game, level, hair, "walking");
			}
			if (tools != 0)
			{
				PlayAnimation(game, level, tools, "walking");
			}
		}
		else if (IsKeyDown(KEY_S))
		{
			movement->dirY = 1;
			PlayAnimation(game, level, player, "walking");
			if (hair != 0)
			{
				PlayAnimation(game, level, hair, "walking");
			}
			if (tools != 0)
			{
				PlayAnimation(game, level, tools, "walking");
			}
		}

		if (IsKeyReleased(KEY_D))
		{
			movement->dirX = 0;
			PlayAnimation(game, level, player, "idle");
			if (hair != 0) PlayAnimation(game, level, hair, "idle");
			if (tools != 0) PlayAnimation(game, level, tools, "idle");
		}
		if (IsKeyReleased(KEY_A))
		{
			movement->dirX = 0;
			PlayAnimation(game, level, player, "idle");
			if (hair != 0) PlayAnimation(game, level, hair, "idle");
			if (tools != 0) PlayAnimation(game, level, tools, "idle");
		}
		if (IsKeyReleased(KEY_W))
		{
			movement->dirY = 0;
			PlayAnimation(game, level, player, "idle");
			if (hair != 0) PlayAnimation(game, level, hair, "idle");
			if (tools != 0) PlayAnimation(game, level, tools, "idle");
		}
		if (IsKeyReleased(KEY_S))
		{
			movement->dirY = 0;
			PlayAnimation(game, level, player, "idle");
			if (hair != 0) PlayAnimation(game, level, hair, "idle");
			if (tools != 0) PlayAnimation(game, level, tools, "idle");
		}
	}
}

void PlayerOnUpdate(Game* game, GameLevel* level, ecs_entity_t player)
{
	ECS_COMPONENT(level->world, C_Transform);

	PlayerMovement* movement = (PlayerMovement*)GetEntityData(level, player, 0);
	C_Transform* transform = ecs_get(level->world, player, C_Transform);

	if (movement != NULL)
	{
		transform->positionX += movement->speed * movement->dirX * GetFrameTime();
		transform->positionY += movement->speed * movement->dirY * GetFrameTime();
	}
}

void PlayerOnCollision(Game* game, GameLevel* level, ecs_entity_t player, ecs_entity_t other)
{
}

void PlayerOnDestroyDataHandler(Game* game, GameLevel* level, ecs_entity_t player)
{
	PlayerOnUnloadDataHandler(game, level, player);
}

void PlayerOnUnloadDataHandler(Game* game, GameLevel* level, ecs_entity_t player)
{
	PlayerMovement* movement = (PlayerMovement*)GetEntityData(level, player, 0);
	if (movement != NULL)
		free(movement);
}
