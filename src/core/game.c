#include "game.h"

void InitGame(Game* game, GameConfig config, void(*LoadResources)(struct Game* game))
{
	game->config = config;
	game->LoadResources = LoadResources;
	InitResourcesManager(&game->resourcesManager);

	InitWindow(game->config.windowWidth, game->config.windowHeight, game->config.windowTitle);
	SetWindowState(FLAG_WINDOW_RESIZABLE);
	SetWindowMinSize(game->config.windowWidth, game->config.windowHeight);

	// verifica si el juego se puede cerrar con la tecla "Escape"
	if (game->config.useEscapeToExit == 0)
	{
		SetExitKey(-1);
	}

	// verificar si el juego se incializa en pantalla completa
	if (game->config.windowFullscreen == 1)
	{
		int fsWidth = GetMonitorWidth(GetCurrentMonitor());
		int fsHeight = GetMonitorHeight(GetCurrentMonitor());
		printf("Win Width: %d | Win Height: %d\n", fsWidth, fsHeight);
		SetWindowSize(fsWidth, fsHeight);
		ToggleFullscreen();
	}

	game->levelCache = NULL;
	game->levelStack = NULL;
	game->levelCacheCount = 0;
	game->levelStackCount = 0;
	game->currentLevel = -1;

	if (game->LoadResources != NULL)
		game->LoadResources(game);
}

void SetGameWindowIcon(Game* game, const char* filename)
{
	Image icon = LoadImage(filename);
	SetWindowIcon(icon);
	UnloadImage(icon);
}

void RunGame(Game* game)
{

	while (!WindowShouldClose())
	{
		for (int i = 0; i < game->levelStackCount; i++)
		{
			if (i == game->currentLevel)
			{
				if (game->levelStack[i]->Update != NULL)
				{
					game->levelStack[i]->Update(game, game->levelStack[i]);
				}
			}
			else
			{
				if(game->levelStack[i]->updateInStack == 1)
				{
					game->levelStack[i]->Update(game, game->levelStack[i]);
				}
			}
		}

		BeginDrawing();
		ClearBackground(game->config.colorBackground);

		if (game->levelStackCount > 0)
		{
			for (int i = 0; i < game->levelStackCount; i++)
			{
				if (i == game->currentLevel)
				{
					if(game->levelStack[i]->Render != NULL)
						game->levelStack[i]->Render(game, game->levelStack[i]);
				}
				else
				{
					if (game->levelStack[i]->renderInStack == 1)
						game->levelStack[i]->Render(game, game->levelStack[i]);
					
				}
			}
		}

		EndDrawing();
	}
}

int LoadSprite(Game* game, const char* textureFilename, const char* name)
{
	TextureResult result = AddTexture(&game->resourcesManager, textureFilename);
	if (result.status == -1) return 0;

	Sprite* sprite = CreateSprite(name, result.textureIndex, 0, 0, result.textureWidth, result.textureHeight, (Vector2) { result.textureWidth/2.0f, result.textureHeight/2.0f }, 0);
	if (!sprite) return 0;

	AddSprite(&game->resourcesManager, sprite);
	return 1;
}

