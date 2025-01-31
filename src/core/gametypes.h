#pragma once
#ifndef GAMETYPES_H
#define GAMETYPES_H

/* Objeto de inventario. */
typedef struct GameItem 
{
	// Nombre del sprite que renderizar� el item en el inventario.
	const char* itemSprite;
	// Cantidad actual del item en el inventario.
	unsigned int inStack;
	// Cantidad maxima que puede haber de este item en el slot.
	unsigned int maxStack;
	// Calidad del material (-1 si el material no posee calidad)
	signed int quality;
	// Indica si el item se puede vender (en tienda).
	unsigned int canBeSold;
	// Indica el precio de venta del item en tienda (solo si se puede vender en tienda).
	unsigned int retailPriceInStore;
	// Indica el precio de venta del item en la caja de env�os.
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
	// Indica el �ndice del item que contiene el slot (-1 si no contiene ning�n item).
	signed int itemIdex;
	// Indica si el slot est� vac�o.
	signed int isEmpty;
	// Indica la cantidad del item que contiene el slot.
	unsigned int inStack;
} InventorySlot;

/* Tipo de construcciones disponibles en el juego. */
typedef enum BUILD_TYPE
{
	// Contrucci�n->Silo
	BUILD_SILO = 0,
	// Contrucci�n->Gallinero
	BUILD_COOP,
	// Contrucci�n->Granero
	BUILD_BARN,
	// Contrucci�n->Pozo de agua
	BUILD_WELL,
	// Contrucci�n->Caja de env�os
	BUILD_SHIPPING_BIN,
	// Contrucci�n->Casa del jugador
	BUILD_PLAYER_HOUSE
} BUILD_TYPE;

/* Estados del constructor */
typedef enum BUILDER_STATE
{
	BUILDER_AVAILABLE,
	BUILDER_BUILDING,
	BUILDER_FREE_DAY
} BUILDER_STATE;

/* Objecto de venta para construcci�n */
typedef struct BuildItem
{
	// Tipo de construcci�n que se realizar�
	BUILD_TYPE type;
	// Cantidad de madera que se requiere para la construcci�n
	unsigned int woodQuantity;
	// Cantidad de roca que se requiere para la construcci�n
	unsigned int rockQuantity;
	// Cantidad de carb�n que se requiere para la construcci�n
	unsigned int coalQuantity;
	// Cantidad de oro que se requiere para la construcci�n
	unsigned int goldQuantity;
	// Cantidad de plata que se requiere para la construcci�n
	unsigned int silverQuantity;
	// Cantidad de diamante que se requiere para la construcci�n
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
} TradeItem;

/* Estado de la tierra */
typedef enum FARMLAND_STATE
{
	// Construcci�n encima
	BUILDING_ABOVE,
	// Semilla encima
	SEED_ABOVE,
	// Material encima
	MATERIAL_ABOVE,
	// Con �rbil encima
	TREE_ABOVE,
	// Con pasto
	WITH_GRASS,
	// Disponible
	FARMLAND_AVAILABLE
} FARMLAND_STATE;

/* Tipos de semillas */
typedef enum CROP_TYPE
{
	// Zanahoria
	CROP_CARROT,
	// Coliflor
	CROP_CAULIFLOWER,
	// Chiriv�a
	CROP_PARSNIP,
	// Calabaza
	CROP_PUMPKIN,
	// Papa
	CROP_POTATO,
	// Girasol
	CROP_SUNFLOWER,
	// Trigo
	CROP_WHEAT,
	// Remolacha
	CROP_BEETROOT,
	// Col
	CROP_CABBAGE,
	// Col rizada
	CROP_KALE,
	// R�bano
	CROP_RADISH
} CROP_TYPE;

#endif // !GAMETYPES_H
