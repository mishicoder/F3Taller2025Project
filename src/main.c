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
#include <stdint.h>
#include "core/game.h"
#include "levels/test_level.h"

int main()
{
	GameConfig config = { 0 };
	config.windowTitle = "My Farm Game";
	config.windowWidth = 800;
	config.windowHeight = 600;
	config.windowFullscreen = 0;
	config.useEscapeToExit = 1;
	config.colorBackground = (Color){ 20, 23, 21, 255 };

	Game game;
	InitGame(&game, config);
	AddLevel(&game, "test", 0, 0, 0, TL_Run);
	RunGame(&game);
	FreeGame(&game);

	return 0;
}