int LoadSpriteWithOptions(Game* game, const char* textureFilename, const char* dataFilename)
{
	FILE* file = fopen(dataFilename, "r");
	if (file == NULL) return 0;

	TextureResult result = AddTexture(&game->resourcesManager, textureFilename);
	if (result.status == -1)
	{
		fclose(file);
		return 0;
	}

	Sprite* sprite = NULL;

	char buffer[1024];
	while (fgets(buffer, sizeof(buffer), file) != NULL)
	{
		size_t len = strlen(buffer);
		if (len > 0 && buffer[len - 1] == '\n')
			buffer[len - 1] = '\0';

		if (strlen(buffer) > 0 && buffer[0] != '#')
		{
			char* line = strdup(buffer);
			if (line != NULL)
			{
				if (strcmp(line, "@break") == 0)
				{
					free(line);
					break;
				}

				char* key = strtok(line, "=");
				char* value = strtok(NULL, "=");
				if (key != NULL && value != NULL)
				{
					if (strcmp(key, "data") == 0)
					{
						char* spriteName = strtok(value, "@");
						char* spriteData = strtok(NULL, "@");
						if (spriteName != NULL && spriteData != NULL)
						{
							char* sxStr = strtok(spriteData, ",");
							char* syStr = strtok(NULL, ",");
							if (sxStr != NULL && syStr != NULL)
							{
								int sliceX = atoi(sxStr);
								int sliceY = atoi(syStr);

								sprite = CreateSprite(spriteName, result.textureIndex, 0, 0, result.textureWidth, result.textureHeight, (Vector2){ result.textureWidth/2.0f, result.textureHeight/2.0f }, (sliceX * sliceY));
								if (sprite != NULL)
								{
									int frameWidth = result.textureWidth / sliceX;
									int frameHeight = result.textureHeight / sliceY;
									for (int i = 0; i < sliceY; i++)
									{
										for (int j = 0; j < sliceX; j++)
										{
											AddSpriteFrame(sprite, frameWidth * j, frameHeight * i, frameWidth, frameHeight);
										}
									}
								}
							}
						}
					}

					// animaciones
					if (strcmp(key, "anik") == 0)
					{
						char* animationName = strtok(value, "@");
						char* animationData = strtok(NULL, "@");
						if (animationName != NULL && animationData != NULL)
						{
							int animationOptions[4] = { 0, 0, 0, 0 };
							int index = 0;
							char* token;
							char* context = NULL; // hace feliz a visual studio

							token = strtok_s(animationData, ",", &context);
							while (token != NULL)
							{
								animationOptions[index] = atoi(token);
								index++;
								token = strtok_s(NULL, ",", &context);
							}
							if (sprite != NULL)
							{
								AddSpriteAnimation(
									sprite,
									animationName,
									animationOptions[0],
									animationOptions[1],
									animationOptions[2],
									animationOptions[3]
								);
							}
						}
					}
				}

				free(line);
			}
		}
	}

	if (sprite != NULL)
		AddSprite(&game->resourcesManager, sprite);

	return 1;
}

int LoadSpriteAtlas(Game* game, const char* textureFilename, const char* dataFilename)
{
	FILE* file = fopen(dataFilename, "r");
	if (file == NULL) return 0;

	TextureResult result = AddTexture(&game->resourcesManager, textureFilename);
	if (result.status == -1)
	{
		fclose(file);
		return 0;
	}

	char buffer[1024];
	while (fgets(buffer, sizeof(buffer), file) != NULL)
	{
		size_t len = strlen(buffer);
		if (len > 0 && buffer[len - 1] == '\n')
			buffer[len - 1] = '\0';

		if (strlen(buffer) > 0 && buffer[0] != '#')
		{
			char* line = strdup(buffer);
			if (line != NULL)
			{
				char* key = strtok(line, "=");
				char* value = strtok(NULL, "=");

				if (key != NULL && value != NULL)
				{
					if (strcmp(key, "sprite") == 0)
					{
						char* spriteName = strtok(value, "[");
						char* spriteCoords = strtok(NULL, "[");

						if (spriteName != NULL && spriteCoords != NULL)
						{
							int coords[4] = { 0, 0, 0, 0 };
							int index = 0;
							char* token;
							char* context = NULL;

							token = strtok_s(spriteCoords, ",", &context);
							while (token != NULL)
							{
								coords[index] = atoi(token);
								index++;
								token = strtok_s(NULL, ",", &context);
							}
							// crear un nuevo sprite
							Sprite* newSprite = CreateSprite(
								spriteName,
								result.textureIndex,
								coords[0],
								coords[1],
								coords[2],
								coords[3],
								(Vector2){ result.textureWidth/2.0f, result.textureHeight/2.0f },
								0
							);

							if (newSprite != NULL)
								AddSprite(&game->resourcesManager, newSprite);
						}
					}
				}

				free(line);
			}
		}
	}

	fclose(file);
	return 1;
}

int AddTilsetToPack(Game* game, const char* pack, const char* filename)
{
	if (game == NULL) return;

	TilesetPack* tsPack = GetTilesetPack(&game->resourcesManager, pack);
	if (tsPack == NULL) return NULL;

	int result = InsertTilesetToPack(tsPack, filename);

	return result;
}

int LoadTilsetsPack(Game* game, const char* filename, const char* name)
{
	if (game == NULL) return 0;

	TilesetPack* pack = CreateTilesetsPack(name, filename);
	if (pack == NULL) return 0;

	AddTilesetsPack(&game->resourcesManager, pack);
	return 1;
}

