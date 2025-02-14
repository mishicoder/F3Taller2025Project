#pragma once
#ifndef COMPONENTS_H
#define COMPONENTS_H

#include <flecs.h>
#include "gametypes.h"

// definiciones útiles para la carga de entidades desde Tiled.
#define C_INFO_ID             "info"
#define C_CAMERA_2D_ID        "camera2d"
#define C_RENDER_LAYER_ID     "layer"
#define C_TRANSFORM_ID        "transform"
#define C_SPRITE_RENDER_ID    "sprite"
#define C_SPRITE_ANIMATION_ID "animation"
#define C_COLOR_ID            "color"
#define C_MAP_RENDER_ID       "map"
#define C_MAP_CONTROLLER_ID   "mapController"
#define C_BEHAVIOUR_ID        "script"
#define C_RECT_COLLIDER_ID	  "rect"
#define C_CIRCLE_COLLIDER_ID  "circle"
#define C_DAY_CYCLE_ID        "daycycle"
#define C_COLLECTOR_ID        "collector"
#define C_DIALOG_ID           "dialog"
#define C_INVENTORY_ID        "inventory"
#define C_HOTBAR_ID           "hotbar"
#define C_MOVEMENT_ID         "movement"
#define C_ACTION_ID           "action"
#define C_PLAYER_STATS_ID     "pstats"
#define C_WORLD_ITEM_ID       "witem"
#define C_BUILD_ID            "build"
#define C_BUILDER_ID          "builder"
#define C_TRADER_ID           "trader"
#define C_DROP_TABLE_ID       "dtable"
#define C_FARM_LAND_ID        "fland"
#define C_CROP_ID             "crop"
#define C_TREE_ID             "tree"
#define C_ORE_ID              "ore"
#define C_BUTTON_ID			  "button"

struct Game;
struct Level;

/*********************************************************
* componentes generales
**********************************************************/

/* Componente para la gestión de cámaras en el juego. */
typedef struct C_Camera2D
{
	// Determina si la cámara es la principal.
	unsigned int isMain;
	// Desplazamiento en x de la cámara.
	float offsetX;
	// Desplazamiento en y de la cámara.
	float offsetY;

	// Objetivo en x de la cámara.
	float targetX;
	// Objetivo en y de la cámara.
	float targetY;

	// Rotación de la cámara.
	float rotation;

	// Alejamiento de la cámara.
	float zoom;
}C_Camera2D;

/* Permite que la entidad tenga nombre y etiqueta */
typedef struct Info
{
	// nombre de la entidad
	char* name;
	// etiqueta de la entidad
	char* tag;
} C_Info;

/* Capa de renderizado de la entidad (solo si tiene C_SpriteRendere) */
typedef struct C_RenderLayer
{
	// indice de la capa
	int index;
} C_RenderLayer;

/* Gestiona la posición, escala y rotación de la entidad */
typedef struct C_Transform
{
	// posicion en el eje x
	float positionX;
	// posicion en el eje y
	float positionY;

	// escala en ancho de la entidad
	float scaleX;
	// escala en alto de la entidad
	float scaleY;

	// rotación de la entidad (a favor de las agujas del reloj)
	float rotation;

	// posicion relativa (parents)
	float relX;
	float relY;
} C_Transform;

/* Permite a la entidad renderizar un sprite en el mundo. */
typedef struct C_SpriteRender
{
	// Nombre del sprite que renderizará la entidad.
	const char* spriteName;
	// Indica si el sprite es visible.
	unsigned int visible;
	// Indica la transparencia del sprite.
	float opacity; // 0.0 - 1.0
	// flip x e y
	unsigned int flipX;
	unsigned int flipY;
} C_SpriteRender;

/* Permite gestionar animaciones (Funciona unicamente con SpriteRender) */
typedef struct C_SpriteAnimation
{
	const char* sprite;
	char* currentAnimation;
	int currentFrame;
	int frameCounter;
	int fromIndex;
	int toIndex;
	int loop;
	int speed;

	void(*OnEnd)(struct Game* game, struct GameLevel* level, ecs_entity_t entity, const char* name);
} C_SpriteAnimation;

