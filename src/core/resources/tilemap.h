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
	// para controlar que textura se tomará
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
	// Lista de todos los tiles (El índice cero es NULL porque se reserva como elemento vacío).
	Tile* tiles;
	// Cantidad de tiles cargados.
	unsigned int tileCount;
	// Lista de todos los tilesets.
	Tileset** tilesets;
	// Cantidad de sets cargados.
	unsigned int tilesetsCount;
	// Índice siguiente de cada tileset.
	unsigned int nextTilesetIndex;
} TilesetPack;

/* Nodo para el árbol de paquete de conjuntos de mosaicos. */
typedef struct TilesetPackNode
{
	TilesetPack* pack;
	struct TilesetPackNode* left;
	struct TilesetPackNode* right;
}TilesetPackNode;

typedef struct TiledLayer
{
	unsigned int width; // ancho de la capa (esto permite calcular el tamaño de la textura)
	unsigned int height; // alto de la capa (esto permite calcular el tamaño de la textura)
	unsigned int dataCount;
	unsigned int* data; // datos de la capa (debe ser un array de enteros ya que así se trabaja el indexado con los arrays)
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
	// la primera en cargar, será la primera en rendereizar
	// por lo que en Tiled se debe colocar bien el orden de las capas
	// según como quieres que se dibujen
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
* @param[in] filename Ruta del arhcivo de donde se cargarán los sets de mosaicos.
* 
* @return Retorna un TilesetPack si todo funciona correctamente, de caso contrario retorna NULL.
*/
TilesetPack* CreateTilesetsPack(const char* name, const char* filename);

/**
* Función que retorna un paquete de sets de mosaicos.
*
* @param[in] name Nombre del paquete de sets de mosaicos.
*
* @return Retorna TilesetPack si se encuentra, caso contrario, retorna NULL.
*/
TilesetPack* GetTilesetsPack(const char* name); // Funcion en el punto equivocado XD

/**
* Funcion para crear un set de mosaicos.
*
* @param[in] filename Ruta al set de mosaicos (.tsx).
* @param[in] initialIndex Índice incial de los mosaicos.
*
* @return Retorna Tileset si la carga ha sido existosa, caso contrario, retorna NULL.
*/
Tileset* CreateTileset(const char* filename, unsigned int initialIndex);

/**
* Función para agregar un set de mosaicos a un paquete de sets.
*
* @param[in] pack Paquete donde se agregará el conjunto de mosaicos.
* @param[in] filename Ruta del archivo del set (.tsx).
*
* @return Retorna 1 si la carga ha tenido éxito, caso contrario, retorna 0.
*/
int InsertTilesetToPack(TilesetPack* pack, const char* filename);

/**
* Función que busca un set de mosaicos en un pack a traves de un índice.
* 
* @param[in] pack Paquete donde se buscará el conjunto de mosaicos.
* @param[in] index Indice del mosaico a buscar.
* 
* @return Retorna Tileset si el índice se encuentra en el rango de alguno de los sets del pack, caso contrario, retorna NULL.
*/
Tileset* GetTileset(TilesetPack* pack, unsigned int index);

/**
* Funcion para agregar un paquete de cojunto de mosaicos a un árbol binario.
* 
* @param[in] node Puntero al nodo raíz.
* @param[in] pack Paquete a agregar en el árbol.
* 
* @return Retorna TilesetPackNode si la inserción ha sido exitosa, caso contrario, retorna NULL.
*/
TilesetPackNode* InsertTilesetPackNode(TilesetPackNode* node, TilesetPack* pack);

/**
* Funcion para buscar un paquete de conjunto de mosaicos en un árbol binario.
* 
* @param[in] node Nodo raíz de la búsqueda.
* @param[in] name Nombre del paquete que se está buscando.
* 
* @return Retorna TilesetPack si encuentra la coincidencia, caso contrario, retorna NULL.
*/
TilesetPack* TreeTilesetPackSearch(TilesetPackNode* node, const char* name);

/**
* Funcion para crear un mapa de mosaicos.
*
* @param[in] name Nombre del mapa
* @param[in] filename Ruta al archivo del mapa (.tmx).
* @param[in] pack Paquete de sets de mosaicos que se usará para el mapa.
*
* @return Retorna un TileMap si la carga ha sido exitosa, caso contrario, retorna NULL.
*/
TileMap* CreateMap(const char* name, const char* filename, TilesetPack* pack);

/**
* Funcion para crear una capa para un mapa.
* 
* @param[in] data Puntero al nodo de datos de la capa.
* @param[in] map Puntero al mapa del cual se extraerán datos necesarios para la creación de la capa.
* 
* @return Retorna un TiledLayer si la creación ha tenido éxito, caso contrario, retorna NULL.
*/
TiledLayer* AddLayer(mxml_node_t* data, TileMap* map);

/**
* Función para insertar un nodo de mapa en un árbol binario.
* 
* @param[in] node Node raíz donde se agregará el nuevo nodo.
* @param[in] map Referencia del mapa que se almacenará en el árbol.
* 
* @return Retorna TileMapNode si la inserción ha tenido éxito, caso contraio, retorna NULL.
*/
TileMapNode* InsertTileMapNode(TileMapNode* node, TileMap* map);

/**
* Función para buscar un mapa de mosaicos en un árbol binario.
* 
* @param[in] node Árbol binario donde se buscará el mapa.
* @param[in] name Nombre del mapa que se está buscando.
* 
* @return Retorna TileMap si se encuentra en el árbol, caso contrario, retorna NULL.
*/
TileMap* TreeTileMapSearch(TileMapNode* node, const char* name);

/**
* Des-carga de memoria un paquete de sets de mosaicos.
* 
* @param[in] pack Bloque de memoria del pack que se des-cargará.
*/
void UnloadTilesetsPack(TilesetPack* pack);

/**
* Des-carga de memoria el árbol de los mapas de mosaicos.
* 
* @param[in] node Árbol que se des-cargará de memoria.
*/
void UnloadTreeTileMapNode(TileMapNode* node);

#endif // !TILEMAP_H
