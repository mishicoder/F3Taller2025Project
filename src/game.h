#pragma once
#ifndef GAME_H

#define MAX_ENTITY_TO_DESTROY 256

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <mxml.h>
#include <lua.h>
#include <lauxlib.h>
#include <lualib.h>
#include "resources_manager.h"
#include "level.h"
#include "components.h"
#include "parser.h"
#include "utilities.h"
#include "handlers.h"
#include "luascripting.h"

//static ecs_entity_t destroyQueue[MAX_ENTITY_TO_DESTROY];
//static int destroyQueueCount = 0;

typedef struct
{
  char* key;
  int value;
} LuaCallbacks;

typedef struct LevelNode
{
  Level* level;
  struct LevelNode* next;
} LevelNode;

typedef struct
{
  char* name;
  Texture2D texture;
  bool isAnimated;
  int frames;
  int frameWidth;
  int frameHeight;
  int animSpeed;
  float scalex;
  float scaley;
  int currentFrame;
  int frameCounter;
} CustomCursor;

typedef struct
{
  const char* windowTitle;
  int windowWidth;
  int windowHeight;
  bool windowFullscreen;
  bool useEscapeToExit;
  int targetFPS;
  Color windowColor;
  bool activeDebug;
  //const char* luaCallbacksFile;
  float globalScale;

  // sistema de físicas
}GameConfig;

typedef struct Game
{
  GameConfig config;
  bool isRunning;
  int exitCode;
  ResourcesManager resManager;

  //lua_State* LuaCallbacksSate;
  //LuaCallbacks* luaCallbacks;

  LevelNode* levelStack;
  LevelNode* cacheLevels;

  CustomCursor** cursors;
  int cursorsCount;
  CustomCursor* currentCursor;

  // querys
  ecs_query_t* tagQuery;

  void(*LoadResources)(void);
}Game;

extern Game game;

// Funcion para definir los componentes por defecto registrados en el motor
void DefineComponents(Level* level);
// Funcion para enviar un string a los scripts de Lua
void PushCstringToLua(lua_State* L, const char* str);
// Funcion para reporucir una animación desde Lua (Siempre y cuando la entidad tenga el componente <AnimationController>)
int LuaPlayAnimation(lua_State* L);
// Funcion para obtener parametros de los componentes desde Lua
int LuaGetComponentField(lua_State* L);
// Funcion para obtener el componente de una entidad desde Lua
int LuaGetComponent(lua_State* L);
// Funcion para agregar un componente a una entidad desde Lua
int LuaAddComponent(lua_State* L);
// Funcion para destruir una entidad desde Lua
int LuaDestroyEntity(lua_State* L);
// Funcion para remover un componente de una entidad desdelua
int LuaRemoveComponent(lua_State* L);
// Funcion para crear una entidad 2D desde lua
int LuaCreate2DEntity(lua_State* L);
// Funcion para crear una entidad 3D desde lua
//int LuaCreate3DEntity(lua_State* L);
// Funcion para crear un prefab desde Lua (a traves de un archivo ".prefab")
//int LuaIntantiatePrefab(lua_State* L);
// Funcíon para establecer el valor de los datos de los componentes desde Lua
int LuaSetComponentField(lua_State* L);
// Funcion para obtener una entidad por nombre
int LuaGetEntityByName(lua_State* L);
// Funcion para obtener una entidad por etiqueta
int LuaGetEntityByTag(lua_State* L);

// Funcion para cerrar la aplicación
int LuaQuitGame(lua_State* L);

void AddEntityToDestroyQueue(Level* level, ecs_entity_t entity);
//void FlushEntityDestroyQueue(ecs_world_t* world);

/**
 * @brief Inicializa la instancia de juego con las configuraciones dadas y una función de carga de recursos.
 *
 * @param config Configuración del juego.
 * @param LoadResources Función que contiene la carga de recursos para el juego.
*/
void InitGame(GameConfig config, void(*LoadResources)(void));

//void RegisterCallback(const char* name);
//void ExecuteCallback(const char* name);
//void CleanupLuaCallbacks(lua_State* L);

/**
 * @brief Establece el ícono de la ventana para el juego.
 *
 * @param imgFilename Ruta a la imágen del ícono.
 *
 * @return Retorna true si se ha logrado establecer el ícono, caso contrario retorna false.
*/
bool SetGameWindowIcon(const char* imgFilename);

/**
 * @brief Carga un cursor personalizado.
 *
 * @param name Nombre del cursor.
 * @param texFilename Ruta a la imagen del cursor.
 * @param scalex Escala en x del cursor.
 * @param scaley Escala en y del cursor.
 *
 * @return Retorna true si la carga ha tenido éxito, caso contrario retorna false.
*/
bool LoadCustomCursor(const char* name, const char* texFilename, float scalex, float scaley);

/**
 * @brief Carga un cursor personalizado con animación.
 *
 * @param name Nombre del cursor.
 * @param texFilename Ruta a la imagen del cursor.
 * @param frames Cantidad de frames que tiene el cursor.
 * @param fwidth Ancho de cada frame.
 * @param fheight Alto de cada frame.
 * @param speed Velocidad de la animación.
 * @param scalex Escala en x del cursor.
 * @param scaley Escala en y del cursor.
 *
 * @return Retorna true si la carga ha tenido éxito, caso contrario retorna false.
*/
bool LoadAnimatedCursor(const char* name, const char* texFilename, int frames, int fwidth, int fheight, int speed, float scalex, float scaley);