/* Permite cambiar el color del componente SpriteRender de una entidad */
typedef struct C_Color
{
	// Tonalidad roja
	unsigned int r;
	// Tonalidad verde
	unsigned int g;
	// Tonalidad azul
	unsigned int b;
} C_Color;

/* Permite a la entidad renderizar un mapa de tiles. */
typedef struct C_MapRender
{
	// Nombre del mapa a renderizar.
	const char* name;
} C_MapRender;

typedef struct C_MapController
{
	// reja de control
	int gridRows;
	int gridCols;
	int cellWidth;
	int cellHeight;

	int showGrid;
	int showCurrentCell;
	
	Cell** grid;
} C_MapController;

/* En desarrollo */
typedef struct C_Behaviour
{
	// datos generales
	void** data;
	int dataCount;
	void** numberData;
	int numberDataCount;

	// Comportamiento general
	void(*OnCreate)(struct Game* game, struct GameLevel* level, ecs_entity_t entity);
	void(*OnInput)(struct Game* game, struct GameLevel* level, ecs_entity_t entity);
	void(*OnUpdate)(struct Game* game, struct GameLevel* level, ecs_entity_t entity);
	void(*OnDestroy)(struct Game* game, struct GameLevel* level, ecs_entity_t entity);
	// Comportamiento para el tema de colisiones
	void(*OnCollision)(struct Game* game, struct GameLevel* level, ecs_entity_t entity, ecs_entity_t collide);

	// handlers adicional (para liverar datos dinámicos almacenados en memoria)
	void(*OnDestroyDataHandler)(struct Game* game, struct GameLevel* level, ecs_entity_t entity);
	void(*OnUnloadDataHandler)(struct Game* game, struct GameLevel* level, ecs_entity_t entity);
} C_Behaviour;

/* Permite que la entidad tenga colisión en el mundo. */
typedef struct C_RectCollider
{
	// Posición del collider (se actualiza con el componente transform)
	float posX;
	float posY;
	// Desplazamiento en el eje x respecto a la posición de la entidad.
	float offsetX;
	// Desplazamiento en el eje y respecto a la posición de la entidad.
	float offsetY;
	// Ancho del rectángulo de colisión.
	unsigned int width;
	// Alto del rectángulo de colisión.
	unsigned int height;
	// Determina si la caja de colisión es solida.
	unsigned short isSolid;
	int isStatic;
} C_RectCollider;

typedef struct C_CircleCollider 
{
	// Posición del collider
	float posX;
	float posY;
	// Desplazamiento en el eje x respecto a la posición de la entidad.
	float offsetX;
	// Desplazamiento en el eje y respecto a la posición de la entidad.
	float offsetY;
	// Radio del círculo
	float radius;
	// Determina si el círculo de colisión es solido.
	unsigned short isSolid;
}C_CircleCollider;

typedef struct C_PointCollider{
	float positionX;
	float positionY;

	float offsetX;
	float offsetY;
}C_PointCollider;

/* Determina que un elemento es de tipo UI */
typedef struct C_UIElement{
	int layer;
	int visible;
} C_UIElement;

typedef struct C_Button {
	// Determina que tipo de boton es (1 texto | 2 sprite)
	int type;
	// Texto que renderiza el boton en caso sea de tipo 1, caso contrario, sería el nombre del sprite.
	char* text;
	// Ancho del boton (si es 0.0 al ser creado, toma el ancho del sprite o texto)
	float widtth;
	// Alto del boton (si es 0.0 al ser creado, toma el alto del sprite o texto)
	float height;

	// Determina si el boton está activo.
	int isActive;
	// Determina si el cursor está encima del botón (por defecto es cero).
	int isHovered;

	// funcion que se ejecuta cuando el cursor está sobre el boton
	void(*OnHover)(struct Game* game, struct GameLevel* level, ecs_entity_t entity);
	// Se ejecuta cuando el usuario presiona el botón
	void(*OnClick)(struct Game* gamee, struct GameLevel* level, ecs_entity_t entity);
	// Se ejecuta cuando el mouse está sobre el elemento solo una vez
	void(*OnMouseEnter)(struct Game* game, struct GameLevel* level, ecs_entity_t entity);
	// Se ejecuta cuando el mouse ya no está sbore el elemento, una sola vez
	void (*OnMouseExit)(struct Game* game, struct GameLevel* level, ecs_entity_t entity);
} C_Button;

