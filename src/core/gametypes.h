#pragma once
#ifndef GAMETYPES_H
#define GAMETYPES_H

#define HELMET_SLOT 0

typedef struct Cell {
	int isFree; // está libre?
	int isInteractable; // se puede interactuar?
	int isBlocked; // está bloqueado para el uso de A*?
} Cell;

typedef struct Circle
{
	float centerX;
	float centerY;
	float radius;
} Circle;

/* Objeto de inventario. */
typedef struct GameItem 
{
	// Nombre del sprite que renderizará el item en el inventario.
	const char* itemSprite;
	// Cantidad maxima que puede haber de este item en el slot.
	unsigned int maxStack;
	// Calidad del material (-1 si el material no posee calidad)
	signed int minQuality;
	signed int maxQuality;
	// Indica si el item se puede vender (en tienda).
	unsigned int canBeSold;
	// Indica el precio de venta del item en tienda (solo si se puede vender en tienda).
	unsigned int retailPriceInStore;
	// Indica el precio de venta del item en la caja de envíos.
	unsigned int retailPriceInShippingBox;
	// Indica el nivel del item (solo para herramientas, si no es una herramienta: -1)
	signed int toolLevel;
	// Indica el frame actual de la herramienta (afecta al sprite - siempre y cuando tenga frames)
	signed int toolLevelSpriteFrame;
} GameItem;

/* Espacio de inventario. */
typedef struct InvSlot
{
	// Indica el slot del item.
	unsigned int index;
	// Indica el índice del item que contiene el slot (-1 si no contiene ningún item).
	signed int itemIdex;
	// Indica si el slot está vacío.
	signed int isEmpty;
	// Indica la cantidad del item que contiene el slot.
	unsigned int inStack;
} InventorySlot;

/* Tipo de construcciones disponibles en el juego. */
typedef enum BUILD_TYPE
{
	BUILD_NONE = -1,
	// Contrucción->Silo
	BUILD_SILO = 0,
	// Contrucción->Gallinero
	BUILD_COOP,
	// Contrucción->Granero
	BUILD_BARN,
	// Contrucción->Pozo de agua
	BUILD_WELL,
	// Contrucción->Caja de envíos
	BUILD_SHIPPING_BIN,
	// Contrucción->Casa del jugador
	BUILD_PLAYER_HOUSE
} BUILD_TYPE;

/* Estados del constructor */
typedef enum BUILDER_STATE
{
	BUILDER_AVAILABLE,
	BUILDER_BUILDING,
	BUILDER_FREE_DAY
} BUILDER_STATE;

/* Objecto de venta para construcción */
typedef struct BuildItem
{
	// Tipo de construcción que se realizará
	BUILD_TYPE type;
	// precio de la estructura
	unsigned int price;
	// Cantidad de madera que se requiere para la construcción
	unsigned int woodQuantity;
	// Cantidad de roca que se requiere para la construcción
	unsigned int rockQuantity;
	// Cantidad de carbón que se requiere para la construcción
	unsigned int coalQuantity;
	// Cantidad de oro que se requiere para la construcción
	unsigned int goldQuantity;
	// Cantidad de plata que se requiere para la construcción
	unsigned int silverQuantity;
	// Cantidad de diamante que se requiere para la construcción
	unsigned int diamondQuantity;
} BuildItem;

/* Estado del comerciante */
typedef enum TRADER_STATE
{
	TRADER_AVAILABLE,
	TRADER_FREE_DAY
} TRADER_STATE;

/* Objeto de venta para el comerciante. */
typedef struct TradeItem
{
	unsigned int itemIndex;
	unsigned int amount;
	unsigned int price;
} TradeItem;

/* Estado de la tierra */
typedef enum FARMLAND_STATE
{
	// Construcción encima
	BUILDING_ABOVE,
	// Semilla encima
	SEED_ABOVE,
	// Material encima
	MATERIAL_ABOVE,
	// Con árbil encima
	TREE_ABOVE,
	// Con pasto
	WITH_GRASS,
	// Disponible
	FARMLAND_AVAILABLE
} FARMLAND_STATE;

/* Tipos de semillas */
typedef enum CROP_TYPE
{

	/* Verano */
	// Zanahoria
	CROP_CARROT,
	// Coliflor
	CROP_CAULIFLOWER,
	// Chirivía
	CROP_PARSNIP,
	// Papa
	CROP_POTATO,

	/* Primavera */
	// Calabaza
	CROP_PUMPKIN,
	// Girasol
	CROP_SUNFLOWER,
	// Trigo
	CROP_WHEAT,
	// Remolacha
	CROP_BEETROOT,

	/* Otoño */
	// Col
	CROP_CABBAGE,
	// Col rizada
	CROP_KALE,
	// Rábano
	CROP_RADISH
} CROP_TYPE;

#endif // !GAMETYPES_H
