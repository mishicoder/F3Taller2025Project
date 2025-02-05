#pragma once
#ifndef LEVEL_H
#define LEVEL_H

#include <flecs.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "components.h"

struct Game;

typedef struct GameLevel
{
	// nombre del nivel
	char* name;
	// indica si el nivel se queda cargado en memoria
	unsigned int keepInMemory;
	// determina si la escena se sigue renderizado cuando
	// se desplaza en el stack
	unsigned int renderInStack;
	// deternina si la escene se sigue actualizando cuado
	// se desplaza en el stack
	unsigned int updateInStack;
	// mundo para contener a todas la entidades
	ecs_world_t* world;
	// vanderilla de apoyo
	unsigned int isComponentsAdded;

	void(*Run)(struct Game* game, struct GameLevel* level);

	/**
	* Se ejecuta cuando la escena es cargada (punto para agregar entidades o cargar datos).
	* Cuando se mantiene en memoria, no se vuelve a ejecutar cuando se agrega al satck con PushMemoryLevel.
	* 
	* @param[in] game Referencia de memoria de la instancia del juego.
	* @param[in] level Referencia al nivel actual.
	*/
	void(*Load)(struct Game* game, struct GameLevel* level);

	/**
	* Se actualiza en cada frame del juego (controla los sistema de control de entrada,
	* actualización de componentes y resolución de físicas.
	* 
	* @param[in] game Referencia de memoria de la instancia del juego.
	* @param[in] level Referencia al nivel actual.
	*/
	void(*Update)(struct Game* game, struct GameLevel* level);

	/**
	* Se encarga de renderizar las entidades que poseen componentes gráficos, las ordena por orden de posición
	* y por capas de renderizado.
	* 
	* @param[in] game Referencia a la instancia de juego.
	* @param[in] level Referencia al nivel actual.
	*/
	void(*Render)(struct Game* gane, struct GameLevel* level);

	/**
	* Se encarga de renderizar los elementos de interfáz gráfica.
	* 
	* @param[in] game Referencia a la instancia del juego.
	* @param[in] level Referencia al nivel actual.
	*/
	void(*RenderUI)();

	/**
	* Se encarga de eliminar la memoria del nivel.
	* Si el nivel se mantiene en memoria, esta función se ignora, hasta que se cierre el juego.
	*
	* @param[in] game Referencia de memoria de la instancia del juego.
	* @param[in] level Referencia al nivel actual.
	*/
	void(*Unload)(struct Game* game, struct GameLevel* level);

	/**
	* Se encarga de renderizar las cajas de colisión y otros elementos para el sistema debug.
	* Solo se ejecutará si la configuración del juego indica que se debe mostrar el debug.
	* 
	* @param[in] game Referencia a la instancia del juego.
	* @param[in] level Referencia al nivel actual.
	*/
	void(*RenderDebug)(struct Game* game, struct GameLevel* level);
} GameLevel;

/**
* Funcion para crear un nuevo nivel de juego.
* 
* @param[in] name Nombre del nivel.
* @param[in] keepInMemory Indica si la escena se mantendrá en memoria cuando se cambie de nivel.
* @param[in] void(*OnLoad)(struct Game* game, struct GameLevel* level) Funcion que se ejecuta cuando la escena se carga.
* (si la escena se mantiene en memoria, no se vuelve a ejecutar al volver a cambiar el nivel).
* @param[in] void(*MoveEntities)(struct Game* game, struct GameLevel* currentLevel, struct GameLevel* nextLevel) 
* Funcion que permite cambiar entidades de un nivel x a un nivel y.
* 
* @return Una bloque de memoria GameLevel si no se detectan errores, de caso contrario, se retorna NULL.
*/
GameLevel* CreateLevel(
	const char* name,
	unsigned int keepInMemory,
	unsigned int renderInStack,
	unsigned int updateInStack,
	void(*Load)(struct Game* game, struct GameLevel* level)
);

//ecs_entity_t CreateEntity(struct GameLevel* level, unsigned int ccount);
//ecs_entity_t CreateEntityFF(struct GameLevel* level, const char* filename);

/**
* Funcion para liberar la memoria asignada para un nivel.
* 
* @param[in] level Dirección en memoria del nivel que será borrado.
*/
void UnloadLevel(struct GameLevel* level);

/**
* Funcion para buscar a una entidad por etiqueta.
* 
* @param[in] level Puntero del nivel.
* @param[in] tag Etiqueta de la entidad a buscar.
* 
* @return Retorna la primera coincidencia, caso contrario, retorna NULL.
*/
ecs_entity_t GetEntityByTag(struct GameLevel* level, const char* tag);

/**
* Funcion para ejecutar una accion para cada entidad con la etiqueta especificada.
* 
* @param[in] level Puntero al nivel donde se ejecutará la función.
* @param[in] tag Etiqueta a comparar en la busqueda.
* @param[in] void func(ecs_entity_t ent) Funcion a ejecutar.
*/
void ForEntityWithTag(struct GameLevel* level, const char* tag, void(*func)(struct GameLevel* level, ecs_entity_t ent));

#endif // !LEVEL_H
