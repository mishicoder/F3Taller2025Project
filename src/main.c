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

/*
* 0 -> carrot
* 1 -> cauliflower
* 2 -> parsnip
* 3 -> potato
* 4 -> pumpkin
* 5 -> sunflower
* 6 -> wheat
* 7 -> beetroot
* 8 -> cabbage
* 9 -> kale
* 10 -> radish
* 
* 11 -> coal
* 12 -> silver
* 13 -> gold
* 14 -> diamond
* 15 -> stone
*/

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
	LoadSprite(game, "assets/game_icon.png", "icon");
	LoadSpriteWithOptions(game, "assets/sprites/player.png", "assets/sprites/player.sprite");
	LoadTilsetsPack(game, "assets/tilesets/pack_0.tspack", "base");
	LoadTileMap(game, "assets/maps/farmhouse.tmx", "base", "player_house");
}