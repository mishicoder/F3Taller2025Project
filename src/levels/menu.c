#include "menu.h"

void OnCreate(Game* game, GameLevel* level, ecs_entity_t entity)
{
	PlayAnimation(game, level, entity, "idle");
}

int currentAnim = 0;
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

	//printf("Anim: %s\n", anims[currentAnim]);

	if (IsKeyReleased(KEY_RIGHT)) {
		currentAnim++;

		if (currentAnim >= 19)
			currentAnim = 0;

		PlayAnimation(game, level, entity, anims[currentAnim]);
	}
	if (IsKeyReleased(KEY_LEFT)) {
		currentAnim--;

		if (currentAnim < 0)
			currentAnim = 18;

		PlayAnimation(game, level, entity, anims[currentAnim]);
	}

	if (IsKeyDown(KEY_D))
	{
		transform->positionX += 200 * GetFrameTime();
		spRender->flipX = 0;
		if(inRoll == 0)
			PlayAnimation(game, level, entity, "walking");
	}
	if (IsKeyDown(KEY_A))
	{
		transform->positionX -= 200 * GetFrameTime();
		spRender->flipX = 1;
		if (inRoll == 0)
			PlayAnimation(game, level, entity, "walking");
	}
	if (IsKeyDown(KEY_W))
	{
		transform->positionY -= 200 * GetFrameTime();
		if (inRoll == 0)
			PlayAnimation(game, level, entity, "walking");
	}
	if (IsKeyDown(KEY_S))
	{
		transform->positionY += 200 * GetFrameTime();
		if (inRoll == 0)
			PlayAnimation(game, level, entity, "walking");
	}

	if(IsKeyPressed(KEY_R))
	{
		inRoll = 1;
		PlayAnimation(game, level, entity, "roll");
	}

	if(inRoll == 0)
	{
		if (IsKeyReleased(KEY_A))
			PlayAnimation(game, level, entity, "idle");
		if (IsKeyReleased(KEY_S))
			PlayAnimation(game, level, entity, "idle");
		if (IsKeyReleased(KEY_D))
			PlayAnimation(game, level, entity, "idle");
		if (IsKeyReleased(KEY_W))
			PlayAnimation(game, level, entity, "idle");
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
	printf("Distancia: %.2f\n", d);
	if (d <= 196.0f)
	{
		PlayAnimation(game, level, skeleton, "death");
	}
	else
	{
		PlayAnimation(game, level, skeleton, "idle");
	}

	ecs_entity_t regadera = ecs_lookup(level->world, "regadera");
	C_Transform* regaderaTransform = ecs_get(level->world, regadera, C_Transform);
	C_SpriteAnimation* regaderaAnim = ecs_get(level->world, regadera, C_SpriteAnimation);
	regaderaTransform->scaleX = 4.0f;
	regaderaTransform->scaleY = 4.0f;

	if (IsKeyReleased(KEY_P))
	{
		PlayAnimation(game, level, regadera, "run");
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
	AddComponentToEntity(game, level, mainCamera, C_CAMERA_2D_ID, "1,0.0,0.0,0.0,0.0,0.0,1.2");
	AddEntityBehaviour(game, level, mainCamera, NULL, NULL, OnUpdateCamera, NULL, NULL);

	ecs_entity_t regadera = CreateBlankEntity(level, "regadera", "regadera");
	AddComponentToEntity(game, level, regadera, C_SPRITE_RENDER_ID, "regadera,1,1.0,0,0");
	AddComponentToEntity(game, level, regadera, C_SPRITE_ANIMATION_ID, "regadera");
	ecs_entity_t iconSpt = CreateBlankEntity(level, "icon", "icon");
	AddComponentToEntity(game, level, iconSpt, C_SPRITE_RENDER_ID, "player,1,1.0,0,0");
	AddComponentToEntity(game, level, iconSpt, "animation", "player");
	AddEntityBehaviour(game, level, iconSpt, OnCreate, OnInput, NULL, NULL, NULL);
	C_Transform* itransform = ecs_get(level->world, iconSpt, C_Transform);
	C_SpriteAnimation* playerSA = ecs_get(level->world, iconSpt, C_SpriteAnimation);
	playerSA->OnEnd = OnPlayerEndAnim;

	ecs_entity_t playerHouse = CreateBlankEntity(level, "house", "player_house");
	AddComponentToEntity(game, level, playerHouse, C_MAP_RENDER_ID, "player_house");
	C_Transform* phTransform = ecs_get(level->world, playerHouse, C_Transform);
	phTransform->scaleX = 4.0f;
	phTransform->scaleY = 4.0f;
}
