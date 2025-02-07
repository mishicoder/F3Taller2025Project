#include "menu.h"

void OnInput(Game* game, GameLevel* level, ecs_entity_t entity)
{
	ECS_COMPONENT(level->world, C_Info);

	if (IsKeyReleased(KEY_N))
	{
		C_Info* info = ecs_get(level->world, entity, C_Info);
		printf("NOMBRE DE LA ENTIDAD: %s | ETIQUETA DE LA ENTIDAD: %s\n", info->name, info->tag);
	}
}

void MenuOnLoad(Game* game, GameLevel* level)
{
	ECS_COMPONENT(level->world, C_Camera2D);
	ECS_COMPONENT(level->world, C_Behaviour);
	ECS_COMPONENT(level->world, C_MapRender);
	ECS_COMPONENT(level->world, C_Transform);

	if (level->isComponentsAdded == 0)
	{
		ecs_entity_t playerHouse = CreateBlankEntity(level, "house", "player_house");
		AddComponentToEntity(game, level, playerHouse, C_MAP_RENDER_ID, "player_house");
		C_Transform* phTransform = ecs_get(level->world, playerHouse, C_Transform);
		phTransform->scaleX = 4.0f;
		phTransform->scaleY = 4.0f;

		level->isComponentsAdded = 1;
	}
}
