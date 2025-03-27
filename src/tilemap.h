#pragma once
#ifndef TILEMAP_H

#include <stdio.h>
#include <stdbool.h>
#include <raylib.h>
#include <mxml.h>

typedef struct
{
  int x;
  int y;
  int width;
  int height;
} Tile;

typedef struct
{
  int tileWidth;
  int tileHeight;
  int initialIndex;
  int finalIndex;
  int tileCount;
  Texture2D texture;
} Tileset;

typedef struct
{
  char* name;
  Tile** tiles;
  int tileCount;
  Tileset** tilesets;
  int tilesetsCount;
  int nextTilesetIndex;
} TilesetPack;

typedef struct TilesetPackNode
{
  TilesetPack* pack;
  struct TilesetPackNode* left;
  struct TilesetPackNode* right;
}TilesetPackNode;

typedef struct
{
  int width;
  int height;
  int dataCount;
  int* data;
} MapLayer;

typedef struct
{
  char* name;
  int tileWidth;
  int tileHeight;
  int mapWidth;
  int mapHeight;
  char* tilesetPack;
  int layersCount;
  MapLayer** layers;
  RenderTexture2D texture;
} TileMap;

typedef struct TileMapNode
{
  TileMap* map;
  struct TileMapNode* left;
  struct TileMapNode* right;
} TileMapNode;

/**
 * @brief Creaa un paquete de conjuntos de mosaicos y lo almacena en memoria.
 *
 * @param name Nombre del paquete.
 * @param filename Ruta donde se encuentra el archivo .pack.
 *
 * @return Retorna <TilesetPack*> si la carga ha tenido éxito, caso contrario retorna NULL.
*/
TilesetPack* CreateTilesetPack(const char* name, const char* filename);

/**
 * @brief Agrega un conjunto de mosaicos a un paquete ya existente.
 *
 * @param pack Referencia al paquete donde se agregará el conjunto.
 * @param filename Ruta donde se encuentra el archivo .tsx.
 *
 * @return Retorna true si la carga ha tenido éxito, caso contrario retorna false.
*/
bool AddTilesetToPack(TilesetPack* pack, const char* filename);

/**
 * @brief Agrega un paquete de conjuntos de mosaicos a un árbol.
 *
 * @param node Nodo raíz donde se agregará el nuevo nodo.
 * @param pack Paquete de conjuntos de mosaicos a enlazar con el nodo.
 *
 * @return Retorna <TilesetPackNode*> si la operación ha sido exitosa, caso contrario retorna NULL.
*/
TilesetPackNode* InsertTilesetPackNode(TilesetPackNode* node, TilesetPack* pack);

/**
 * @brief Busca un paquete de conjuntos de mosaicos en un árbol.
 *
 * @param node Nodo desde donde se iniciará la búsqueda.
 * @param name Nombre del paquete a buscar.
 *
 * @return Retorna <TilesetPack*> si se encuentra, caso contrario retorna NULL.
*/
TilesetPack* SearchTilesetPackInTree(TilesetPackNode* node, const char* name);

/**
 * @brief Crea un conjunto de mosaicos.
 *
 * @param filename Ruta del archivo .tsx.
 * @param initialIndex Índice inicial de los mosaicos.
 *
 * @return Retorna <Tileset*> si la carga ha tenido éxito, caso contrario retorna NULL.
*/
Tileset* CreateTileset(const char* filename, int initialIndex);

/**
 * @brief Retorna un conjunto de mosaicos de un paquete a través de un índice.
 *
 * @param pack Paquete en el que se buscará el conjunto de mosaicos.
 * @param index Índice del conjunto a buscar.
 *
 * @return Retorna <Tileset*> si encuentra el conjunto de mosaicos en el paquete, caso contrario retorna NULL.
*/
Tileset* GetTileset(TilesetPack* pack, int index);

/**
 * @brief Crea una para un mapa de mosaicos.
 *
 * @param data Node de los datos numéricos de la capa.
 * @param mapWidth Ancho del mapa.
 * @param mapHeight Alto del mapa.
 *
 * @return Retorna <MapLayer*> si la carga ha tenido éxito, caso contrario retorna NULL.
*/
MapLayer* CreateLayer(mxml_node_t* data, int mapWidth, int mapHeight);

/**
 * @brief Crear una mapa de mosaicos.
 *
 * @param name Nombre del mapa.
 * @param filename Ruta del archivo .tmx del mapa.
 * @param pack Paquede de conjuntos de mosaicos que usará el mapa.
 *
 * @return Retorna <TileMap*> si la carga ha tenido éxito, caso contrario retorna NULL.
*/
TileMap* CreateMap(const char* name, const char* filename, TilesetPack* pack);

/**
 * @brief Agrega un mapa de mosaicos a un árbol.
 *
 * @param node Punto de inserción del nuevo nodo.
 * @param map Mapara a enlazar con el nuevo nodo.
 *
 * @return Retorna <TileMapNode*> si se ha agregado correctamente, caso contrario reetorna NULL.
*/
TileMapNode* InsertTileMapNode(TileMapNode* node, TileMap* map);

/**
 * @brief Busca un mapa de mosaicos en un árbol.
 *
 * @param nodo Nodo desde el cuál se iniciará la búsqueda.
 * @param name Nombre del mapa que se está buscando.
 *
 * @return Retorna <TileMap*> si se encuentra el mapa, caso contrario retorna NULL.
*/
TileMap* SearchTileMapInTree(TileMapNode* node, const char* name);

/**
 * @brief Elimina de memoria un paquete de conjuntos de mosaicos.
 *
 * @param pack Paquete a eliminar.
*/
void UnloadTilesetPack(TilesetPack* pack);

/**
 * @brief Elimina el árbol de los paquetes de conjuntos de mosaicos.
 *
 * @param nodo Nodo desde el cual se inicia la eliminación.
*/
void UnloadTilesetPackNode(TilesetPackNode* node);

/**
 * @brief Elimina el árbol de los mapas de mosaicos.
 *
 * @param node Nodo desde el cual se inicia la eliminación.
*/
void UnloadTileMapNode(TileMapNode* node);

#endif // !TILEMAP_H