int LoadTileMap(Game* game, const char* filename, const char* pack, const char* name)
{
	if (game == NULL) return 0;

	TilesetPack* tsPack = GetTilesetPack(&game->resourcesManager, pack);
	if (tsPack == NULL) return 0;

	TileMap* map = CreateMap(name, filename, tsPack);
	if (map == NULL) return 0;

	AddTilemap(&game->resourcesManager, map);
	return 1;
}

int CheckLevel(Game* game, const char* name)
{
	for (int i = 0; i < game->levelCacheCount; i++)
	{
		int cmp = strcmp(game->levelCache[i]->name, name);
		if (cmp == 0) return 1;
	}

	for (int i = 0; i < game->levelStackCount; i++)
	{
		int cmp = strcmp(game->levelStack[i]->name, name);
		if (cmp == 0) return 1;
	}

	return 0;
}

int LoadLevel(Game* game, const char* name, unsigned int keepInMemory, unsigned int renderInStack, unsigned int updateInStack, void(*OnLoad)(Game* game, GameLevel* level))
{
	if (CheckLevel(game, name) == 1) return 0;

	GameLevel* level = CreateLevel(name, keepInMemory, renderInStack, updateInStack, OnLoad);
	if (level == NULL) return 0;

	level->Update = UpdateLevel;
	level->Render = RenderLevel;

	if (game->levelStack[game->currentLevel]->keepInMemory == 1)
	{
		if (AddLevelToCacheStack(game, game->levelStack[game->currentLevel]) == 1)
		{
			game->levelStack[game->currentLevel] = NULL;
			game->levelStack[game->currentLevel] = level;
			if (game->levelStack[game->currentLevel]->Load != NULL)
				game->levelStack[game->currentLevel]->Load(game, game->levelStack[game->currentLevel]);
			return 1;
		}
	}
	else
	{
		UnloadLevel(game->levelStack[game->currentLevel]);
		game->levelStack[game->currentLevel] = NULL;
		game->levelStack[game->currentLevel] = level;
		if (game->levelStack[game->currentLevel]->Load != NULL)
			game->levelStack[game->currentLevel]->Load(game, game->levelStack[game->currentLevel]);
		return 1;
	}

	return 0;
}

int PushLevel(Game* game, const char* name, unsigned int keepInMemory, unsigned int renderInStack, unsigned int updateInStack, void(*OnLoad)(Game* game, GameLevel* level))
{
	if (CheckLevel(game, name) == 1) return 0;

	GameLevel* level = CreateLevel(name, keepInMemory, renderInStack, updateInStack, OnLoad);
	if (level == NULL) return 0;

	level->Update = UpdateLevel;
	level->Render = RenderLevel;

	GameLevel** levelsMemTemp = (GameLevel**)realloc(game->levelStack, (size_t)(game->levelStackCount + 1) * sizeof(GameLevel*));
	if (levelsMemTemp == NULL)
	{
		UnloadLevel(level);
		return 0;
	}

	game->levelStack = levelsMemTemp;
	game->levelStack[game->levelStackCount] = level;
	game->currentLevel = game->levelStackCount;
	game->levelStackCount += 1;

	if (level->Load != NULL) level->Load(game, level);
	return 1;
}

int PushMemoryLevel(Game* game, const char* name)
{
	if (game->levelCacheCount <= 0) return 0;
	if (game->levelCache == NULL) return 0;

	for (int i = 0; i < game->levelCacheCount; i++)
	{
		int nameCompare = strcmp(game->levelCache[i]->name, name);
		printf("NOMBRES A COMPARAR: %s <> %s\n", game->levelCache[i]->name, name);
		if (nameCompare == 0)
		{
			GameLevel** cacheMemTemp = NULL;
			if (game->levelCacheCount > 1)
			{
				cacheMemTemp = (GameLevel**)realloc(game->levelCache, (size_t)(game->levelCacheCount - 1) * sizeof(GameLevel*));
				if(cacheMemTemp == NULL) return 0;
			}

			// guardamos el nivel en un variable
			GameLevel* refLevel = game->levelCache[i];

			GameLevel** memTemp = (GameLevel**)realloc(game->levelStack, (size_t)(game->levelStackCount + 1) * sizeof(GameLevel*));
			if (memTemp == NULL) return 0;

			game->levelStack = memTemp;
			game->levelStack[game->levelStackCount] = refLevel;
			game->currentLevel = game->levelStackCount;
			game->levelStackCount += 1;
			game->levelCache[i] = NULL;

			if(cacheMemTemp != NULL)
			{ 
				/* reordernar */ 
				for (int j = i; j < game->levelCacheCount; j++)
				{
					game->levelCache[j] = game->levelCache[j + 1];
					game->levelCache[j + 1] = NULL;
				}
				game->levelCache = cacheMemTemp;
				game->levelCacheCount -= 1;
			}
			else
			{
				game->levelCache[0] = NULL;
				game->levelCacheCount = 0;
				free(game->levelCache);
				game->levelCache = NULL;
			}

			
			return 1;
		}
	}

	return 0;
}

