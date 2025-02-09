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
	// determina si se quita el boton de escape como salida del juego.
	unsigned int useEscapeToExit;
	// limita los fps
	unsigned int targetFPS;
	// colo de fondo de la pantalla del juego
	Color colorBackground;
	// Determina si se hace un debug de las colisiones y otras cosas.
	unsigned int activeDebug;
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
	signed int currentLevel;

	/**
	* Carga los recursos que se necesitan en el juego.
	*
	* @param[in] game Referencia de la instancia del juego.
	*/
	void(*LoadResources)(struct Game* game);
} Game;

/**
* Funcion para inicializar la instancia de un juego.
* Al inicializar una instancia de juego, también se inicializa
* el gestor de recursos.
* 
* @param[in] game Referencia en memoria de la instancia del juego.
* @param[in] config Estructura que contiene la configuracion del juego.
* @param[in] void(*LoadResources)(Game* game) Función para cargar los recursos.
*/
void InitGame(struct Game* game, GameConfig config, void(*LoadResources)(struct Game* game));

/**
* Establece el ícono de la ventana del juego.
* 
* @param[in] game Referencia a la instancia del juego al cual se le establecerá el ícono.
* @param[in] filename Ruta del archivo del ícono.
*/
void SetGameWindowIcon(Game* game, const char* filename);

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
* Ejecuta la animación de una entidad que tenga el componente C_SpriteAnimation.
* 
* @param[in] game Puntero a la instancia del juego.
* @param[in] level Puntero al nivel donde se encuentra la entidad.
* @param[in] entity Entidad a la que modificar el componente.
* @param[in] name Nombre de la animación.
*/
void PlayAnimation(Game* game, GameLevel* level, ecs_entity_t entity, const char* name);

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
* Agrega un nuevo conjunto de patrones a un paquete.
* 
* @param[in] game Referencia al juego de donde se obtendrá el paquete para agregar el conjunto.
* @param[in] pack Nombre del paquete donde se agregará el conjunto.
* @param[in] filename Ruta del conjunto.
*/
int AddTilsetToPack(Game* game, const char* pack, const char* filename);

/**
* Carga un paquete de conjuntos de mosaicos al gestor del recursos del juego.
* 
* @param[in] game Referencia al juego donde se cargará el paquete.
* @param[in] filename Ruta del archivo del paquete.
* @param[in] name Nombre del paquete.
* 
* @return Retorna 1 sila cargar ha sido exitosa, caso contrario, retorna 0.
*/
int LoadTilsetsPack(Game* game, const char* filename, const char* name);

/**
* Carga un mapa de mosaicos utilizando un paquete de conjunto de mosaicos.
* 
* @param[in] game Referencia al juego donde se cargará el mapa.
* @param[in] filename Ruta del archivo del mapa.
* @param[in] pack Nombre del paquete de conjuntos de mosaicos que usará el mapa.
* @param[in] name Nombre del mapa.
* 
* @return Retorna 1 si la carga ha sido exitosa, caso contrario, retorna 0;
*/
int LoadTileMap(Game* game, const char* filename, const char* pack, const char* name);

/**
* Comprueba si el nivel ya ha sido agregado.
* 
* @param[in] game Puntero a la instancia del juego.
* @param[in] name Nombre de la escena a comprobar.
* 
* @return Retorna 1 si el nivel ya existe, caso contrario, retorna 0.
*/
int CheckLevel(Game* game, const char* name);

/**
* Carga un nuevo nivel, reemplazando al que se encuentra en la parte más alta del stack.
* 
* @param[in] game Puntero a la instancia del juego.
* @param[in] filename Nombre del archivo tmx de donde se extraerá la capa de objetos.
* @param[in] name Nombre de la escena.
* @param[in] keepInMemory Indica si la escena se quedará cargada en memoria.
* @param[in] renderInStack Indica si el nivel se sigue renderizando cuando se estaquea un nuevo nivel.
* @param[in] updateInStack Indica si el nivel se sigue actualizando cuando se estaquea un nuevo nivel.
* @param[in] void(*OnLoad)(struct Game* game, struct GameLevel* level) Función que se ejecuta cuando la escena termina de ser cargada.
* 
* @return Retorna GameLevel si la carga ha tenido éxito, caso contrario, retorna 0.
*/
int LoadLevel(Game* game, const char* name, unsigned int keepInMemory, unsigned int renderInStack, unsigned int updateInStack, void(*OnLoad)(struct Game* game, struct GameLevel* level));

