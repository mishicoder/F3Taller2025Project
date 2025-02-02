#include "menu.h"

void Menu_Run(Game* game, GameLevel* level, ecs_entity_t entity)
{
	if (level->isComponentsAdded == 0)
	{

		LoadSprite(game, "assets/window_icon.png", "test");

		LoadTilsetsPack(game, "assets/tilesets/pack_0.tspack", "base");
		LoadTileMap(game, "assets/maps/farm_ext.tmx", "base", "test");
		LoadTileMap(game, "assets/maps/farmhouse.tmx", "base", "player_house");

		level->isComponentsAdded = 1;
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