int LoadMemoryLevel(Game* game, const char* name)
{
	if (game->levelCacheCount <= 0) return 0;
	if (game->levelCache == NULL) return 0;

	for (int i = 0; i < game->levelCacheCount; i++)
	{
		int nameCompare = strcmp(game->levelCache[i]->name, name);
		if (nameCompare == 0)
		{
			if (game->levelStack[game->currentLevel]->keepInMemory == 1)
			{
				// Hacemos un swap
				GameLevel* referTemp = game->levelStack[game->currentLevel];
				game->levelStack[game->currentLevel] = NULL;
				game->levelStack[game->currentLevel] = game->levelCache[i];
				game->levelCache[i] = referTemp;
				return 1;
			}

			// hacemos redimensión de memoria.
			GameLevel** cacheMemTemp = NULL;
			if(game->levelCacheCount > 1)
				cacheMemTemp = (GameLevel**)realloc(game->levelCache, (size_t)(game->levelCacheCount - 1) * sizeof(GameLevel*));

			// Descargar el nivel actual
			UnloadLevel(game->levelStack[game->currentLevel]);
			game->levelStack[game->currentLevel] = NULL;
			game->levelStack[game->currentLevel] = game->levelCache[i];
			game->levelCache[i] = NULL;

			// ordenar los nivel de cache antes de reajustar el bloque de memoria
			if (cacheMemTemp != NULL)
			{
				for (int j = i; j < game->levelCacheCount; j++)
				{
					game->levelCache[j] = game->levelCache[j + 1];
					game->levelCache[j + 1] = NULL;
				}

				game->levelCache = cacheMemTemp;
				game->levelCacheCount -= 1;
			}
			else
			{
				game->levelCache[0] = NULL;
				game->levelCacheCount = 0;
				free(game->levelCache);
				game->levelCache = NULL;
			}

			return 1;
		}
	}

	return 0;
}

int AddLevelToCacheStack(Game* game, GameLevel* level)
{
	if (level == NULL) return;

	GameLevel** memTemp = (GameLevel**)realloc(game->levelCache, (size_t)(game->levelCacheCount + 1) * sizeof(GameLevel*));
	if (memTemp == NULL) return 0;

	game->levelCache = memTemp;
	game->levelCache[game->levelCacheCount] = level;
	game->levelCacheCount += 1;

	return 1;
}

int PopLevel(Game* game)
{
	if (game->levelStackCount <= 0) return 0;

	GameLevel** levelStackMemTemp = (GameLevel**)realloc(game->levelStack, (size_t)(game->levelStackCount - 1) * sizeof(GameLevel*));
	if (levelStackMemTemp == NULL) return 0;

	if (AddLevelToCacheStack(game, game->levelStack[game->levelStackCount - 1]) == 1)
	{
		game->levelStack[game->levelStackCount - 1] = NULL;
		game->levelStack = levelStackMemTemp;
		game->currentLevel = game->levelStackCount;
		game->levelStackCount -= 1;
		
	}

	return 0;
}

/*int LoadLevel(Game* game, const char* filename)
{
	// si no se puede abrir el archivo, se retorna cero.
	FILE* levelFile = fopen(filename, "r");
	if (levelFile == NULL) return 0;

	mxml_options_t* options = mxmlOptionsNew();
	mxmlOptionsSetTypeValue(options, MXML_TYPE_OPAQUE);
	mxml_node_t* root = mxmlLoadFile(NULL, options, levelFile);
	fclose(levelFile);

	// si no se encuentra el elemento root, se retorna cero
	if (root == NULL)
	{
		mxmlOptionsDelete(options);
		return 0;
	}

	mxmlOptionsDelete(options);
	return 0;
}*/