/*********************************************************
* componentes para eljuego
**********************************************************/

/* Controla el ciclo de día y noche. */
typedef struct C_DayCicle
{
	// tiempo en segundos reales en que pasa un minuto en el juego
	float minuteTime;
	// tiempo en el que inicia un nuevo día
	int firstDayTime;
	// Tiempo total que dura un día
	unsigned int dayTime;
	// Tiempo completo actual del día
	unsigned int currentTime;
	// Hora actual en el juego (por día)
	unsigned int currentHour;
	// Minuto actual en el juego (por día)
	unsigned int currentMinute;
} C_DayCicle;

/* Controla el rango de colección de objectos del juegador. */
typedef struct C_Collector
{
	// Desplazamiento en el eje de coordenadas x
	float offsetX;
	// Desplazamiento en el eje de coordenadas y
	float offsetY;
	// Radio de acción para recolectar los objectos.
	float radius;
} C_Collector;

/* Permite que un NPC tenga diálogos con lo que comunicarse con el jugador */
typedef struct C_Dialog
{
	// Diálogos en memoria
	char** dialogs;
	// Cantidad de diálogos cargados
	unsigned int dialogCount;
	// Maximo de interacciones que puede tener el npc
	unsigned int maxInteractions;
} C_Dialog;

/* Permite que una entidad tenga un inventario */
typedef struct C_Inventory
{
	// Cantidad máxima de slots que tiene el inventario.
	unsigned int maxSlots;
	// Slots del inventario
	InventorySlot** slots;

	/* No aplica para el inventario del jugador */
	// Indica si el inventario está bloqueado por un cerrojo.
	unsigned int isLocked;
	// Indica el tipo de llave que se necesita para que sea desbloqueado.
	unsigned int keyType;
} C_Inventory;

/* Permite que el jugador tenga una barra de acción (inventario adicional) */
typedef struct C_HotBar
{
	// Cantidad de slots de la barra de acción.
	unsigned int maxSlots;
	// Slots de la barra de acción.
	InventorySlot** slots;
	// Slot seleccionado
	int selectedSlot;
} C_HotBar;

/* Componente que dota de movimiento al jugador */
typedef struct C_Movement
{
	// Dirección en x de la entidad (puede ser 0, 1 o -1).
	signed int directionX;
	// Dirección en y de la entidad (puede ser 0, 1 o -1).
	signed int directionY;
	// Velocidad de movimiento del jugador.
	float speed;
} C_Movement;

/* Componente que permite realizar una acción con un item en el inventario. */
typedef struct C_Action
{
	// Controlador de acciones (lo gestiona el usuario).
	void (*OnAction)(InventorySlot* selectedSlot, ecs_entity_t player);
} C_Action;

