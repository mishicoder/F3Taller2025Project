#include "menu.h"

void OnInput(Game* game, GameLevel* level, ecs_entity_t entity)
{
	ECS_COMPONENT(level->world, C_Info);

	if (IsKeyDown(KEY_N))
	{
		C_Info* info = ecs_get(level->world, entity, C_Info);
		printf("NOMBRE DE LA ENTIDAD: %s | ETIQUETA DE LA ENTIDAD: %s\n", info->name, info->tag);
	}
}

void MenuOnLoad(Game* game, GameLevel* level)
{
	ECS_COMPONENT(level->world, C_Camera2D);
	ECS_COMPONENT(level->world, C_Behaviour);

	if (level->isComponentsAdded == 0)
	{
		LoadTilsetsPack(game, "assets/tilesets/pack_0.tspack", "base");
		LoadTileMap(game, "assets/maps/farmhouse.tmx", "base", "player_house");

		ecs_entity_t myEnt = CreateBlankEntity(level, "player", "player");
		AddEntityBehaviour(level, myEnt, NULL, OnInput, NULL, NULL, NULL);
		if (myEnt != 0)
		{
			printf("SE CREO LA ENTIDAD EN BLANCO: %llu\n", myEnt);
			if(AddComponentToEntity(game, level, myEnt, C_CAMERA_2D_ID, "1,0,0,0,0,0,1") == 1)
			{
				C_Camera2D* myCamera = ecs_get(level->world, myEnt, C_Camera2D);
				printf("CAMERA ZOOM: %.2f\n", myCamera->zoom);
			}
		}

		ecs_entity_t anotherEnt = CreateBlankEntity(level, "bomb", "enemy");
		AddEntityBehaviour(level, anotherEnt, NULL, OnInput, NULL, NULL, NULL);

		level->isComponentsAdded = 1;
	}

	ecs_entity_t playerEnt = ecs_lookup(level->world, "enemy");
	C_Behaviour* beh = ecs_get(level->world, playerEnt, C_Behaviour);
	if (beh != NULL)
	{
		beh->OnInput(game, level, playerEnt);
	}

	if (IsKeyPressed(KEY_SPACE))
	{
		LoadLevel(game, "another", 0, 0, 0, Another);
	}
	if (IsKeyPressed(KEY_O))
	{
		LoadLevel(game, "tuki", 0, 0, 0, Tuki);
	}

	TileMap* map = GetTileMap(&game->resourcesManager, "player_house");
	if (map != NULL)
	{
		DrawTexturePro(map->mapTexture.texture,
			(Rectangle) {
			0, 0, map->mapTexture.texture.width, -map->mapTexture.texture.height
		},
			(Rectangle) {
			20, 20, map->mapTexture.texture.width * 4.0f, map->mapTexture.texture.height * 4.0f
		},
			(Vector2) {
			0.0f, 0.0f
		}, 0.0f, WHITE
		);
	}
}

void Another(Game* game, GameLevel* level)
{
	//printf("CANTIDAD DE NIVELES EN CACHE: %d\nCANTIDAD DE NIVELES EN STACK: %d\n\n", game->levelCacheCount, game->levelStackCount);
	if (IsKeyPressed(KEY_R))
	{
		printf("LEVEL CACHE NAME: %s\n", game->levelCache[0]->name);
	}
	if (IsKeyPressed(KEY_SPACE))
	{
		PushMemoryLevel(game, "test");
	}
}

void Tuki(Game* game, GameLevel* level)
{
	printf("NIVELES EN STACK: %d | NIVELES EN CACHE: %d\n", game->levelStackCount, game->levelCacheCount);
	if (IsKeyPressed(KEY_SPACE))
	{
		PushMemoryLevel(game, "test");
	}
}
