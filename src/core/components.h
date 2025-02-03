#pragma once
#ifndef COMPONENTS_H
#define COMPONENTS_H

#include <flecs.h>
#include "gametypes.h"

struct Game;
struct Level;

/*********************************************************
* componentes generales
**********************************************************/

/* Componente para la gesti�n de c�maras en el juego. */
typedef struct C_Camera2D
{
	// Determina si la c�mara es la principal.
	unsigned int isMain;
	// Desplazamiento en x de la c�mara.
	float offsetX;
	// Desplazamiento en y de la c�mara.
	float offsetY;

	// Objetivo en x de la c�mara.
	float targetX;
	// Objetivo en y de la c�mara.
	float targetY;

	// Rotaci�n de la c�mara.
	float rotation;

	// Alejamiento de la c�mara.
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
	unsigned int index;
} C_RenderLayer;

/* Gestiona la posici�n, escala y rotaci�n de la entidad */
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

	// rotaci�n de la entidad (a favor de las agujas del reloj)
	float rotation;
} C_Transform;

/* Permite a la entidad renderizar un sprite en el mundo. */
typedef struct C_SpriteRender
{
	// Nombre del sprite que renderizar� la entidad.
	const char* spriteName;
	// Indica si el sprite es visible.
	unsigned int visible;
	// Indica la transparencia del sprite.
	float opacity; // 0.0 - 1.0
} C_SpriteRender;

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

/* En desarrollo */
typedef struct C_Behaviour
{
	void(*OnInput)(struct Game* game, struct Level* level, ecs_entity_t entity);
	void(*OnCreate)(struct Game* game, struct Level* level, ecs_entity_t entity);
	void(*OnUpdate)(struct Game* game, struct Level* level, ecs_entity_t entity);
} C_Behaviour;

/* Permite que la entidad tenga colisi�n en el mundo. */
typedef struct C_RectCollider
{
	// Desplazamiento en el eje x respecto a la posici�n de la entidad.
	float offsetX;
	// Desplazamiento en el eje y respecto a la posici�n de la entidad.
	float offsetY;
	// Ancho del rect�ngulo de colisi�n.
	unsigned int width;
	// Alto del rect�ngulo de colisi�n.
	unsigned int height;
	// Determina si la caja de colisi�n es solida.
	unsigned short isSolid;
} C_RectCollider;

typedef struct C_CircleCollider {
	// Desplazamiento en el eje x respecto a la posici�n de la entidad.
	float offsetX;
	// Desplazamiento en el eje y respecto a la posici�n de la entidad.
	float offsetY;
	// Radio del c�rculo
	float radius;
	// Determina si el c�rculo de colisi�n es solido.
	unsigned short isSolid;
};

/*********************************************************
* componentes para eljuego
**********************************************************/

/* Controla el ciclo de d�a y noche. */
typedef struct C_DayCicle
{
	// Tiempo total que dura un d�a
	unsigned int dayTime;
	// Tiempo completo actual del d�a
	unsigned int currentTime;
	// Hora actual en el juego (por d�a)
	unsigned int currentHour;
	// Minuto actual en el juego (por d�a)
	unsigned int currentMinute;
	// Factor de tiempo que tarda un minuto en pasar
} C_DayCicle;

/* Controla el rango de colecci�n de objectos del juegador. */
typedef struct C_Collector
{
	// Desplazamiento en el eje de coordenadas x
	float offsetX;
	// Desplazamiento en el eje de coordenadas y
	float offsetY;
	// Radio de acci�n para recolectar los objectos.
	float radius;
} C_Collector;

/* Permite que un NPC tenga di�logos con lo que comunicarse con el jugador */
typedef struct C_Dialog
{
	// Di�logos en memoria
	char** dialogs;
	// Maximo de interacciones que puede tener el npc
	unsigned int maxInteractions;
} C_Dialog;

/* Permite que una entidad tenga un inventario */
typedef struct C_Inventory
{
	// Cantidad m�xima de slots que tiene el inventario.
	unsigned int maxSlots;
	// Slots del inventario
	InventorySlot* slots;

	/* No aplica para el inventario del jugador */
	// Indica si el inventario est� bloqueado por un cerrojo.
	unsigned int isLocked;
	// Indica el tipo de llave que se necesita para que sea desbloqueado.
	unsigned int keyType;
} C_Inventory;

/* Permite que el jugador tenga una barra de acci�n (inventario adicional) */
typedef struct C_HotBar
{
	// Cantidad de slots de la barra de acci�n.
	unsigned int maxSlots;
	// Slots de la barra de acci�n.
	InventorySlot* slots;
} C_HotBar;

