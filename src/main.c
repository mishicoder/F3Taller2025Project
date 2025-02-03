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
#include "levels/menu.h"
#include "core/utilities.h"

float speed = 2.0f;

int main()
{
	InitWindow(800, 600, "Colisiones");
	SetTargetFPS(60);

	Rectangle rectA = { 20, 20, 50, 50 };
	Rectangle rectB = { 200, 80, 400, 80 };
	Circle circle = { 200, 200, 40 };

	while (!WindowShouldClose())
	{
		if (IsKeyDown(KEY_D))
		{
			rectA.x += speed;
		}
		if (IsKeyDown(KEY_A))
		{
			rectA.x -= speed;
		}
		if (IsKeyDown(KEY_W))
		{
			rectA.y -= speed;
		}
		if (IsKeyDown(KEY_S))
		{
			rectA.y += speed;
		}

		ResolveRectangleCircleCollision(&rectA, circle);
		ResolveRectRectCollision(&rectA, rectB);

		BeginDrawing();
		ClearBackground((Color) { 33, 33, 33, 255 });

		DrawRectangle(rectA.x, rectA.y, rectA.width, rectA.height, BLUE);
		DrawRectangle(rectB.x, rectB.y, rectB.width, rectB.height, RED);
		DrawCircle(circle.centerX, circle.centerY, circle.radius, WHITE);

		EndDrawing();
	}
	CloseWindow();

	/*
	GameConfig config = {0};
	config.windowTitle = "My Farm Game";
	config.windowWidth = 800;
	config.windowHeight = 600;
	config.windowFullscreen = 0;
	config.useEscapeToExit = 1;
	config.colorBackground = (Color){ 20, 23, 21, 255 };

	Game game;
	InitGame(&game, config, NULL);
	SetGameWindowIcon(&game, "assets/game_icon.png");

	//AddLevel(&game, "test", 0, 0, 0, TL_Run);
	AddLevel(&game, "menu", 0, 0, 0, Menu_Run);

	RunGame(&game);
	FreeGame(&game);
	*/

	return 0;
}