/**
* Carga una capa de objectos desde tiled como nivel.
*
* @param[in] game Puntero a la instancia del juego.
* @param[in] filename Nombre del archivo tmx de donde se extraerá la capa de objetos.
* @param[in] name Nombre de la escena.
* @param[in] keepInMemory Indica si la escena se quedará cargada en memoria.
* @param[in] renderInStack Indica si el nivel se sigue renderizando cuando se estaquea un nuevo nivel.
* @param[in] updateInStack Indica si el nivel se sigue actualizando cuando se estaquea un nuevo nivel.
* @param[in] void(*OnLoad)(struct Game* game, struct GameLevel* level) Función que se ejecuta cuando la escena termina de ser cargada.
*
* @return Retorna 1 si la carga ha tenido éxito, caso contrario, retorna 0.
*/
int LoadLevelF(Game* game, const char* filename, const char* name, unsigned int keepInMemory, unsigned int renderInStack, unsigned int updateInStack, void(*OnLoad)(struct Game* game, struct GameLevel* level));

/**
* Agrega un nivel a la escena.
* 
* @param[in] game Referencia a la instancia del juego.
* @param[in] name Nombre del nivel.
* @param[in] keepInMemory Indica si la escena se quedará cargada en memoria.
* @param[in] renderInStack Indica si el nivel se sigue renderizando cuando se estaquea un nuevo nivel.
* @param[in] updateInStack Indica si el nivel se sigue actualizando cuando se estaquea un nuevo nivel.
* @param[in] void(*OnLoad)(struct Game* game, struct GameLevel* level) Función que se ejecuta cuando la escena termina de ser cargada.
*/
int PushLevel(Game* game, const char* name, unsigned int keepInMemory, unsigned int renderInStack, unsigned int updateInStack, void(*OnLoad)(struct Game* game, struct GameLevel* level));

/**
* Agrega un nivel guardado en memoria al stack de niveles.
* 
* @param[in] game Referencia a la instancia del juego.
* @param[in] name Nombre del nivel.
* 
* @return Retorna 1 si encuentra y agrega el nivel, caso contrario, retorna 0.
*/
int PushMemoryLevel(Game* game, const char* name);

/**
* Carga un nivel en memoria, reemplazando al actual.
* 
* @param[in] game Puntero a la instancia del juego.
* @param[in] name Nombre del nivel a cargar.
* 
* @return Retorna 1 si la operación fue exitosa, caso contrario, retorna 0.
*/
int LoadMemoryLevel(Game* game, const char* name);

/**
* Agrega un nivel al stack de cache.
* 
* @param[in] game Puntero a la instancia del juego.
* @param[in] level Nivel a agregar.
* 
* @return Retorna 1 si la operación se completa con éxito, caso contrario, retorna 0.
*/
int AddLevelToCacheStack(Game* game, GameLevel* level);

/**
* Quita el último nivel agregado en el stack.
* 
* @param[in] game Referencia a la instancia del juego.
* 
* @return Retorna 1 si la operación resulta exitosa, caso contrario, retorna 0.
*/
int PopLevel(Game* game);

/**
* Crea un entidad en blanco, solo tendrá el componente "C_Transform".
*
* @param[in] level Puntero al nivel donde se agregará la entidad.
* @param[in] name Nombre de la entidad.
* @param[in] tag Etiqueta de la entidad (si no se quiere añadir etiqueta el valor debe ser NULL).
*
* @return Retorna ecs_entity_t cuando se crear correctamente la entidad, caso contrario, retorna 0.
*/
ecs_entity_t CreateBlankEntity(GameLevel* level, const char* name, const char* tag);

/**
* Agrega un hijo una entidad, que solo tiene el componente "C_Transform".
* 
* @param[in] level Puntero al nivel donde se agregará la entidad hija.
* @param[in] parent Entidad padre para la entidad hija.
* @param[in] name Nombre de la entidad
* @param[in] tag Etiqueta de la entidad.
* 
* @return Retorna ecs_entity_t si se crea correctamente, caso contrario, retorna 0.
*/
ecs_entity_t AddChildToEntity(GameLevel* level, ecs_entity_t parent, const char* name, const char* tag);

