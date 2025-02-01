# Changelog

Todos los cambios notables en este proyecto se documentarán en este archivo.  

El formato se basa (en su mayor parte) en [Keep a Changelog](https://keepachangelog.com/en/1.1.0/) y este proyecto se adhiere al [Semantic Versioning](https://semver.org/spec/v2.0.0.html).

## Próximos cambios.

### Añadido

- v1.1 Tipo de dato `TilesetPack` agregado. 

  ```
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
- v1.1 Función `CreateTilesetsPack` agregada.  
- v1.1 Función `AddTilesetToPack` agregada.  
- v1.1 Función `GetTilesetsPack` agregada.  
- v1.1 Función `UnloadTilesetsPack` agregada.  

### Modificado

- v1.1 Se modifica la función `GetTileset` para que ahora funcione con `TilesetPack`.
- v1.1 Se modifica la función `CreateMap` para que ahora funcione con `TilesetPack`.

### Eliminado

- v1.1 Propiedad `tiles` eliminada de `TileMap`.

### Corregido

- v1.1 Se corrige el error al momento de liberar de memoria el nombre de los sprites.