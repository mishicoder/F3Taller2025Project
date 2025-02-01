# Changelog

Cambios realizados en el proyecto para cada versión.  

## Próximos cambios.

### Añadido

- v1.1 Tipo de dato `TilesetPack` agregado.
- ```
typedef struct TilesetPack
{
	// Nombre del pack.
	char* name;
	// Lista de todos los tiles (El índice cero es NULL porque se reserva como elemento vacío).
	Tile** tiles;
	// Cantidad de tiles cargados.
	unsigned int tileCount;
	// Lista de todos los tilesets.
	Tileset** tilesets;
	// Cantidad de sets cargados.
	unsigned int tilesetsCount;
	// Índice siguiente de cada tileset.
	unsigned int nextTilesetIndex;
} TilesetPack;
```