ecs_entity_t CreateBlankEntity(GameLevel* level, const char* name, const char* tag)
{
	ECS_COMPONENT(level->world, C_Info);
	ECS_COMPONENT(level->world, C_Transform);

	struct ecs_entity_desc_t desc = { 0 };
	desc.name = tag;

	ecs_entity_t entity = ecs_entity_init(level->world, &desc);

	// agregar los componentes base
	ecs_add(level->world, entity, C_Info);
	ecs_add(level->world, entity, C_Transform);

	// establecer los datos de los componentes
	ecs_set(level->world, entity, C_Info, { name, tag });
	ecs_set(level->world, entity, C_Transform, {0.0f, 0.0f, 1.0f, 1.0f, 0.0f});

	return entity;
}

int AddComponentToEntity(Game* game, GameLevel* level, ecs_entity_t entity, const char* component, const char* cdata)
{
	ECS_COMPONENT(level->world, C_Camera2D);
	ECS_COMPONENT(level->world, C_Info);
	ECS_COMPONENT(level->world, C_RenderLayer);
	ECS_COMPONENT(level->world, C_Transform);
	ECS_COMPONENT(level->world, C_SpriteRender);
	ECS_COMPONENT(level->world, C_Color);
	ECS_COMPONENT(level->world, C_MapRender);
	ECS_COMPONENT(level->world, C_RectCollider);
	ECS_COMPONENT(level->world, C_CircleCollider);
	ECS_COMPONENT(level->world, C_DayCicle);
	ECS_COMPONENT(level->world, C_Collector);
	ECS_COMPONENT(level->world, C_Dialog);
	ECS_COMPONENT(level->world, C_Inventory);
	ECS_COMPONENT(level->world, C_HotBar);
	ECS_COMPONENT(level->world, C_Movement);
	//ECS_COMPONENT(level->world, C_Action);
	ECS_COMPONENT(level->world, C_PlayerStats);
	ECS_COMPONENT(level->world, C_WorldItem);
	ECS_COMPONENT(level->world, C_Build);
	ECS_COMPONENT(level->world, C_Builder);
	ECS_COMPONENT(level->world, C_Trader);
	ECS_COMPONENT(level->world, C_DropTable);
	ECS_COMPONENT(level->world, C_FarmLand);
	ECS_COMPONENT(level->world, C_Crop);
	ECS_COMPONENT(level->world, C_Tree);
	ECS_COMPONENT(level->world, C_Ore);

	if (strcmp(component, C_CAMERA_2D_ID) == 0)
	{
		if (ecs_has(level->world, entity, C_Camera2D)) return 0;

		char* data = strdup(cdata);
		char* context = NULL;

		char* token = strtok_s(data, ",", &context);
		// indica si la cámara es la principal
		unsigned int isMain = atoi(token);
		// offset de la cámara
		token = strtok_s(NULL, ",", &context);
		float offsetX = atof(token);
		token = strtok_s(NULL, ",", &context);
		float offsetY = atof(token);
		// target al que apunta la camara
		token = strtok_s(NULL, ",", &context);
		float targetX = atof(token);
		token = strtok_s(NULL, ",", &context);
		float targetY = atof(token);
		// rotation
		token = strtok_s(NULL, ",", &context);
		float rotation = atof(token);
		// zoom
		token = strtok_s(NULL, ",", &context);
		float zoom = atof(token);

		ecs_add(level->world, entity, C_Camera2D);
		ecs_set(level->world, entity, C_Camera2D, { isMain, offsetX, offsetY, targetX, targetY, rotation, zoom });
		free(data);

		return 1;
	}
	else if (strcmp(component, C_INFO_ID) == 0)
	{
		/*
			en este caso no se comprueba si la entidad tiene el componente
			ya que toda entidad se crear con este componente, así que
			solo cambiamos sus datos
		*/
		// nombre
		char* data = strdup(cdata);
		char* context = NULL;
		char* token = strtok_s(data, ",", &context);
		char* name = token;
		// tag
		token = strtok_s(NULL, ",", &context);
		char* tag = token;

		ecs_set(level->world, entity, C_Info, { name, tag });
		free(data);
		return 1;
	}
	else if (strcmp(component, C_RENDER_LAYER_ID) == 0) 
	{
		if (ecs_has(level->world, entity, C_RenderLayer)) return 0;

		char* data = strdup(cdata);

		int index = atoi(data);
		ecs_set(level->world, entity, C_RenderLayer, { index });

		free(data);
		return 1;
	}
	else if (strcmp(component, C_TRANSFORM_ID) == 0) 
	{
		/*
			en este caso no se comprueba si la entidad tiene el componente
			ya que toda entidad se crear con este componente, así que
			solo cambiamos sus datos
		*/
		char* data = strdup(cdata);
		char* context = NULL;
		// position
		char* token = strtok_s(data, ",", &context);
		float posX = atof(token);
		token = strtok_s(NULL, ",", &context);
		float posY = atof(token);
		// scale
		token = strtok_s(NULL, ",", &context);
		float scaleX = atof(token);
		token = strtok_s(NULL, ",", &context);
		float scaleY = atof(token);
		// rotation
		token = strtok_s(NULL, ",", &context);
		float rotation = atof(token);

		ecs_set(level->world, entity, C_Transform, { posX, posY, scaleX, scaleY, rotation });
		free(data);
		return 1;

	}
	else if (strcmp(component, C_SPRITE_RENDER_ID) == 0) 
	{
		if (ecs_has(level->world, entity, C_SpriteRender)) return 0;

		char* data = strdup(cdata);

		char* context = NULL;
		// name
		char* token = strtok_s(data, ",", &context);
		char* name = token;
		// visible
		token = strtok_s(NULL, ",", &context);
		int visible = atoi(token);
		// opacity
		token = strtok_s(NULL, ",", &context);
		float opacity = atof(token);

		ecs_set(level->world, entity, C_SpriteRender, { name, visible, opacity });

		free(data);
		return 1;
	}
	else if (strcmp(component, C_COLOR_ID) == 0) 
	{
		if (ecs_has(level->world, entity, C_Color)) return 0;

		char* data = strdup(cdata);

		char* context = NULL;
		char* token = strtok_s(data, ",", &context);
		int r = atoi(token);
		token = strtok_s(NULL, ",", &context);
		int g = atoi(token);
		token = strtok_s(NULL, ",", &context);
		int b = atoi(token);

		ecs_set(level->world, entity, C_Color, { r, g, b });

		free(data);
		return 1;
	}
	else if (strcmp(component, C_MAP_RENDER_ID) == 0)
	{
		if (ecs_has(level->world, entity, C_MapRender)) return 0;

		char* data = strdup(cdata);

		char* name = strdup(data);

		ecs_set(level->world, entity, C_MapRender, { name });

		free(data);
		return 1;
	}
	else if (strcmp(component, C_RECT_COLLIDER_ID) == 0) {}
	else if (strcmp(component, C_CIRCLE_COLLIDER_ID) == 0) {}
	else if (strcmp(component, C_DAY_CYCLE_ID) == 0) {}
	else if (strcmp(component, C_COLLECTOR_ID) == 0) {}
	else if (strcmp(component, C_DIALOG_ID) == 0) 
	{
		if (ecs_has(level->world, entity, C_Dialog)) return 0; // ya tiene el componente

		char* data = strdup(cdata);

		char* context = NULL;
		char* token = strtok_s(data, "@", &context);
		int interactionCount = atoi(token);

		char** dialogs = NULL;
		int count = 0;

		token = strtok_s(NULL, "@", &context);
		while (token != NULL)
		{
			char** memTemp = (char**)realloc(dialogs, (size_t)(count + 1) * sizeof(char*));
			if (memTemp == NULL) return 0;

			dialogs = memTemp;
			dialogs[count] = token;
			count++;

			token = strtok_s(NULL, "@", &context);
		}

		ecs_set(level->world, entity, C_Dialog,{ dialogs, count, interactionCount });

		free(data);
		return 1;
	}
	else if (strcmp(component, C_INVENTORY_ID) == 0) {}
	else if (strcmp(component, C_HOTBAR_ID) == 0) {}
	else if (strcmp(component, C_MOVEMENT_ID) == 0) {}
	else if (strcmp(component, C_PLAYER_STATS_ID) == 0) {}
	else if (strcmp(component, C_WORLD_ITEM_ID) == 0) {}
	else if (strcmp(component, C_BUILD_ID) == 0) {}
	else if (strcmp(component, C_BUILDER_ID) == 0) {}
	else if (strcmp(component, C_TRADER_ID) == 0) {}
	else if (strcmp(component, C_DROP_TABLE_ID) == 0) {}
	else if (strcmp(component, C_FARM_LAND_ID) == 0) {}
	else if (strcmp(component, C_CROP_ID) == 0) {}
	else if (strcmp(component, C_TREE_ID) == 0) {}
	else if (strcmp(component, C_ORE_ID) == 0) {}

	return 0;
}

