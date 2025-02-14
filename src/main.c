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
#include "game/levels/farm.h"
#include "game/levels/house.h"

void LoadResources(Game* game);

int main()
{
	/*
	// Inicializar ventana
    const int screenWidth = 800;
    const int screenHeight = 600;
    InitWindow(screenWidth, screenHeight, "9-Slice con NPatch");

    // Cargar imagen
    Texture2D texture = LoadTexture("assets/sprites/ui/panel2.png");

    // Definir el N-Patch (bordes que no se estiran)
    int border = 3;  // Ajusta según la imagen
    NPatchInfo patch = {
        (Rectangle){ 0, 0, texture.width, texture.height },  // Región de la imagen completa
        7, 7, 7, 11,  // Márgenes (left, top, right, bottom)
        NPATCH_NINE_PATCH  // Tipo de parche (9-Slice)
    };

    // Área donde se dibujará la imagen
    Rectangle dest = { 200, 150, 400, 300 };

    // Bucle principal
    while (!WindowShouldClose())
    {
        BeginDrawing();
        ClearBackground(RAYWHITE);

        // Dibujar el N-Patch
        DrawTextureNPatch(texture, patch, dest, (Vector2){ 0, 0 }, 0.0f, WHITE);
		DrawTexturePro(texture, (Rectangle) { 0, 0, texture.width, texture.height }, (Rectangle) { 20, 50, texture.width * 4, texture.height * 4 }, (Vector2) { 0.0, 0.0 }, 0.0, WHITE);

        DrawText("9-Slice con NPatch", 10, 10, 20, DARKGRAY);
        EndDrawing();
    }

    // Liberar recursos
    UnloadTexture(texture);
    CloseWindow();
	*/
	
	GameConfig config = {0};
	config.windowTitle = "Sunny Side";
	config.windowWidth = 800;
	config.windowHeight = 600;
	config.windowFullscreen = 1;
	config.useEscapeToExit = 1;
	config.activeDebug = 0;
	config.colorBackground = (Color){ 20, 23, 21, 255 };

	Game game;
	InitGame(&game, config, LoadResources);
	SetGameWindowIcon(&game, "assets/game_icon.png");

	//PushLevel(&game, "test", 1, 0, 0, MenuOnLoad);
	PushLevel(&game, "house", 0, 0, 0, HouseOnLoad);

	RunGame(&game);
	FreeGame(&game);
	
	return 0;
}

void LoadResources(Game* game)
{
	// Cursores
	LoadCustomCursor(game, "default", "assets/sprites/ui/default.png", 2.0, 2.0);
	LoadCustomCursor(game, "action", "assets/sprites/ui/action.png", 2.0, 2.0);
	LoadCustomCursor(game, "dig", "assets/sprites/ui/dig.png", 2.0, 2.0);
	LoadCustomCursor(game, "chop", "assets/sprites/ui/chop.png", 2.0, 2.0);
	LoadCustomCursor(game, "mine", "assets/sprites/ui/mine.png", 2.0, 2.0);
	LoadCustomCursor(game, "slash", "assets/sprites/ui/slash.png", 2.0, 2.0);
	LoadCustomCursor(game, "watering", "assets/sprites/ui/watering.png", 2.0, 2.0);
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

	SetCustomCursor(game, "default");
}