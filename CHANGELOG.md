# Changelog

Todos los cambios notables en este proyecto se documentarán en este archivo.  

El formato se basa (en su mayor parte) en [Keep a Changelog](https://keepachangelog.com/en/1.1.0/) y este proyecto se adhiere al [Semantic Versioning](https://semver.org/spec/v2.0.0.html).

## [Próximos Cambios] "¿Aliens?"

### Añadido

- v1.2 Se agrega la función `AddEntityBehaviour` para agregar el componente de comportamiento a las entidades.
- v1.2 Se agrega la función `AddComponentToEntity` para agregar un componente a una entidad usando literales.  
- v1.2 Se agrega la función `PopLevel` para sacar del stack el último nivel agregado.   
- v1.2 Se agrega la función `AddLevelToCacheStack` para guardar el nivel en memoria.   
- v1.2 Se agrega la función `PushMemoryLevel` para agregar un nivel guardado en memoria al stack.   
- v1.2 Se agrega la función `PushLevel` para agregar un nivel al stack.   
- v1.2 Se agrega la función `LoadLevel` para cargar un nivel, reemplazando al actual.   
- v1.2 Se agrega la propiedad `Load` al elemento `GameLevel` para que sea manejado por el usuario.   
- v1.2 Se agrega la propiedad `activeDebug` a `GameConfig` para indicar si se quiere activar el debug de las colisiones y otros elementos.   
- v1.2 Se agrega la función `IntersectionRectangleRectangleCollisionImplementation` para evitar el tunelado (costo medio).   
- v1.2 Se agrega la función `IntersectionCircleRectCollisionImplementation` para evitar el tunelado (costo medio).   
- v1.2 Se agrega la función `ResolveRectangleCircleCollision` para resolver colisiones entre rectángulos y círculos.   
- v1.2 Se agrega la función `ResolveRectRectCollision` para resolver colisiones entre rectángulos.   
- v1.2 Se agrega la función `GetCollisionRectangleRectangle` para encontrar el rectángulo de intersección entre dos rectángulos.   
- v1.2 Se agrega la propiedad `isSolid` a los componentes de colisión.   
- v1.2 Se agrega la función `LoadResources` como propiedad a la estructura `Game`.   
- v1.2 Se agrega el component `C_CircleCollider` para manejar colisiones circulares.   
- v1.2 Se agrega la función `CreateBlankEntity` para crear una entidad que solo tiene los componente `C_Info` y `C_Transform`.  

### Obsoleto

- v1.2 La función `AddLevel` queda obsoleta y será eliminada en la siguiente versión, ahora se usarán las funciones `PushLevel`, `PushMemoryLevel`, `LoadLevel` o `LoadLevelF`.  

### Eliminado

- v1.2 Se elimina la función `Run` que se encarba de realizar la actualización y renderizado de objetos de forma manual.   
- v1.2 Se elimina la función `DefineComponents` debido al método de gestión de componentes de Flecs.   

### Corregido

- v1.2 Se corrige el error del nombre de `C_Transform` que anteriormente era `C_Transfom`.  

## [1.1] "Explorador" - 2025-02-02

### Añadido

- v1.1 Se agrega la función `SetGameWindowIcon` para establece un ícono para la ventana.
- v1.1 Se agrega la propiedad `tilesetsPacksNode` al gestor de recursos para manejar los paquetes de conjuntos de mosaicos.
- v1.1 Tipo de dato `TilesetPack` añadido.  
- v1.1 Función `CreateTilesetsPack` agregada.  
- v1.1 Función `AddTilesetToPack` agregada.  
- v1.1 Función `GetTilesetsPack` agregada.  
- v1.1 Función `UnloadTilesetsPack` agregada.  
- v1.1 Se agrega el componente `C_Ore` para el control de materiales.  
- v1.1 Se agrega el componente `C_Tree` para el control de los árboles.  
- v1.1 Se agrega el componente `C_Crop` para el control de cultivos.  
- v1.1 Se agrega el componente `C_FramLand` para el control de la tierra de la granja.   
- v1.1 Se agrega el componente `C_DropTable` para indicar que objetos puede arrojar una entidad al ser destruida.   
- v1.1 Se agrega el componente `C_Trader` para permitir que un NPC sea comerciante.   
- v1.1 Se agrega el componente `C_Builder` para permitir que un NPC sea contructor.   
- v1.1 Se agrega el componente `C_Build` para el control de las contrucciones en el mundo.   
- v1.1 Se agrega el componente `C_WorldItem` para controlar los objetos arrojados en el mundo.   
- v1.1 Se agrega el componente `C_PlayerStats` para las carácterísticas del jugador.   
- v1.1 Se agrega el componente `C_Action` para definir las acciones que puede realizar un jugador con algún objeto.   
- v1.1 Se agrega el componente `C_Movement` para el control de movimiento del jugador.   
- v1.1 Se agrega el componente `C_HotBar` para el control de una barra de acción para el jugador.  
- v1.1 Se agrega el componente `C_Invetory` para el control de inventario de las entidades.  
- v1.1 Se agrega el componente `C_Dialog` para los diálogos de los NPC's.  
- v1.1 Se agrega el componente `C_Collector` para permitir al jugador recolectar objetos del mundo.  
- v1.1 Se agrega el componente `C_DayCicle` para el control de tiempo del juego.  
- v1.1 Se agrega el componente `C_RectCollider` para el gestión de colisiones.  
- v1.1 Se agrega el componente `C_Behaviour` para el manejo personalizado en una entidad.   
- v1.1 Se agrega el componente `C_MapRender` para el manejo de renderizado y límites de un mapa de mosaicos.   
- v1.1 Se agrega el componente `C_Color` para el manejo del color de un sprite.   
- v1.1 Se agrega el componente `C_SpriteRender` para el manejo de renderizado de sprites.   
- v1.1 Se agrega el componente `C_Transform` para la transformación de las entidades.   
- v1.1 Se agrega el componente `C_RenderLayer` para el manejo de capas de renderizado.  
- v1.1 Se agrega el componente `C_Info` para la información de las entidades.  
- v1.1 Se agrega el componente `C_Camera2D` para el manejo de múltiples cámaras y la cámara principal.   
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

- v1.1 Se modifica la función `GetTileset` para que ahora funcione con `TilesetPack`.   
- v1.1 Se modifica la función `CreateMap` para que ahora funcione con `TilesetPack`.   
- v1.1 El tipo de dato `Sprite` se movió a un archivo independiente.   
- v1.1 El tipo de dato `TileMap` se movió a un archivo independiente.   
- v1.1 Ahora los tieleset se manejan por paquete y se define un árbol binario para dichos paquetes.   

### Eliminado

- v1.1 Propiedad `tiles` eliminada de `TileMap`, ahora el mapa de mosaicos ya no maneja los tiles.   
- v1.1 Propiedad `tilesets` elimina de `TileMap`, ahora el mapa de mosaicos ya no maneja los tilesets.   
- v1.1 Se elimina la función `LoadSpriteAtlasWithOption` por problemas de carga (se re-implementará en una futura versión).   

### Corregido

- v1.1 Se corrige el error al momento de liberar de memoria el nombre de los sprites.  
- v1.1 Se corrige el error al momento de cargar un paquete de conjuntos de mosaicos.   
- v1.1 Se corrige el error al cargar y dibujar el mapa de mosaicos haciendo uso de `TilesetPack`.   
- v1.1 Se corrige el error que no permitia borra la memoria de `TilesetPackNode` y `TileMapNode`.   

## [1.0] "Kikiriki" - 2025-01-01

### Añadido  

- Se agregó la función `CreateTileMap` para cargar un mapa y los tilesets desde un archivo "tmx".  
- Se agregó el tipo `TileMap` para cargar un mapa de mosaicos desde Tiled.   
- Se agregó el tipo de dato `Tile` para gestionar los mosaicos de los tilesets.   
- Se agregó el tipo de dato `Tileset` para cargar tilesets con datos de Tiled.   
- Se agregó el tipo de dato `TiledLayer` para cargar los datos de las capas de Tiled.   
- Se agregó la función `LoadTilemap` para simplificar la carga de un mapa.   
- Se agregó la función `FreeGame` para eliminar de memoria una instancia de juego.   
- Se agregó la función `AddLevel` para agregar un nivel a un juego.   
- Se agregó la función `UnloadLevel` para eliminar un nivel de memoria.
- Se agregó la función `CreateLevel` para crear un nivel.
- Se agregó el tipo de dato `GameLevel` para definir un nivel y sus propiedades.   
- Se agregó la función `LoadSpriteAtlasWithOptions` para cargar una colección de sprites con opciones.   
- Se agregó la función `LoadSpriteAtlas` para cargar una colección de sprites.  
- Se agregó la función `LoadSpriteWithOptions` para cargar un sprite con opciones.  
- Se agregó la función `LoadSprite` para simplificar la carga de un sprite.  
- Se agregó la función `RunGame` para la ejecución de un juego.  
- Se agregó la función `InitGame` para inicializar una instancia de juego.  
- Se agregó el tipo de dato `Game` para el control de un juego.    
- Se agregó el tipo de dato `GameConfig` para las configuraciones del juego.  
- Se agregó la función `UnloadResourcesManager` para eliminar de memoria los recursos del gestor.  
- Se agregó la función `UnloadTreeSpriteNode` para eliminar de memoria el árbol de sprites.  
- Se agregó la función `GetSpriteAnimation` para devolver los datos de animación de un sprite.  
- Se agregó la función `GetSpriteFrame` para devolver el frame de un sprite siempre y cuando lo tenga.  
- Se agregó la función `GetSprite` para devolver un sprite.  
- Se agregó la función `AddSprite` para agregar un sprite al gestor de recursos.  
- Se agregó la función `AddTexture` para cargar y agregar una textura al gestor de recursos.  
- Se agregó la función `InitResourcesManager` para inicializar el gestor de recursos.  
- Se agregó el tipo de dato `ResourcesManager` para la gestión de los recursos. 
- Se agregó el tipo de dato `Sprite Animation` para definir la animación de un sprite.
- Se agregó el tipo de dato `SpriteFrame` para definir el frame de un sprite.
- Se agregó el tipo de dato `Sprite` para definir un sprite con sus propiedades.
- Se agregó el tipo de dato `TextureResult` para la carga de texturas.  