int AddEntityBehaviour(GameLevel* level, ecs_entity_t entity, void(*OnCreate)(Game* game, GameLevel* level, ecs_entity_t entity), void(*OnInput)(Game* game, GameLevel* level, ecs_entity_t entity), void(*OnUpdate)(Game* game, GameLevel* level, ecs_entity_t entity), void(*OnDestroy)(Game* game, GameLevel* level, ecs_entity_t entity), void(*OnCollision)(Game* game, GameLevel* level, ecs_entity_t entity, ecs_entity_t collide))
{
	if (entity == 0) return 0;

	ECS_COMPONENT(level->world, C_Behaviour);
	ecs_set(level->world, entity, C_Behaviour, { OnCreate, OnInput, OnUpdate, OnDestroy, OnCollision });

	return 1;
}

int AddLevel(Game* game,
	const char* name, 
	unsigned int keepInMemory, 
	unsigned int renderInStack, 
	unsigned int updateInStack,
	void(*Run)(struct Game* game, struct GameLevel* level)
)
{
	GameLevel* level = CreateLevel(name, keepInMemory, renderInStack, updateInStack, Run);
	if (level == NULL) return 0;

	GameLevel** levelsMemTemp = (GameLevel**)realloc(game->levelStack, (size_t)(game->levelStackCount + 1) * sizeof(GameLevel*));
	if (levelsMemTemp == NULL)
	{
		UnloadLevel(level);
		return 0;
	}

	game->levelStack = levelsMemTemp;
	game->levelStack[game->levelStackCount] = level;
	game->currentLevel = game->levelStackCount;
	game->levelStackCount += 1;

	return 1;
}