/**
* Retorna un entidad hija segun su indice.
* 
* @param[in] level Puntero al nivel donde se buscará la entidad hija.
* @param[in] parent Entidad padre en la cual se buscará la entidad hija.
* @param[in] index Indice de la entidad
* 
* @return Retorna ecs_entity_t si se encuentra un hijo, caso contrario, retorna 0.
*/
ecs_entity_t GetChildFromIndex(GameLevel* level, ecs_entity_t parent, int index);

/**
* Agrega un componente a una entidad cargada en base a cadenas de texto.
* El componente "C_Behaviour" no funciona en esta función, para ello se debe usar
* la función "AddEntityBehaviour"
* 
* @param[in] game Referencia a la instancia del juego.
* @param[in] level Referencia al nivel.
* @param[in] entity Entidad a la que se le agregará el componente
* @param[in] component Componente a agregar.
* @param[in] cdata Datos del componente.
* 
* @return Retorna 1 si el componente se agregó de forma exitosa, caso contrario, retorna 0.
*/
int AddComponentToEntity(Game* game, GameLevel* level, ecs_entity_t entity, const char* component, const char* cdata);

/**
* Agrega el componente "C_Behaviour" a la entidad.
* 
* @param[in] level Nivel en el que se encuentra la entidad.
* @param[in] entity Entidad a la que se le agregará el componente.
*/
int AddEntityBehaviour(Game* game, GameLevel* level, ecs_entity_t entity,
	void(*OnCreate)(Game* game, GameLevel* level, ecs_entity_t entity),
	void(*OnInput)(Game* game, GameLevel* level, ecs_entity_t entity),
	void(*OnUpdate)(Game* game, GameLevel* level, ecs_entity_t entity),
	void(*OnDestroy)(Game* game, GameLevel* level, ecs_entity_t entity),
	void(*OnCollision)(Game* game, GameLevel* level, ecs_entity_t entity, ecs_entity_t collide)
);

// Temporal
int AddLevel(struct Game* game, const char* name,
	unsigned int keepInMemory,
	unsigned int renderInStack,
	unsigned int updateInStack,
	void(*Run)(struct Game* game, struct GameLevel* level)
);

/**
* Obtiene la cámara principal del nivel actual.
* 
* @param[in] level Puntero al nivel donde se buscará la cámara principal.
*/
ecs_entity_t GetMainCamera(GameLevel* level);

/**
* Actualiza todas las entidades del nivel.
*
* @param[in] game Puntero a la instancia de juego.
* @param[in] level Puntero al nivel a actualizar.
*/
void UpdateLevel(struct Game* game, struct GameLevel* level);

/**
* Renderiza las entidades del mundo.
*
* @param[in] game Puntero a la instancia de juego.
* @param[in] level Puntero al nivel a actualizar.
*/
void RenderLevel(struct Game* gane, struct GameLevel* level);

/**
* Renderiza los elementos de interfaz del nivel.
*
* @param[in] game Puntero a la instancia de juego.
* @param[in] level Puntero al nivel a actualizar.
*/
void RenderUI(struct Game* game, struct GameLevel* level);

/**
* Libera la memoria del nivel.
*
* @param[in] game Puntero a la instancia de juego.
* @param[in] level Puntero al nivel a actualizar.
*/
void Unload(struct Game* game, struct GameLevel* level);

/**
* Renderiza elementos para debugear en el nivel.
*
* @param[in] game Puntero a la instancia de juego.
* @param[in] level Puntero al nivel a actualizar.
*/
void RenderDebug(struct Game* game, struct GameLevel* level);

/**
* Hook para limpiar memoria del componente C_MapRender
* 
* @param[in] ptr Puntero del componente.
*/
void MapRenderDestroyHook(void* ptr);

/**
* Funcion para liberar memoria de una instancia de juego.
* Se elimina de memoria los recursos del gestor de recursos.
* Se elimina de momoria los niveles cargados.
* 
* @param[in] game Referencia en memoria de la instancia del juego.
*/
void FreeGame(Game* game);

#endif // !GAME_H