/**
 * @brief Establece un cursor personalizado.
 *
 * @param name Nombre del cursor.
 *
 * @return Retorna true si el cursor existe, caso contrario retorna false.
*/
bool SetCustomCursor(const char* name);

/**
 * @brief Carga un sprite con un identificador.
 *
 * @param texFilename Ruta al archivo de la textura del sprite.
 * @param name Nombre del sprite.
 *
 * @return Retorna true si la carga ha tenido éxito, caso contrario retorna false.
*/
bool LoadSprite(const char* texFilename, const char* name);

/**
 * @brief Carga un sprite con animaciones desde un archivo .sprite.
 *
 * @param texFilename Ruta al archivo de la textura del sprite.
 * @param sprFilename Ruta al archivo .sprite.
 *
 * @return Retorna true si la carga ha tenido éxito, caso contrario retorna false.
*/
bool LoadSpriteWithOptions(const char* texFilename, const char* sprFilename);

/**
 * Carga varios sprites (sin opciones) desde un archivo .atlas.
 *
 * @param texFilename Ruta al archivo de la textura del sprite.
 * @param atlasFilename Ruta al archivo (.atlas).
 *
 * @return Retorna true si la carga ha tenido éxito, caso contrario retorna false.
*/
bool LoadSpriteAtlas(const char* texFilename, const char* atlasFilename);

/**
 * Carga un paquete de conjuntos de mosaicos.
 *
 * @param filename Ruta al archivo (.pack).
 * @param name Nombre del paquete de conjuntos de mosaicos.
 *
 * @return Retorna true si la carga ha tenido éxito, caso contrario retorna false.
*/
bool LoadTilesetPack(const char* filename, const char* name);

/**
 * Agrega un conjunto de mosaicos a un paquete de conjuntos de mosaicos.
 *
 * @param filename Ruta al archivo (.tsx).
 * @param packname Nombre del paquete al que se agregará el nuevo conjunto.
 *
 * @return Retorna true si la carga ha tenido éxito, caso contrario retorna false.
*/
bool LoadTilesetToPack(const char* filename, const char* packname);

/**
 * Carga un mapa de conjuntos de mosaicos.
 *
 * @param filename Ruta al archivo (.tmx).
 * @param packname Nombre del paquete de conjuntos de mosaicos que usará el mapa.
 * @param name Nombre del mapa.
 *
 * @return Retorna true si la carga ha tenido éxito, caso contrario retornal false.
*/
bool LoadTileMap(const char* filename, const char* packname, const char* name);

/**
 * @brief Carga un nivel y lo coloca en el contexto principal.
 *
 * @param name Nombre del nivel.
 * @param keepInMemory Indica si el nivel se mantiene en memoria.
 * @param renderInStack Indica si el nivel se renderiza cuando se agrega un nivel por encima.
 * @param updateInStack Indice si el nivel se actualiza cuando se agrega un nivel por encima.
 * @param OnLoad Función que se ejecuta cuando se carga el nivel (no se vuelve a ejecutar cuando se carga desde memoria).
*/
void SetLevel(const char* name, bool keepInMemory, bool renderInStack, bool updateInStack, void(*OnLoad)(Game* game, Level* level));

/**
 * @brief Agrega un nivel a la pila, ya sea que esté en caché o no.
 *
 * @param name Nombre del nivel a crear y agregar o cargar de memoria.
 * @param keepInMemory Indica si el nivel se mantiene en memoria.
 * @param renderInStack Indica si el nivel se renderiza cuando se agrega un nivel por encima.
 * @param updateInStack Indica si el nivel se actualiza cuando se agrega un nivel por encima.
 * @param OnLoad Función que se ejecuta cuando se carga el nivel (no se vuelve a ejecutar cuando se carga desde memoria).
*/
void PushLevel(const char* name, bool keepInMemory, bool renderInStack, bool updateInStack, void(*OnLoad)(Game* game, Level* level));

/**
 * @brief Quita un nivel de la pila de niveles.
*/
void PopLevel();

Level* GetCurrentLevel();

/**
 * @brief ELimina de memoria un nivel.
 *
 * @param level Nivel a eliminar de memoria.
*/
void UnloadLevel(Level* level);

/**
 * Crea una entidad que contiene los componentes Info y Transform2D por defecto.
 *
 * @param level Nivel en el que la entidad será agregado.
 * @param name Nombre de la entidad [Puede ser NULL].
 * @param tag Etiqueta de la entidad [Puede ser NULL].
 *
 * @return Retorna el id de la entidad si se ha creado correctamente, caso contrario retorna 0.
*/
ecs_entity_t Create2DEntity(Level* level, const char* name, const char* tag);
void* GetComponent(Level* level, ecs_entity_t entity, const char* componentID);
void DestroyEntity(ecs_world_t* world, ecs_entity_t entity);
ecs_entity_t AddEntity2DChild(Level* level, ecs_entity_t parent, const char* name, const char* tag);
ecs_entity_t GetChildFromIndex(Level* level, ecs_entity_t parent, int index);
void* AddComponent(Level* level, ecs_entity_t entity, const char* component, const char* cdata);

void RegisterHooks(Level* level);
ecs_entity_t GetMainCamera(Level* level);
void InitEntities(Game* gameInstance, Level* level);
void UpdateLevel(Game* gameInstance, Level* level);
void RenderLevel(Game* gameInstance, Level* level);
void FLushDestroyEntities(Level* level);
void RenderUI(Level* level);
void RenderDebug(Level* level);

/**
 * @brief Ejecuta el bucle principal del juego.
*/
void RunGame();

/**
 * @brief Libera de memoria la instancia del juego.
*/
int GameDestroy();

#endif // !GAME_H