void UpdateLevel(Game* game, GameLevel* level)
{
	ECS_COMPONENT(level->world, C_Behaviour);
	ECS_COMPONENT(level->world, C_Transform);
	ECS_COMPONENT(level->world, C_RectCollider);
	ECS_COMPONENT(level->world, C_CircleCollider);

	/******************************************************************************
	* COMPORTAMIENTO PERSONALIZADO
	******************************************************************************/
	ecs_query_t* query = ecs_query_init(level->world, &(ecs_query_desc_t){
		.terms = {{ .id =  ecs_id(C_Behaviour) }}
	});

	ecs_iter_t it = ecs_query_iter(level->world, query);
	while (ecs_query_next(&it))
	{
		for (int i = 0; i < it.count; i++)
		{
			C_Behaviour* behaviour = ecs_get(level->world, it.entities[i], C_Behaviour);
			if (behaviour == NULL) continue;
			if (behaviour->OnInput != NULL)
				behaviour->OnInput(game, level, it.entities[i]);
			if(behaviour->OnUpdate)
				behaviour->OnUpdate(game, level, it.entities[i]);
		}
	}

	ecs_query_fini(query);

	/******************************************************************************
	* GESTION DE COLISIONES
	******************************************************************************/
	ecs_query_t* queryCollider = ecs_query_init(level->world, &(ecs_query_desc_t){
		.terms = { 
			{ .id = ecs_id(C_RectCollider), .oper = EcsOr }, // 1
			{ .id = ecs_id(C_CircleCollider) } // 2
		}
	});

	ecs_iter_t itCollider = ecs_query_iter(level->world, queryCollider);
	while (ecs_query_next(&itCollider))
	{
		for (int i = 0; i < itCollider.count; i++)
		{
			C_Transform* transform = ecs_get(level->world, it.entities[i], C_Transform);
			C_RectCollider* rectCollider = ecs_get(level->world, it.entities[i], C_RectCollider);
			if (rectCollider != NULL)
			{
				float px = transform->positionX + rectCollider->offsetX;
				float py = transform->positionY + rectCollider->offsetY;
				rectCollider->posX = px;
				rectCollider->posY = py;
			}
		}
	}

	ecs_query_fini(queryCollider);
}

