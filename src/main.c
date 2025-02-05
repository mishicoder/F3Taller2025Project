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
	//AddLevel(&game, "test", 0, 0, 0, TL_Run);
	//AddLevel(&game, "menu", 0, 0, 0, Menu_Run);

	RunGame(&game);
	FreeGame(&game);


	return 0;
}

void LoadResources(Game* game)
{
	printf("AQUI SE CARGARIAN TODOS LOS RECURSOS PARA EL JUEGO\n");
}