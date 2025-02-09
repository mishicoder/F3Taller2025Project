#include "menu.h"

void OnCreate(Game* game, GameLevel* level, ecs_entity_t entity)
{
	PlayAnimation(game, level, entity, "idle");
}

int currentAnim = 0;
int isRunning = 0;
int inRoll = 0;

void OnPlayerEndAnim(Game* game, GameLevel* level, ecs_entity_t entity, const char* name)
{
	if (strcmp(name, "roll") == 0)
	{
		inRoll = 0;
		PlayAnimation(game, level, entity, "idle");
	}
}

void OnInput(Game* game, GameLevel* level, ecs_entity_t entity)
{
	ECS_COMPONENT(level->world, C_Transform);
	ECS_COMPONENT(level->world, C_SpriteRender);
	C_Transform* transform = ecs_get(level->world, entity, C_Transform);
	C_SpriteRender* spRender = ecs_get(level->world, entity, C_SpriteRender);

	char* anims[] = { 
		"attack", // 1
		"axe", // 2
		"carry", // 3
		"casting", // 4
		"caught", // 5
		"death", // 6
		"dig", // 7
		"doing", // 8
		"hurt", // 9
		"idle", // 10
		"jump", // 11
		"mining", // 12
		"reeling", // 13
		"roll", // 14
		"run", // 15
		"swimming", // 16
		"waiting", // 17
		"walking", // 18
		"watering" // 19
	};

	ecs_entity_t hair = GetChildFromIndex(level, entity, 0);
	ecs_entity_t child = GetChildFromIndex(level, entity, 1);

	if (IsKeyDown(KEY_LEFT_SHIFT))
	{
		isRunning = 1;
	}
	if (IsKeyReleased(KEY_LEFT_SHIFT))
	{
		isRunning = 0;
	}

	if (IsKeyReleased(KEY_RIGHT)) {
		currentAnim++;

		if (currentAnim >= 19)
			currentAnim = 0;

		PlayAnimation(game, level, entity, anims[currentAnim]);
		if (child != 0)
			PlayAnimation(game, level, child, anims[currentAnim]);
		if(hair != 0)
			PlayAnimation(game, level, hair, anims[currentAnim]);
	}
	if (IsKeyReleased(KEY_LEFT)) {
		currentAnim--;

		if (currentAnim < 0)
			currentAnim = 18;

		PlayAnimation(game, level, entity, anims[currentAnim]);
		if (child != 0)
			PlayAnimation(game, level, child, anims[currentAnim]);
		if (hair != 0)
			PlayAnimation(game, level, hair, anims[currentAnim]);
	}

	if (IsKeyDown(KEY_D))
	{
		transform->positionX += 200 * GetFrameTime();
		spRender->flipX = 0;
		if (child != 0)
		{
			C_SpriteRender* csp = ecs_get(level->world, child, C_SpriteRender);
			csp->flipX = 0;
		}
		if (hair != 0)
		{
			C_SpriteRender* hsp = ecs_get(level->world, hair, C_SpriteRender);
			hsp->flipX = 0;
		}

		if(inRoll == 0)
		{
			isRunning == 0 ? PlayAnimation(game, level, entity, "walking") : PlayAnimation(game, level, entity, "run");
			if (child != 0)
				PlayAnimation(game, level, child, "walking");
			if (hair != 0)
				PlayAnimation(game, level, hair, "walking");
		}
	}
	if (IsKeyDown(KEY_A))
	{
		transform->positionX -= 200 * GetFrameTime();
		spRender->flipX = 1;
		if (child != 0)
		{
			C_SpriteRender* csp = ecs_get(level->world, child, C_SpriteRender);
			csp->flipX = 1;
		}
		if (hair != 0)
		{
			C_SpriteRender* hsp = ecs_get(level->world, hair, C_SpriteRender);
			hsp->flipX = 1;
		}

		if (inRoll == 0)
		{
			PlayAnimation(game, level, entity, "walking");
			if (child != 0)
				PlayAnimation(game, level, child, "walking");
			if (hair != 0)
				PlayAnimation(game, level, hair, "walking");
		}
	}
	if (IsKeyDown(KEY_W))
	{
		transform->positionY -= 200 * GetFrameTime();
		if (inRoll == 0)
		{
			PlayAnimation(game, level, entity, "walking");
			if (child != 0)
				PlayAnimation(game, level, child, "walking");
			if (hair != 0)
				PlayAnimation(game, level, hair, "walking");
		}
	}
	if (IsKeyDown(KEY_S))
	{
		transform->positionY += 200 * GetFrameTime();
		if (inRoll == 0)
		{
			PlayAnimation(game, level, entity, "walking");
			if (child != 0)
				PlayAnimation(game, level, child, "walking");
			if (hair != 0)
				PlayAnimation(game, level, hair, "walking");
		}
	}

	if(IsKeyPressed(KEY_R))
	{
		inRoll = 1;
		{
			PlayAnimation(game, level, entity, "roll");
			if (child != 0)
				PlayAnimation(game, level, child, "roll");
			if (hair != 0)
				PlayAnimation(game, level, hair, "roll");
		}
	}

	if(inRoll == 0)
	{
		if (IsKeyReleased(KEY_A))
		{
			PlayAnimation(game, level, entity, "idle");
			if (child != 0)
				PlayAnimation(game, level, child, "idle");
			if (hair != 0)
				PlayAnimation(game, level, hair, "idle");
		}
		if (IsKeyReleased(KEY_S))
		{
			PlayAnimation(game, level, entity, "idle");
			if (child != 0)
				PlayAnimation(game, level, child, "idle");
			if (hair != 0)
				PlayAnimation(game, level, hair, "idle");
		}
		if (IsKeyReleased(KEY_D))
		{
			PlayAnimation(game, level, entity, "idle");
			if (child != 0)
				PlayAnimation(game, level, child, "idle");
			if (hair != 0)
				PlayAnimation(game, level, hair, "idle");
		}
		if (IsKeyReleased(KEY_W))
		{
			PlayAnimation(game, level, entity, "idle");
			if (child != 0)
				PlayAnimation(game, level, child, "idle");
			if (hair != 0)
				PlayAnimation(game, level, hair, "idle");
		}
	}
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

	ecs_entity_t skeleton = ecs_lookup(level->world, "skeleton");
	C_Transform* skeletonT = ecs_get(level->world, skeleton, C_Transform);

	float d = sqrt(pow(iconTransform->positionX - skeletonT->positionX, 2) + pow(iconTransform->positionY - skeletonT->positionY, 2));
	if (d <= 196.0f)
	{
		PlayAnimation(game, level, skeleton, "death");
	}
	else
	{
		PlayAnimation(game, level, skeleton, "idle");
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

	ecs_entity_t skeleton = CreateBlankEntity(level, "skely", "skeleton");
	AddComponentToEntity(game, level, skeleton, C_SPRITE_RENDER_ID, "skeleton,1,1.0,0,0");
	AddComponentToEntity(game, level, skeleton, "animation", "skeleton");
	C_Transform* skeletonT = ecs_get(level->world, skeleton, C_Transform);
	PlayAnimation(game, level, skeleton, "idle");
	skeletonT->positionX = -445.0f;
	skeletonT->scaleX = 6.0f;
	skeletonT->scaleY = 6.0f;

	ecs_entity_t mainCamera = CreateBlankEntity(level, "camera", "main_camera");
	AddComponentToEntity(game, level, mainCamera, C_CAMERA_2D_ID, "1,0.0,0.0,0.0,0.0,0.0,1.0");
	AddEntityBehaviour(game, level, mainCamera, NULL, NULL, OnUpdateCamera, NULL, NULL);

	ecs_entity_t player = CreateBlankEntity(level, "icon", "icon");
	AddComponentToEntity(game, level, player, C_SPRITE_RENDER_ID, "player,1,1.0,0,0");
	AddComponentToEntity(game, level, player, "animation", "player");
	AddEntityBehaviour(game, level, player, OnCreate, OnInput, NULL, NULL, NULL);
	C_Transform* itransform = ecs_get(level->world, player, C_Transform);
	C_SpriteAnimation* playerSA = ecs_get(level->world, player, C_SpriteAnimation);
	playerSA->OnEnd = OnPlayerEndAnim;

	ecs_entity_t playerHair = AddChildToEntity(level, player, "longhair", "longhair");
	AddComponentToEntity(game, level, playerHair, C_SPRITE_RENDER_ID, "long_hair,1,1.0,0,0");
	AddComponentToEntity(game, level, playerHair, C_SPRITE_ANIMATION_ID, "long_hair");
	C_Transform* hairT = ecs_get(level->world, playerHair, C_Transform);
	hairT->scaleX = 4.0f;
	hairT->scaleY = 4.0f;
	PlayAnimation(game, level, playerHair, "idle");

	ecs_entity_t playerTools = AddChildToEntity(level, player, "tools", "tools");
	AddComponentToEntity(game, level, playerTools, C_SPRITE_RENDER_ID, "tools,1,1.0,0,0");
	AddComponentToEntity(game, level, playerTools, C_SPRITE_ANIMATION_ID, "tools");
	C_Transform* toolsT = ecs_get(level->world, playerTools, C_Transform);
	toolsT->scaleX = 4.0f;
	toolsT->scaleY = 4.0f;
	PlayAnimation(game, level, playerTools, "idle");

	ecs_entity_t playerHouse = CreateBlankEntity(level, "house", "player_house");
	AddComponentToEntity(game, level, playerHouse, C_MAP_RENDER_ID, "player_house");
	C_Transform* phTransform = ecs_get(level->world, playerHouse, C_Transform);
	phTransform->scaleX = 4.0f;
	phTransform->scaleY = 4.0f;
}