void RenderLevel(Game* game, GameLevel* level)
{
	ECS_COMPONENT(level->world, C_Transform);
	ECS_COMPONENT(level->world, C_Color);
	ECS_COMPONENT(level->world, C_SpriteRender);
	ECS_COMPONENT(level->world, C_MapRender);

	/******************************************************************************
	* RENDERIZADO DE ELEMENTOS
	******************************************************************************/
	ecs_query_t* queryRender = ecs_query_init(level->world, &(ecs_query_desc_t){
		.terms = {
			{ .id = ecs_id(C_SpriteRender), .oper = EcsOr },
			{ .id = ecs_id(C_MapRender), .oper = EcsOr },
			{ .id = ecs_id(C_Color) }
		}
	});

	ecs_iter_t itRender = ecs_query_iter(level->world, queryRender);
	while (ecs_query_next(&itRender))
	{
		for (int i = 0; i < itRender.count; i++)
		{
			ecs_entity_t entity = itRender.entities[i];
			C_Transform* transform = ecs_get(level->world, entity, C_Transform);
			C_Color* color = ecs_get(level->world, entity, C_Color);
			C_SpriteRender* spriteRender = ecs_get(level->world, entity, C_SpriteRender);
			C_MapRender* mapRender = ecs_get(level->world, entity, C_MapRender);

			// Si renderiza un mapa
			if (mapRender != NULL)
			{
				TileMap* map = GetTileMap(&game->resourcesManager, mapRender->name);
				if (map == NULL) continue;

				DrawTexturePro(
					map->mapTexture.texture,
					(Rectangle){ 0, 0, map->mapTexture.texture.width, -map->mapTexture.texture.height },
					(Rectangle){ transform->positionX, transform->positionY, map->mapTexture.texture.width * transform->scaleX, map->mapTexture.texture.height * transform->scaleY },
					(Vector2){ 0.0f, 0.0f },
					0.0f,
					color != NULL ? (Color){ color->r, color->g, color->b, 255 } : WHITE
				);
			}
			// Si renderiza un sprite
			if (spriteRender != NULL)
			{
				Sprite* sprite = GetSprite(&game->resourcesManager, spriteRender->spriteName);
				if (sprite == NULL) continue;

				Texture2D tex = game->resourcesManager.textures[sprite->textureIndex];
				DrawTexturePro(
					tex,
					(Rectangle) {sprite->x, sprite->y, sprite->width, sprite->height},
					(Rectangle) { transform->positionX, transform->positionY, sprite->width* transform->scaleX, sprite->height* transform->scaleY
					},
					sprite->origin,
					transform->rotation,
					color != NULL ? (Color) { color->r, color->g, color->b, spriteRender->opacity } : WHITE
				);
			}
		}
	}
}

void MapRenderDestroyHook(void* ptr)
{
	C_MapRender* comp = ptr;
	if (comp->name)
	{
		free(comp->name);
		comp->name = NULL;
	}
}

void FreeGame(Game* game)
{
	for (int i = 0; i < game->levelCacheCount; i++)
	{
		ECS_COMPONENT(game->levelCache[i]->world, C_MapRender);
		ecs_set_hooks(game->levelCache[i]->world, C_MapRender, {
			.dtor = MapRenderDestroyHook
			});
	}

	for (int i = 0; i < game->levelStackCount; i++)
	{
		ECS_COMPONENT(game->levelStack[i]->world, C_MapRender);
		ecs_set_hooks(game->levelStack[i]->world, C_MapRender, {
			.dtor = MapRenderDestroyHook
		});
	}

	if (game->levelCacheCount > 0)
	{
		for (int i = 0; i < game->levelCacheCount; i++)
		{
			UnloadLevel(game->levelCache[i]);
		}
		free(game->levelCache);
		game->levelCache = NULL;
	}

	if (game->levelStackCount > 0)
	{
		for (int i = 0; i < game->levelStackCount; i++)
		{
			UnloadLevel(game->levelStack[i]);
		}
		free(game->levelStack);
		game->levelStack = NULL;
	}

	UnloadResourcesManager(&game->resourcesManager);

	CloseWindow();
}
