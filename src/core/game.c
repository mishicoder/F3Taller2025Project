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
		BeginDrawing();
		ClearBackground(game->config.colorBackground);

		if (game->levelStackCount > 0)
		{
			for (int i = 0; i < game->levelStackCount; i++)
			{
				if(i != game->currentLevel)
				{
					if (game->levelStack[i]->renderInStack)
						game->levelStack[i]->Run(game, game->levelStack[i]);
				}
				else
				{
					game->levelStack[i]->Run(game, game->levelStack[i]);
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

	if (strcmp(component, C_CAMERA_2D_ID))
	{
		if (ecs_has(level->world, entity, C_Camera2D)) return 0;

		char* token = strtok(cdata, ",");
		// indica si la cámara es la principal
		unsigned int isMain = atoi(token);
		// offset de la cámara
		token = strtok(NULL, ",");
		float offsetX = atof(token);
		token = strtok(NULL, ",");
		float offsetY = atof(token);
		// target al que apunta la camara
		token = strtok(NULL, ",");
		float targetX = atof(token);
		token = strtok(NULL, ",");
		float targetY = atof(token);
		// rotation
		token = strtok(NULL, ",");
		float rotation = atof(token);
		// zoom
		token = strtok(NULL, ",");
		float zoom = atof(token);

		ecs_add(level->world, entity, C_Camera2D);
		ecs_set(level->world, entity, C_Camera2D, { isMain, offsetX, offsetY, targetX, targetY, rotation, zoom });

		return 1;
	}
	else if (strcmp(component, C_INFO_ID))
	{
		/*
			en este caso no se comprueba si la entidad tiene el componente
			ya que toda entidad se crear con este componente, así que
			solo cambiamos sus datos
		*/
		// nombre
		char* token = strtok(cdata, ",");
		char* name = token;
		// tag
		char* token = strtok(NULL, ",");
		char* tag = token;

		ecs_set(level->world, entity, C_Info, { name, tag });
		return 1;
	}
	else if (strcmp(component, C_RENDER_LAYER_ID)) {}
	else if (strcmp(component, C_TRANSFORM_ID)) 
	{
		/*
			en este caso no se comprueba si la entidad tiene el componente
			ya que toda entidad se crear con este componente, así que
			solo cambiamos sus datos
		*/
		// position
		char* token = strtok(cdata, ",");
		float posX = atof(token);
		char* token = strtok(NULL, ",");
		float posY = atof(token);
		// scale
		char* token = strtok(NULL, ",");
		float scaleX = atof(token);
		char* token = strtok(NULL, ",");
		float scaleY = atof(token);
		// rotation
		char* token = strtok(NULL, ",");
		float rotation = atof(token);

		ecs_set(level->world, entity, C_Transform, { posX, posY, scaleX, scaleY, rotation });
		return 1;

	}
	else if (strcmp(component, C_SPRITE_RENDER_ID)) {}
	else if (strcmp(component, C_COLOR_ID)) {}
	else if (strcmp(component, C_MAP_RENDER_ID)) {}
	else if (strcmp(component, C_RECT_COLLIDER_ID)) {}
	else if (strcmp(component, C_CIRCLE_COLLIDER_ID)) {}
	else if (strcmp(component, C_DAY_CYCLE_ID)) {}
	else if (strcmp(component, C_COLLECTOR_ID)) {}
	else if (strcmp(component, C_DIALOG_ID)) 
	{
		if (ecs_has(level->world, entity, C_Dialog)) return 0; // ya tiene el componente

		char* token = strtok(cdata, "@");
		int interactionCount = atoi(token);

		char** dialogs = NULL;
		int count = 0;

		token = strtok(cdata, "@");
		while (token != NULL)
		{
			char** memTemp = (char**)realloc(dialogs, (size_t)(count + 1) * sizeof(char*));
			if (memTemp == NULL) return 0;

			dialogs = memTemp;
			dialogs[count] = token;
			count++;

			token = strtok(NULL, "@");
		}

		ecs_add(level->world, entity, C_Dialog);
		ecs_set(level->world, entity, C_Dialog,{ dialogs, count, interactionCount });

		return 1;
	}
	else if (strcmp(component, C_INVENTORY_ID)) {}
	else if (strcmp(component, C_HOTBAR_ID)) {}
	else if (strcmp(component, C_MOVEMENT_ID)) {}
	else if (strcmp(component, C_PLAYER_STATS_ID)) {}
	else if (strcmp(component, C_WORLD_ITEM_ID)) {}
	else if (strcmp(component, C_BUILD_ID)) {}
	else if (strcmp(component, C_BUILDER_ID)) {}
	else if (strcmp(component, C_TRADER_ID)) {}
	else if (strcmp(component, C_DROP_TABLE_ID)) {}
	else if (strcmp(component, C_FARM_LAND_ID)) {}
	else if (strcmp(component, C_CROP_ID)) {}
	else if (strcmp(component, C_TREE_ID)) {}
	else if (strcmp(component, C_ORE_ID)) {}

	return 0;
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

void FreeGame(Game* game)
{
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
