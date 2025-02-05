# Changelog

Todos los cambios notables en este proyecto se documentar�n en este archivo.  

El formato se basa (en su mayor parte) en [Keep a Changelog](https://keepachangelog.com/en/1.1.0/) y este proyecto se adhiere al [Semantic Versioning](https://semver.org/spec/v2.0.0.html).

## [Pr�ximos Cambios] "�Aliens?"

### A�adido

- v1.2 Se agrega la funci�n `AddEntityBehaviour` para agregar el componente de comportamiento a las entidades.
- v1.2 Se agrega la funci�n `AddComponentToEntity` para agregar un componente a una entidad usando literales.  
- v1.2 Se agrega la funci�n `PopLevel` para sacar del stack el �ltimo nivel agregado.   
- v1.2 Se agrega la funci�n `AddLevelToCacheStack` para guardar el nivel en memoria.   
- v1.2 Se agrega la funci�n `PushMemoryLevel` para agregar un nivel guardado en memoria al stack.   
- v1.2 Se agrega la funci�n `PushLevel` para agregar un nivel al stack.   
- v1.2 Se agrega la funci�n `LoadLevel` para cargar un nivel, reemplazando al actual.   
- v1.2 Se agrega la propiedad `Load` al elemento `GameLevel` para que sea manejado por el usuario.   
- v1.2 Se agrega la propiedad `activeDebug` a `GameConfig` para indicar si se quiere activar el debug de las colisiones y otros elementos.   
- v1.2 Se agrega la funci�n `IntersectionRectangleRectangleCollisionImplementation` para evitar el tunelado (costo medio).   
- v1.2 Se agrega la funci�n `IntersectionCircleRectCollisionImplementation` para evitar el tunelado (costo medio).   
- v1.2 Se agrega la funci�n `ResolveRectangleCircleCollision` para resolver colisiones entre rect�ngulos y c�rculos.   
- v1.2 Se agrega la funci�n `ResolveRectRectCollision` para resolver colisiones entre rect�ngulos.   
- v1.2 Se agrega la funci�n `GetCollisionRectangleRectangle` para encontrar el rect�ngulo de intersecci�n entre dos rect�ngulos.   
- v1.2 Se agrega la propiedad `isSolid` a los componentes de colisi�n.   
- v1.2 Se agrega la funci�n `LoadResources` como propiedad a la estructura `Game`.   
- v1.2 Se agrega el component `C_CircleCollider` para manejar colisiones circulares.   
- v1.2 Se agrega la funci�n `CreateBlankEntity` para crear una entidad que solo tiene los componente `C_Info` y `C_Transform`.  

### Obsoleto

- v1.2 La funci�n `AddLevel` queda obsoleta y ser� eliminada en la siguiente versi�n, ahora se usar�n las funciones `PushLevel`, `PushMemoryLevel`, `LoadLevel` o `LoadLevelF`.  

### Eliminado

- v1.2 Se elimina la funci�n `Run` que se encarba de realizar la actualizaci�n y renderizado de objetos de forma manual.   
- v1.2 Se elimina la funci�n `DefineComponents` debido al m�todo de gesti�n de componentes de Flecs.   

### Corregido

- v1.2 Se corrige el error del nombre de `C_Transform` que anteriormente era `C_Transfom`.  

## [1.1] "Explorador" - 2025-02-02

### A�adido

- v1.1 Se agrega la funci�n `SetGameWindowIcon` para establece un �cono para la ventana.
- v1.1 Se agrega la propiedad `tilesetsPacksNode` al gestor de recursos para manejar los paquetes de conjuntos de mosaicos.
- v1.1 Tipo de dato `TilesetPack` a�adido.  
- v1.1 Funci�n `CreateTilesetsPack` agregada.  
- v1.1 Funci�n `AddTilesetToPack` agregada.  
- v1.1 Funci�n `GetTilesetsPack` agregada.  
- v1.1 Funci�n `UnloadTilesetsPack` agregada.  
- v1.1 Se agrega el componente `C_Ore` para el control de materiales.  
- v1.1 Se agrega el componente `C_Tree` para el control de los �rboles.  
- v1.1 Se agrega el componente `C_Crop` para el control de cultivos.  
- v1.1 Se agrega el componente `C_FramLand` para el control de la tierra de la granja.   
- v1.1 Se agrega el componente `C_DropTable` para indicar que objetos puede arrojar una entidad al ser destruida.   
- v1.1 Se agrega el componente `C_Trader` para permitir que un NPC sea comerciante.   
- v1.1 Se agrega el componente `C_Builder` para permitir que un NPC sea contructor.   
- v1.1 Se agrega el componente `C_Build` para el control de las contrucciones en el mundo.   
- v1.1 Se agrega el componente `C_WorldItem` para controlar los objetos arrojados en el mundo.   
- v1.1 Se agrega el componente `C_PlayerStats` para las car�cter�sticas del jugador.   
- v1.1 Se agrega el componente `C_Action` para definir las acciones que puede realizar un jugador con alg�n objeto.   
- v1.1 Se agrega el componente `C_Movement` para el control de movimiento del jugador.   
- v1.1 Se agrega el componente `C_HotBar` para el control de una barra de acci�n para el jugador.  
- v1.1 Se agrega el componente `C_Invetory` para el control de inventario de las entidades.  
- v1.1 Se agrega el componente `C_Dialog` para los di�logos de los NPC's.  
- v1.1 Se agrega el componente `C_Collector` para permitir al jugador recolectar objetos del mundo.  
- v1.1 Se agrega el componente `C_DayCicle` para el control de tiempo del juego.  
- v1.1 Se agrega el componente `C_RectCollider` para el gesti�n de colisiones.  
- v1.1 Se agrega el componente `C_Behaviour` para el manejo personalizado en una entidad.   
- v1.1 Se agrega el componente `C_MapRender` para el manejo de renderizado y l�mites de un mapa de mosaicos.   
- v1.1 Se agrega el componente `C_Color` para el manejo del color de un sprite.   
- v1.1 Se agrega el componente `C_SpriteRender` para el manejo de renderizado de sprites.   
- v1.1 Se agrega el componente `C_Transform` para la transformaci�n de las entidades.   
- v1.1 Se agrega el componente `C_RenderLayer` para el manejo de capas de renderizado.  
- v1.1 Se agrega el componente `C_Info` para la informaci�n de las entidades.  
- v1.1 Se agrega el componente `C_Camera2D` para el manejo de m�ltiples c�maras y la c�mara principal.   
- v1.1 Se agrega el tipo de dato `GameItem` para manejar los objetos de inventario del juego.  
- v1.1 Se agrega el tipo de dato `InventorySlot` para manejar los espacios de un inventario.  
- v1.1 Se agrega el tipo de dato `BUILD_TYPE` para los tipos de construcciones disponibles en el juego.   
- v1.1 Se agrega el tipo de dato `BUILDER_STATE` para manejar los estados del NPC constructor.   
- v1.1 Se agrega el tipo de dato `BuildItem` para manejar los elementos de comercio del cosntructor.   
- v1.1 Se agrega el tipo de dato `TRADER_STATE` para manejar los estados del NPC comerciante.   
- v1.1 Se agrega el tipo de dato `TraderItem` para manejar los elementos de comercio del comerciante.   
- v1.1 Se agrega el tipo de dato `FARMLAND_STATE` para manejar los estados de la tierra de la granja.   
- v1.1 Se agrega el tipo de dato `CROP_TYPE` para manejar los tipos de cultivos disponibles en el juego.   

### Modificado

- v1.1 Se modifica la funci�n `GetTileset` para que ahora funcione con `TilesetPack`.   
- v1.1 Se modifica la funci�n `CreateMap` para que ahora funcione con `TilesetPack`.   
- v1.1 El tipo de dato `Sprite` se movi� a un archivo independiente.   
- v1.1 El tipo de dato `TileMap` se movi� a un archivo independiente.   
- v1.1 Ahora los tieleset se manejan por paquete y se define un �rbol binario para dichos paquetes.   

### Eliminado

- v1.1 Propiedad `tiles` eliminada de `TileMap`, ahora el mapa de mosaicos ya no maneja los tiles.   
- v1.1 Propiedad `tilesets` elimina de `TileMap`, ahora el mapa de mosaicos ya no maneja los tilesets.   
- v1.1 Se elimina la funci�n `LoadSpriteAtlasWithOption` por problemas de carga (se re-implementar� en una futura versi�n).   

### Corregido

- v1.1 Se corrige el error al momento de liberar de memoria el nombre de los sprites.  
- v1.1 Se corrige el error al momento de cargar un paquete de conjuntos de mosaicos.   
- v1.1 Se corrige el error al cargar y dibujar el mapa de mosaicos haciendo uso de `TilesetPack`.   
- v1.1 Se corrige el error que no permitia borra la memoria de `TilesetPackNode` y `TileMapNode`.   

## [1.0] "Kikiriki" - 2025-01-01

### A�adido  

- Se agreg� la funci�n `CreateTileMap` para cargar un mapa y los tilesets desde un archivo "tmx".  
- Se agreg� el tipo `TileMap` para cargar un mapa de mosaicos desde Tiled.   
- Se agreg� el tipo de dato `Tile` para gestionar los mosaicos de los tilesets.   
- Se agreg� el tipo de dato `Tileset` para cargar tilesets con datos de Tiled.   
- Se agreg� el tipo de dato `TiledLayer` para cargar los datos de las capas de Tiled.   
- Se agreg� la funci�n `LoadTilemap` para simplificar la carga de un mapa.   
- Se agreg� la funci�n `FreeGame` para eliminar de memoria una instancia de juego.   
- Se agreg� la funci�n `AddLevel` para agregar un nivel a un juego.   
- Se agreg� la funci�n `UnloadLevel` para eliminar un nivel de memoria.
- Se agreg� la funci�n `CreateLevel` para crear un nivel.
- Se agreg� el tipo de dato `GameLevel` para definir un nivel y sus propiedades.   
- Se agreg� la funci�n `LoadSpriteAtlasWithOptions` para cargar una colecci�n de sprites con opciones.   
- Se agreg� la funci�n `LoadSpriteAtlas` para cargar una colecci�n de sprites.  
- Se agreg� la funci�n `LoadSpriteWithOptions` para cargar un sprite con opciones.  
- Se agreg� la funci�n `LoadSprite` para simplificar la carga de un sprite.  
- Se agreg� la funci�n `RunGame` para la ejecuci�n de un juego.  
- Se agreg� la funci�n `InitGame` para inicializar una instancia de juego.  
- Se agreg� el tipo de dato `Game` para el control de un juego.    
- Se agreg� el tipo de dato `GameConfig` para las configuraciones del juego.  
- Se agreg� la funci�n `UnloadResourcesManager` para eliminar de memoria los recursos del gestor.  
- Se agreg� la funci�n `UnloadTreeSpriteNode` para eliminar de memoria el �rbol de sprites.  
- Se agreg� la funci�n `GetSpriteAnimation` para devolver los datos de animaci�n de un sprite.  
- Se agreg� la funci�n `GetSpriteFrame` para devolver el frame de un sprite siempre y cuando lo tenga.  
- Se agreg� la funci�n `GetSprite` para devolver un sprite.  
- Se agreg� la funci�n `AddSprite` para agregar un sprite al gestor de recursos.  
- Se agreg� la funci�n `AddTexture` para cargar y agregar una textura al gestor de recursos.  
- Se agreg� la funci�n `InitResourcesManager` para inicializar el gestor de recursos.  
- Se agreg� el tipo de dato `ResourcesManager` para la gesti�n de los recursos. 
- Se agreg� el tipo de dato `Sprite Animation` para definir la animaci�n de un sprite.
- Se agreg� el tipo de dato `SpriteFrame` para definir el frame de un sprite.
- Se agreg� el tipo de dato `Sprite` para definir un sprite con sus propiedades.
- Se agreg� el tipo de dato `TextureResult` para la carga de texturas.  