/* Componente que dota de movimiento al jugador */
typedef struct C_Movement
{
	// Direcci�n en x de la entidad (puede ser 0, 1 o -1).
	signed int directionX;
	// Direcci�n en y de la entidad (puede ser 0, 1 o -1).
	signed int directionY;
	// Velocidad de movimiento del jugador.
	float speed;
} C_Movement;

/* Componente que permite realizar una acci�n con un item en el inventario. */
typedef struct C_Action
{
	// Controlador de acciones (lo gestiona el usuario).
	void (*OnAction)(InventorySlot* selectedSlot, ecs_entity_t playerID);
} C_Action;

/* Componente para las estad�sticas del jugador */
typedef struct C_PlayerStats
{
	// Nivel m�ximo de recolecci�n.
	unsigned int maxCollectionLevel;
	// Nivel de recolecci�n actual.
	unsigned int collectionLevel;
	// Experiencia de recolecci�n actual.
	float currentCollectionLevelExp;
	// Experiencia de recolecci�n necesaria para el siguiente nivel.
	float nextCollectionLevelExp;

	// Nivel m�ximo de granjero.
	unsigned int maxFarmerLevel;
	// Nivel de granjero actual.
	unsigned int farmerLevel;
	// Experiencia de granjero actual.
	float currentFarmerLevelExp;
	// Experiencia de granjero necesaria para el siguiente nivel.
	float nextFarmerLevelExp;

	// Nivel m�ximo de pesca.
	unsigned int maxFisingLevel;
	// Nivel de pesca actual.
	unsigned int fishingLevel;
	// Experiencia de pesca actual.
	float currentFishingLevelExp;
	// Experiencia de pesca necesaria para el siguiente nivel.
	float nextFishingLevelExp;

	// Nivel m�ximo de combate.
	unsigned int maxCombatLevel;
	// Nivel de combate actual.
	unsigned int combatLevel;
	// Experiencia de combate actual.
	float currentCombatLevelExp;
	// Experiencia de combate necesaria para el siguiente nivel.
	float nextCombatLevelExp;

	// Determina si el jugador se encuentra durmiendo o no.
	unsigned int isSleeping;

	// Vida m�xima del jugador (Solo funciona en las minas).
	float maxHealth;
	// Vida actual del jugador.
	float currentHealth;

	// Energ�a m�xima del jugador (tanto para la granja como para la cueva).
	float maxStamina;
	// Energ�a actual del jugador.
	float currentStamina;
} C_PlayerStats;

/* Componente para gestionar el drop de un item en el mundo. */
typedef struct C_WorldItem
{
	unsigned int itemIndex;
	unsigned int count;
} C_WorldItem;

/* Componente que indica que la entidad es una construcci�n */
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
	BuildItem buildings;
	// Mejoras que puede realizar el contructor.
	BuildItem upgrades;

	// Construcci�n en la que se encuentra trabajando.
	BUILD_TYPE currentBuild;
	// Dias que le toma una construcci�n o mejora.
	unsigned int daysToBuild;
	// D�a actual de construcci�n.
	unsigned int currentBuildDay;
} C_Builder;

/* Componente para que la entidad pueda vender productos */
typedef struct C_Trader
{
	// Estado del comerciante
	TRADER_STATE state;
	// Productos que vende el comerciante
	TradeItem products;
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
	// D�as que tarda en madurar.
	unsigned int daysToMature;
	// D�a actual de maduraci�n.
	unsigned int currentCropDay;
	// Indica si est� lista para ser cosechada.
	unsigned int readytoHarvest;
	// Indica si est� regada.
	unsigned int isWatered;
	// Indica si est� fertilizada.
	unsigned int isFertilized;
} C_Crop;

/* Componente para los �rboles */
typedef struct C_Tree
{
	// Indica si est� talado
	unsigned int isCutDown;
	// Indica la cantidad de d�as que le toma volver a crecer
	unsigned int daysToGrowBack;
	// Indica si es de generaci�n natural o semilla
	unsigned int isNaturalGeneration;
	// Indica la cantidad de d�as que le toma crecer siendo semilla
	unsigned int dayToGrow;
	// Indica el d�a de crecimiento actual
	unsigned int currentGrowthDay;
	// Indica el estado actual del �rbol (para usarlo con SpriteRender)
	//unsigned int currentState;
} C_Tree;

/* Componente para los ores */
typedef struct C_Ore {
	unsigned int type;
} C_Ore;

#endif // !COMPONENTS_H
