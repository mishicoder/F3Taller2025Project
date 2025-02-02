#include "menu.h"

void Menu_Run(Game* game, GameLevel* level, ecs_entity_t entity)
{
	if (level->isComponentsAdded == 0)
	{

		LoadSprite(game, "assets/window_icon.png", "test");
		Sprite* ts = GetSprite(&game->resourcesManager, "test");
		if (ts != NULL)
			printf("Ancho: %d | Alto: %d\n", ts->width, ts->height);

		LoadTilsetsPack(game, "assets/tilesets/pack_0.tspack", "base");
		TilesetPack* pack = GetTilesetPack(&game->resourcesManager, "base");
		if (pack != NULL)
		{
			printf("TILESET: %d\n", pack->tilesetsCount);
			printf("El paquete se encuentra\n");
		}

		LoadTileMap(game, "assets/maps/farm_ext.tmx", "base", "test");
		//TileMap* map = GetTileMap(&game->resourcesManager, "test");
		//if (map != NULL)
		//	printf("Mapa cargado\n");

		level->isComponentsAdded = 1;
	}
}
