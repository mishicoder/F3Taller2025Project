#pragma once
#ifndef GAME_H
#define GAME_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <mxml.h>
#include "resources_manager.h"
#include "level.h"

/*
* Configuracion del juego.
*/
typedef struct GameConfig{
	// titulo de la ventana
	const char* windowTitle;
	// ancho de la ventana
	unsigned int windowWidth;
	// alto de la ventana
	unsigned int windowHeight;
	// determina si se usará pantalla completa
	// al iniciar el juego
	unsigned int windowFullscreen;
	// determina si se quita el boton de escape como
	// salida del juego
	unsigned int useEscapeToExit;
	// colo de fondo de la pantalla del juego
	Color colorBackground;
} GameConfig;

/**
* Estructura para crear la instancia de un juego.
* Se encarga de gestionar recursos y niveles.
*/
typedef struct Game
{
	/* Configuracion del juego. */
	GameConfig config;
	/* gestor de recursos del juego. */
	ResourcesManager resourcesManager;
	/* Niveles que se quedan cargados en memoria al cambiar a otro nivel.*/
	GameLevel** levelCache;
	unsigned int levelCacheCount;
	/* Stack de niveles. */
	struct GameLevel** levelStack;
	unsigned int levelStackCount;
} Game;

/**
* Funcion para inicializar la instancia de un juego.
* Al inicializar una instancia de juego, también se inicializa
* el gestor de recursos.
* 
* @param[in] game Referencia en memoria de la instancia del juego.
* @param[in] config Estructura que contiene la configuracion del juego.
*/
void InitGame(struct Game* game, GameConfig config);

/** 
* Inicialia el bucle principal del juego.
* 
* @param[in] game Referencia en memoria de la instancia del juego.
*/
void RunGame(struct Game* game);

/**
* Funcion para cargar un sprite al gestor de recursos del juego.
* Se libera de memoria automáticamente cuando se cierra el juego.
* 
* @param[in] game Puntero a la instancia del juego.
* @param[in] textureFilename Ruta de la imagen que se cargará como sprite.
* @param[in] name Nombre del sprite.
* 
* @return Retorna 1 si la carga ha tenido éxito, caso contrario, retorna 0.
*/
int LoadSprite(Game* game, const char* textureFilename, const char* name);

/**
* Funcion para cargar un sprite con carga de frames y animaciones.
* 
* @param[in] game Puntero a la instancia del juego.
* @param[in] textureFilename Ruta de la imagen que se cargará como sprite.
* @param[in] dataFilename Ruta del archivo que contiene los datos de carga del sprite.
* 
* @return Retorna 1 si la carga ha tenido éxito, caso contrario, retorna 0.
*/
int LoadSpriteWithOptions(Game* game, const char* textureFilename, const char* dataFilename);

/**
* Funcion para cargar varios sprites desde un archivo.
* 
* @param[in] game Puntero a la instancia del juego.
* @param[in] textureFilename Ruta de la imagen que se cargará como sprite.
* @param[in] dataFilename Ruta del archivo que contiene los datos de los sprites a cargar.
* 
* @return Retorna 1 si la carga ha tenido éxito, caso contrario, retorna 0.
*/
int LoadSpriteAtlas(Game* game, const char* textureFilename, const char* dataFilename);

/**
* Funcion que permite registrar y reafirma los componentes en los mundos de entidades.
* 
* @param[in] game Referencia en memoria de la instancia del juego.
* @param[in] level Referencia en memoria del nivel.
*/
void DefineComponents(Game* game, GameLevel* level);

/**
* Funcion para cargar un nivel en el juego a traves de un archivo.
* 
* @param[in] game Referencia en memoria de la instancia del juego.
* @param[in] filename Ruta del archivo del nivel que se cargará.
* 
* @return Retorna 1 si el nivel se ha logrado cargar, caso contrario, 0.
*/
int LoadLevel(struct Game* game, const char* filename);

int AddLevel(struct Game* game, const char* name,
	unsigned int keepInMemory,
	unsigned int renderInStack,
	unsigned int updateInStack,
	void(*Run)(struct Game* game, struct GameLevel* level)
);

/**
* Funcion para liberar memoria de una instancia de juego.
* Se elimina de memoria los recursos del gestor de recursos.
* Se elimina de momoria los niveles cargados.
* 
* @param[in] game Referencia en memoria de la instancia del juego.
*/
void FreeGame(Game* game);

#endif // !GAME_H