/* Componente para las estadísticas del jugador */
typedef struct C_PlayerStats
{
	/*
	// Nivel máximo de recolección.
	unsigned int maxCollectionLevel;
	// Nivel de recolección actual.
	unsigned int collectionLevel;
	// Experiencia de recolección actual.
	float currentCollectionLevelExp;
	// Experiencia de recolección necesaria para el siguiente nivel.
	float nextCollectionLevelExp;

	// Nivel máximo de granjero.
	unsigned int maxFarmerLevel;
	// Nivel de granjero actual.
	unsigned int farmerLevel;
	// Experiencia de granjero actual.
	float currentFarmerLevelExp;
	// Experiencia de granjero necesaria para el siguiente nivel.
	float nextFarmerLevelExp;

	// Nivel máximo de pesca.
	unsigned int maxFisingLevel;
	// Nivel de pesca actual.
	unsigned int fishingLevel;
	// Experiencia de pesca actual.
	float currentFishingLevelExp;
	// Experiencia de pesca necesaria para el siguiente nivel.
	float nextFishingLevelExp;

	// Nivel máximo de combate.
	unsigned int maxCombatLevel;
	// Nivel de combate actual.
	unsigned int combatLevel;
	// Experiencia de combate actual.
	float currentCombatLevelExp;
	// Experiencia de combate necesaria para el siguiente nivel.
	float nextCombatLevelExp;
	*/

	int money;

	// Determina si el jugador se encuentra durmiendo o no.
	unsigned int isSleeping;

	// Vida máxima del jugador (Solo funciona en las minas).
	float maxHealth;
	// Vida actual del jugador.
	float currentHealth;

	// Energía máxima del jugador (tanto para la granja como para la cueva).
	float maxStamina;
	// Energía actual del jugador.
	float currentStamina;
} C_PlayerStats;

/* Componente para gestionar el drop de un item en el mundo. */
typedef struct C_WorldItem
{
	unsigned int itemIndex;
	unsigned int count;
} C_WorldItem;

/* Componente que indica que la entidad es una construcción */
typedef struct C_Build
{
	BUILD_TYPE type;
} C_Build;

/* Componente para que la entidad pueda construir estructuras */
typedef struct C_Builder
{
	// Estado del constructor.
	BUILDER_STATE state;
	// Construcciones que puede realizar el constructor.
	BuildItem** buildings;
	// Mejoras que puede realizar el contructor.
	BuildItem** upgrades;

	// cantidad de items
	int buildingsCount;
	int upgradesCount;
	// Construcción en la que se encuentra trabajando.
	BUILD_TYPE currentBuild;
	// Dias que le toma una construcción o mejora.
	unsigned int daysToBuild;
	// Día actual de construcción.
	unsigned int currentBuildDay;
} C_Builder;

/* Componente para que la entidad pueda vender productos */
typedef struct C_Trader
{
	// Estado del comerciante
	TRADER_STATE state;
	// Productos que vende el comerciante
	TradeItem** products;
	int productsCount;
} C_Trader;

/* Componente que permite a una entidad tener un drop */
typedef struct C_DropTable
{
	// Objetos que puede dropear la entidad
	unsigned int* items;
	// Probabilidad de arrojar los objetos
	unsigned int* dropProbability;
	// Cantidad de cada drop
	unsigned int* dropAmount;
	// Experiencia que dropea
	float exp;
} C_DropTable;

/* Componente para la tierra que puede ser manipulada */
typedef struct C_FarmLand
{
	FARMLAND_STATE state;
} C_FarmLand;

/* Componente para los cultivos */
typedef struct C_Crop
{
	// Tipo de semilla.
	CROP_TYPE seedType;
	// Días que tarda en madurar.
	unsigned int daysToMature;
	// Día actual de maduración.
	unsigned int currentCropDay;
	// Indica si está lista para ser cosechada.
	unsigned int readytoHarvest;
	// Indica si está regada.
	unsigned int isWatered;
	// Indica si está fertilizada.
	unsigned int isFertilized;
} C_Crop;

/* Componente para los árboles */
typedef struct C_Tree
{
	// Indica si está talado
	unsigned int isCutDown;
	// Indica la cantidad de días que le toma volver a crecer
	unsigned int daysToGrowBack;
	// Indica si es de generación natural o semilla
	unsigned int isNaturalGeneration;
	// Indica la cantidad de días que le toma crecer siendo semilla
	unsigned int dayToGrow;
	// Indica el día de crecimiento actual
	unsigned int currentGrowthDay;
	// Indica el estado actual del árbol (para usarlo con SpriteRender)
	//unsigned int currentState;
} C_Tree;

/* Componente para los ores */
typedef struct C_Ore {
	unsigned int type;
} C_Ore;

#endif // !COMPONENTS_H
