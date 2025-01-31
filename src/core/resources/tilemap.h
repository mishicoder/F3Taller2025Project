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
	Texture2D texture; // textura del tileset (importante: no entra en el array de texturas del getor de recursos)
} Tileset;

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
	int nextTileIndex;
	// Lista de todos los tilesets
	int tilesetsCount;
	Tileset** tilesets;
	// el indice 0 es NULL
	// debido a que Tiled toma dicho índice como espacio vacío
	Tile** tiles;
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

TileMap* CreateMap(const char* name, const char* filename);
Tileset* CreateTileset(mxml_node_t* node, TileMap* map);
TiledLayer* AddLayer(mxml_node_t* data, TileMap* map);
TileMapNode* InsertTileMapNode(TileMapNode* node, TileMap* tmap);
TileMap* TreeTileMapSearch(TileMapNode* node, const char* name);
Tileset* GetTileset(TileMap* tmap, unsigned int index);

void UnloadTreeTileMapNode(TileMapNode* node);

#endif // !TILEMAP_H
