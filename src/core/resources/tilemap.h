#pragma once
#ifndef TILEMAP_H
#define TILEMAP_H

#include <stdio.h>
#include <raylib.h>
#include <mxml.h>

typedef struct Tile
{
	unsigned int x;
	unsigned int y;
	unsigned int width;
	unsigned int height;
}Tile;

typedef struct Tileset {
	unsigned int tileWidth; // ancho de cada tile
	unsigned int tileHeight; // alto de cada tile
	// para controlar que textura se tomar�
	unsigned int initialIndex;
	unsigned int finalIndex;
	// cantidad de tiles en el tileset
	unsigned int tilesCount;
	Texture2D texture; // textura del tileset (importante: no entra en el array de texturas del getor de recursos)
} Tileset;

/* Paquete de sets de mosaicos. */
typedef struct TilesetPack
{
	// Nombre del pack.
	char* name;
	// Lista de todos los tiles (El �ndice cero es NULL porque se reserva como elemento vac�o).
	Tile** tiles;
	// Cantidad de tiles cargados.
	unsigned int tileCount;
	// Lista de todos los tilesets.
	Tileset** tilesets;
	// Cantidad de sets cargados.
	unsigned int tilesetsCount;
	// �ndice siguiente de cada tileset.
	unsigned int nextTilesetIndex;
} TilesetPack;

/* Nodo para el �rbol de paquete de conjuntos de mosaicos. */
typedef struct TilesetPackNode
{
	TilesetPack* pack;
	struct TilesetPackNode* left;
	struct TilesetPackNode* right;
}TilesetPackNode;

typedef struct TiledLayer
{
	unsigned int width; // ancho de la capa (esto permite calcular el tama�o de la textura)
	unsigned int height; // alto de la capa (esto permite calcular el tama�o de la textura)
	unsigned int dataCount;
	int* data; // datos de la capa (debe ser un array de enteros ya que as� se trabaja el indexado con los arrays)
} TiledLayer;

typedef struct TileMap
{
	char* name;
	// ancho y alto de cada tile
	int tileWidth;
	int tileHeight;
	int mapWidth;
	int mapHeight;
	// Lista de todos los tilesets
	char* tilesetsPack;
	// el indice 0 es NULL
	// lista de todas las capas cargadas
	// la primera en cargar, ser� la primera en rendereizar
	// por lo que en Tiled se debe colocar bien el orden de las capas
	// seg�n como quieres que se dibujen
	int layerCount;
	TiledLayer** layers;
	RenderTexture2D mapTexture;
} TileMap;

typedef struct TileMapNode
{
	TileMap* tmap;
	struct TileMapNode* left;
	struct TileMapNode* right;
} TileMapNode;

/**
* Funcion para crear un paquete de sets de mosaicos.
* Cada set tiene su propia textura.
* 
* @param[in] name Nombre del paquete.
* @param[in] filename Ruta del arhcivo de donde se cargar�n los sets de mosaicos.
* 
* @return Retorna un TilesetPack si todo funciona correctamente, de caso contrario retorna NULL.
*/
TilesetPack* CreateTilesetsPack(const char* name, const char* filename);

/**
* Funcion para crear un set de mosaicos.
*
* @param[in] filename Ruta al set de mosaicos (.tsx).
* @param[in] initialIndex �ndice incial de los mosaicos.
*
* @return Retorna Tileset si la carga ha sido existosa, caso contrario, retorna NULL.
*/
Tileset* CreateTileset(const char* filename, unsigned int initialIndex);

/**
* Funci�n para agregar un set de mosaicos a un paquete de sets.
*
* @param[in] pack Paquete donde se agregar� el conjunto de mosaicos.
* @param[in] filename Ruta del archivo del set (.tsx).
*
* @return Retorna 1 si la carga ha tenido �xito, caso contrario, retorna 0.
*/
int InsertTilesetToPack(TilesetPack* pack, const char* filename);

/**
* Funci�n que busca un set de mosaicos en un pack a traves de un �ndice.
* 
* @param[in] pack Paquete donde se buscar� el conjunto de mosaicos.
* @param[in] index Indice del mosaico a buscar.
* 
* @return Retorna Tileset si el �ndice se encuentra en el rango de alguno de los sets del pack, caso contrario, retorna NULL.
*/
Tileset* GetTileset(TilesetPack* pack, unsigned int index);

/**
* Funcion para agregar un paquete de cojunto de mosaicos a un �rbol binario.
* 
* @param[in] node Puntero al nodo ra�z.
* @param[in] pack Paquete a agregar en el �rbol.
* 
* @return Retorna TilesetPackNode si la inserci�n ha sido exitosa, caso contrario, retorna NULL.
*/
TilesetPackNode* InsertTilesetPackNode(TilesetPackNode* node, TilesetPack* pack);

/**
* Funcion para buscar un paquete de conjunto de mosaicos en un �rbol binario.
* 
* @param[in] node Nodo ra�z de la b�squeda.
* @param[in] name Nombre del paquete que se est� buscando.
* 
* @return Retorna TilesetPack si encuentra la coincidencia, caso contrario, retorna NULL.
*/
TilesetPack* TreeTilesetPackSearch(TilesetPackNode* node, const char* name);

/**
* Funcion para crear un mapa de mosaicos.
*
* @param[in] name Nombre del mapa
* @param[in] filename Ruta al archivo del mapa (.tmx).
* @param[in] pack Paquete de sets de mosaicos que se usar� para el mapa.
*
* @return Retorna un TileMap si la carga ha sido exitosa, caso contrario, retorna NULL.
*/
TileMap* CreateMap(const char* name, const char* filename, TilesetPack* pack);

/**
* Funcion para crear una capa para un mapa.
* 
* @param[in] data Puntero al nodo de datos de la capa.
* @param[in] map Puntero al mapa del cual se extraer�n datos necesarios para la creaci�n de la capa.
* 
* @return Retorna un TiledLayer si la creaci�n ha tenido �xito, caso contrario, retorna NULL.
*/
TiledLayer* CreateLayer(mxml_node_t* data, unsigned int mapWidth, unsigned int mapHeight);

/**
* Funci�n para insertar un nodo de mapa en un �rbol binario.
* 
* @param[in] node Node ra�z donde se agregar� el nuevo nodo.
* @param[in] map Referencia del mapa que se almacenar� en el �rbol.
* 
* @return Retorna TileMapNode si la inserci�n ha tenido �xito, caso contraio, retorna NULL.
*/
TileMapNode* InsertTileMapNode(TileMapNode* node, TileMap* map);

/**
* Funci�n para buscar un mapa de mosaicos en un �rbol binario.
* 
* @param[in] node �rbol binario donde se buscar� el mapa.
* @param[in] name Nombre del mapa que se est� buscando.
* 
* @return Retorna TileMap si se encuentra en el �rbol, caso contrario, retorna NULL.
*/
TileMap* TreeTileMapSearch(TileMapNode* node, const char* name);

/**
* Des-carga de memoria un paquete de sets de mosaicos.
* 
* @param[in] pack Bloque de memoria del pack que se des-cargar�.
*/
void UnloadTilesetsPack(TilesetPack* pack);

/**
* Des-carga de memoria el �rbol de los mapas de mosaicos.
*
* @param[in] node �rbol que se des-cargar� de memoria.
*/
void UnloadTreeTilesetsPackNode(TilesetPackNode* node);

/**
* Des-carga de memoria el �rbol de los mapas de mosaicos.
* 
* @param[in] node �rbol que se des-cargar� de memoria.
*/
void UnloadTreeTileMapNode(TileMapNode* node);

#endif // !TILEMAP_H
