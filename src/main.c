/**
* @file: main.c
* @author: Mishicoder
* 
* Punto de entrada del videojuego.
* Raylib: https://github.com/raysan5/raylib/blob/master/src/raylib.h
* Hoja de Raylib: https://www.raylib.com/cheatsheet/cheatsheet.html
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <raylib.h>
#include <float.h>
#include "levels/menu.h"

void LoadResources(Game* game);

int main()
{
	GameConfig config = {0};
	config.windowTitle = "Sunny Side";
	config.windowWidth = 800;
	config.windowHeight = 600;
	config.windowFullscreen = 0;
	config.useEscapeToExit = 1;
	config.activeDebug = 0;
	config.colorBackground = (Color){ 20, 23, 21, 255 };

	Game game;
	InitGame(&game, config, LoadResources);
	SetGameWindowIcon(&game, "assets/game_icon.png");

	PushLevel(&game, "test", 1, 0, 0, MenuOnLoad);

	RunGame(&game);
	FreeGame(&game);

	return 0;
}

void LoadResources(Game* game)
{
	// Cursores
	LoadCustomCursor(game, "default", "assets/sprites/ui/default.png", 2.0, 2.0);
	LoadCustomAnimatedCursor(game, "hand", "assets/sprites/ui/hand.png", 2, 16, 16, 8, 2.0f, 2.0f);

	// Cargar semillasy plantas
	LoadSpriteAtlas(game, "assets/sprites/plants/crops.png", "assets/sprites/plants/crops.atlas");
	LoadSpriteAtlas(game, "assets/sprites/plants/seeds.png", "assets/sprites/plants/seeds.atlas");
	LoadSpriteWithOptions(game, "assets/sprites/plants/bushes.png", "assets/sprites/plants/bushes.sprite");

	LoadSpriteAtlas(game, "assets/sprites/player/tools_items.png", "assets/sprites/player/tools_items.atlas");

	LoadSpriteWithOptions(game, "assets/sprites/player/player.png", "assets/sprites/player/player.sprite");
	LoadSpriteWithOptions(game, "assets/sprites/player/tools.png", "assets/sprites/player/tools.sprite");
	LoadSpriteWithOptions(game, "assets/sprites/player/longhair.png", "assets/sprites/player/longhair.sprite");
	LoadSpriteWithOptions(game, "assets/sprites/player/bowlhair.png", "assets/sprites/player/bowlhair.sprite");

	LoadTilsetsPack(game, "assets/tilesets/pack_0.tspack", "base");
	LoadTileMap(game, "assets/maps/farmhouse.tmx", "base", "player_house");

	LoadSpriteWithOptions(game, "assets/sprites/enemy/skeleton.png", "assets/sprites/enemy/skeleton.sprite");
	LoadSpriteWithOptions(game, "assets/sprites/enemy/goblin.png", "assets/sprites/enemy/goblin.sprite");
}