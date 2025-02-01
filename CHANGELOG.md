# Changelog

Cambios realizados en el proyecto para cada versi�n.  

## Pr�ximos cambios.

### A�adido

- v1.1 Tipo de dato `TilesetPack` agregado.
- ```
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
```