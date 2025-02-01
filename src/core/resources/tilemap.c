#include "tilemap.h"

TilesetPack* CreateTilesetsPack(const char* name, const char* filename)
{
	// Asigna bloque de memoria para el paquete.
	TilesetPack* pack = (TilesetPack*)malloc(sizeof(TilesetPack));
	// Verificar si la asignación ha sido correcta.
	if (pack == NULL)
	{
		// Muestra el error.
		printf("Error al crear el paquete de sets de mosaicos {%s}\n", name);
		// returna NULL.
		return NULL;
	}

	FILE* tilestsfile = fopen(filename, "r");
	if (tilestsfile == NULL)
	{
		free(pack);
		return NULL;
	}

	pack->name = name;
	pack->nextTilesetIndex = 1;
	pack->tilesetsCount = 0;
	pack->tilesets = NULL;

	char buffer[1024];
	while (fgets(buffer, sizeof(buffer), tilestsfile) != NULL)
	{
		size_t len = strlen(buffer);
		if (len > 0 && buffer[len - 1] == '\n')
			buffer[len - 1] = '\0';

		if (strlen(buffer) > 0 && buffer[0] != '#')
		{
			char* line = strdup(buffer);
			if (line != NULL)
			{
				// si encuentra la palabra "@break" romperá el bucle
				if (strcmp(line, "@break") == 0)
				{
					free(line);
					break;
				}

				// leer datos
				char* key = strtok(line, "@");
				char* val = strtok(NULL, "@");
				if (key != NULL && val != NULL)
				{
					if (strcmp(key, "ts") == 0)
					{
					}
				}
			}
		}
	}

	return pack;
}

Tileset* CreateTileset(const char* filename, unsigned int initialIndex)
{
	FILE* xmlFile = fopen(filename, "r");
	if(xmlFile == NULL)
	{
		printf("Imposible cargar el archivo {%s}\n", filename);
		return NULL;
	}

	Tileset* tileset = (Tileset*)malloc(sizeof(Tileset));
	if (tileset == NULL)
	{
		printf("Error al asignar memoria para el tileset.\n");
		fclose(xmlFile);
		return NULL;
	}

	tileset->initialIndex = initialIndex;

	mxml_options_t* options = mxmlOptionsNew();
	mxmlOptionsSetTypeValue(options, MXML_TYPE_OPAQUE);
	mxml_node_t* root = mxmlLoadFile(NULL, options, xmlFile);
	fclose(xmlFile);
	if (root == NULL)
	{
		printf("Error al leer los nodos del tileset.\n");
		free(tileset);
		mxmlOptionsDelete(options);
		return NULL;
	}

	// obtener el nodo tileset
	mxml_node_t* tsNode = mxmlFindElement(root, root, "tileset", NULL, NULL, MXML_DESCEND_ALL);
	if (tsNode == NULL)
	{
		free(tileset);
		mxmlOptionsDelete(options);
		mxmlDelete(root);
		return NULL;
	}

	// obtener atributos del tileset
	unsigned int tileWidth = atoi(mxmlElementGetAttr(tsNode, "tilewidth"));
	unsigned int tileHeight = atoi(mxmlElementGetAttr(tsNode, "tileheight"));
	tileset->finalIndex = (tileset->initialIndex + atoi(mxmlElementGetAttr(tsNode, "tilecount"))) - 1;
	tileset->tilesCount = atoi(mxmlElementGetAttr(tsNode, "tilecount"));

	// obtener el nodo de la imagen del tileset
	mxml_node_t* textureNode = mxmlFindElement(tsNode, root, "image", NULL, NULL, MXML_DESCEND_ALL);
	const char* textureFilename = mxmlElementGetAttr(textureNode, "source");
	tileset->texture = LoadTexture(textureFilename);

	// descargar datos de memoria
	mxmlOptionsDelete(options);
	mxmlDelete(root);

	return tileset;
}


/*
TileMap* CreateMap(const char* name, const char* filename)
{
	TileMap* tmap = (TileMap*)malloc(sizeof(TileMap));
	if (tmap == NULL)
	{
		printf("Error al asignar memoria para el tilemap: %s\n", name);
		return NULL;
	}

	tmap->name = _strdup(name);
	tmap->tiles = (Tile**)malloc(sizeof(Tile*));
	if (tmap->tiles == NULL)
	{
		printf("Error al asignar memoria para los tiles.\n");
		free(tmap->name);
		free(tmap);
		return NULL;
	}

	tmap->nextTileIndex = 1;
	tmap->tiles[tmap->nextTileIndex - 1] = NULL;
	tmap->tilesetsCount = 0;
	tmap->layers = NULL;
	tmap->layerCount = 0;
	tmap->tilesets = NULL;

	// cargar el archivo del mapa
	FILE* xmlFile = fopen(filename, "r");
	if (xmlFile == NULL)
	{
		printf("Error al cargar el archivo para el mapa: %s\n", name);
		free(tmap->name);
		free(tmap->tiles);
		free(tmap);
		return NULL;
	}

	// cargar el nodo raíz
	mxml_options_t* options = mxmlOptionsNew();
	mxmlOptionsSetTypeValue(options, MXML_TYPE_OPAQUE);
	mxml_node_t* tree = mxmlLoadFile(NULL, options, xmlFile);
	fclose(xmlFile);
	if (tree == NULL)
	{
		printf("Errro al cargar el nodo xml principal para el mapa: %s\n", name);
		free(tmap->name);
		free(tmap->tiles);
		free(tmap);
		mxmlOptionsDelete(options);
		return NULL;
	}

	// leer los datos del mapa y los tiles
	mxml_node_t* mapNode = mxmlFindElement(tree, tree, "map", NULL, NULL, MXML_DESCEND_ALL);
	if (mapNode == NULL) 
	{
		printf("Errro al cargar el nodo map para el mapa: %s\n", name);
		free(tmap->name);
		free(tmap->tiles);
		free(tmap);
		mxmlOptionsDelete(options);
		return NULL;
	}
	const char* wstr = mxmlElementGetAttr(mapNode, "tilewidth");
	const char* hstr = mxmlElementGetAttr(mapNode, "tileheight");
	const char* mwstr = mxmlElementGetAttr(mapNode, "width");
	const char* mhstr = mxmlElementGetAttr(mapNode, "height");

	tmap->tileWidth = atoi(wstr);
	tmap->tileHeight = atoi(hstr);
	tmap->mapWidth = atoi(mwstr);
	tmap->mapHeight = atoi(mhstr);

	// cargar los tilesets
	mxml_node_t* tilesetNode = mxmlFindElement(tree, tree, "tileset", NULL, NULL, MXML_DESCEND_ALL);
	while (tilesetNode != NULL)
	{
		Tileset* tlst = CreateTileset(tilesetNode, tmap);

		if (tlst == NULL)
		{
			printf("Error al crear un tileset para el mapa: %s\n", name);

			if (tmap->tilesetsCount > 0)
			{
				for(int i = 0; i < tmap->tilesetsCount; i++)
				{
					UnloadTexture(tmap->tilesets[i]->texture);
					free(tmap->tilesets[i]);
				}
			}

			free(tmap->name);
			free(tmap->tiles);
			free(tmap);
			mxmlOptionsDelete(options);
			return NULL;
		}

		Tileset** tilesetsMemTemp = (Tileset**)realloc(tmap->tilesets, (size_t)(tmap->tilesetsCount + 1) * sizeof(Tileset*));
		if (tilesetsMemTemp == NULL)
		{
			printf("Errro al asignar memoria para los tilesets del mapa: %s\n", name);

			if (tmap->tilesetsCount > 0)
			{
				for (int i = 0; i < tmap->tilesetsCount; i++)
				{
					UnloadTexture(tmap->tilesets[i]->texture);
					free(tmap->tilesets[i]);
				}
			}

			free(tmap->name);
			free(tmap->tiles);
			free(tmap);
			mxmlOptionsDelete(options);
			return NULL;
		}

		tmap->tilesets = tilesetsMemTemp;
		tmap->tilesets[tmap->tilesetsCount] = tlst;
		tmap->tilesetsCount += 1;

		tilesetNode = mxmlFindElement(tilesetNode, tree, "tileset", NULL, NULL, MXML_DESCEND_ALL);
	}

	// cargar las capas
	mxml_node_t* layerNode = mxmlFindElement(tree, tree, "layer", NULL, NULL, MXML_DESCEND_ALL);
	while (layerNode != NULL)
	{
		mxml_node_t* layerData = mxmlFindElement(layerNode, layerNode, "data", NULL, NULL, MXML_DESCEND_ALL);
		if (layerData == NULL)
		{
			printf("Error al crear un tileset para el mapa: %s\n", name);

			if (tmap->tilesetsCount > 0)
			{
				for (int i = 0; i < tmap->tilesetsCount; i++)
				{
					UnloadTexture(tmap->tilesets[i]->texture);
					free(tmap->tilesets[i]);
				}
			}

			free(tmap->name);
			free(tmap->tiles);
			free(tmap);
			mxmlOptionsDelete(options);
			return NULL;
		}

		TiledLayer* layer = AddLayer(layerData, tmap);
		if (layer == NULL)
		{
			printf("Error al cargar una capa para el mapa: %s\n", name);

			if (tmap->tilesetsCount > 0)
			{
				for (int i = 0; i < tmap->tilesetsCount; i++)
				{
					UnloadTexture(tmap->tilesets[i]->texture);
					free(tmap->tilesets[i]);
				}
			}

			if (tmap->layerCount > 0)
			{
				for (int i = 0; i < tmap->layerCount; i++)
				{
					free(tmap->layers[i]->data);
				}
			}

			free(tmap->name);
			free(tmap->tiles);
			free(tmap);
			mxmlOptionsDelete(options);
			return NULL;
		}

		TiledLayer** layersMemTemp = (TiledLayer**)malloc((size_t)(tmap->layerCount + 1) * sizeof(TiledLayer*));
		if (layersMemTemp == NULL)
		{
			printf("Error al asignar memoria de capas para el mapa: %s\n", name);

			if (tmap->tilesetsCount > 0)
			{
				for (int i = 0; i < tmap->tilesetsCount; i++)
				{
					UnloadTexture(tmap->tilesets[i]->texture);
					free(tmap->tilesets[i]);
				}
			}

			if (tmap->layerCount > 0)
			{
				for (int i = 0; i < tmap->layerCount; i++)
				{
					free(tmap->layers[i]->data);
				}
			}

			free(tmap->name);
			free(tmap->tiles);
			free(tmap);
			mxmlOptionsDelete(options);
			return NULL;
		}

		tmap->layers = layersMemTemp;
		tmap->layers[tmap->layerCount] = layer;
		tmap->layerCount += 1; // hacer a visual studio feliz

		layerNode = mxmlFindElement(layerNode, tree, "layer", NULL, NULL, MXML_DESCEND_ALL);
	}

	// crear textura
	tmap->mapTexture = LoadRenderTexture(
		tmap->tileWidth * tmap->mapWidth,
		tmap->tileHeight * tmap->mapHeight
	);

	// dibujar las capas
	BeginTextureMode(tmap->mapTexture);
	ClearBackground(BLANK);

	if (tmap->layerCount > 0 && tmap->tiles != NULL)
	{
		for (int i = 0; i < tmap->layerCount; i++)
		{
			int tileIndex = 0;
			for (int ty = 0; ty < tmap->mapHeight; ty++)
			{
				int destY = ty * tmap->tileHeight;
				int destX = 0;
				for (int tx = 0; tx < tmap->mapWidth; tx++)
				{
					destX = tx * tmap->tileWidth;
					if (tmap->layers[i]->data[tileIndex] != 0)
					{
						Tileset* tilesetForTexture = GetTileset(tmap, tmap->layers[i]->data[tileIndex]);
						if (tilesetForTexture != NULL)
						{
							if (tmap->tiles[tmap->layers[i]->data[tileIndex]] != NULL)
							{
								Rectangle src = {
									tmap->tiles[tmap->layers[i]->data[tileIndex]]->x,
									tmap->tiles[tmap->layers[i]->data[tileIndex]]->y,
									tmap->tiles[tmap->layers[i]->data[tileIndex]]->width,
									tmap->tiles[tmap->layers[i]->data[tileIndex]]->height
								};

								Rectangle dst = {
									destX, destY,
									tmap->tiles[tmap->layers[i]->data[tileIndex]]->width,
									tmap->tiles[tmap->layers[i]->data[tileIndex]]->height
								};

								Vector2 origin = { 0.0f, 0.0f };
								DrawTexturePro(tilesetForTexture->texture, src, dst, origin, 0.0f, WHITE);
							}
						}
					}
					tileIndex++;
				}
			}
		}
	}

	EndTextureMode();

	mxmlOptionsDelete(options);
	mxmlDelete(tree);
	return tmap;
}

Tileset* CreateTileset(mxml_node_t* node, TileMap* map)
{
	if (node == NULL) return NULL;

	const char* tilesetFilename = mxmlElementGetAttr(node, "source");
	if (tilesetFilename == NULL) return NULL;

	Tileset* tileset = (Tileset*)malloc(sizeof(Tileset));
	if (!tileset) return NULL;

	tileset->initialIndex = atoi(mxmlElementGetAttr(node, "firstgid"));

	// cargar el archivo tsx
	FILE* xmlFile = fopen(tilesetFilename, "r");
	if (xmlFile == NULL)
	{
		free(tileset);
		return NULL;
	}

	mxml_options_t* options = mxmlOptionsNew();
	mxmlOptionsSetTypeValue(options, MXML_TYPE_OPAQUE);
	mxml_node_t* root = mxmlLoadFile(NULL, options, xmlFile);
	fclose(xmlFile);
	if (root == NULL)
	{
		free(tileset);
		mxmlOptionsDelete(options);
		return NULL;
	}

	mxml_node_t* data = mxmlFindElement(root, root, "tileset", NULL, NULL, MXML_DESCEND_ALL);
	if (data == NULL)
	{
		free(tileset);
		mxmlOptionsDelete(options);
		mxmlDelete(root);
		return NULL;
	}

	tileset->finalIndex = tileset->initialIndex + (atoi(mxmlElementGetAttr(data, "tilecount")) - 1);
	mxml_node_t* textureNode = mxmlFindElement(data, root, "image", NULL, NULL, MXML_DESCEND_ALL);
	if (textureNode == NULL)
	{
		free(tileset);
		mxmlOptionsDelete(options);
		mxmlDelete(root);
		return NULL;
	}

	const char* textureFilename = mxmlElementGetAttr(textureNode, "source");
	tileset->texture = LoadTexture(textureFilename);
	if (tileset->texture.id == 0)
	{
		free(tileset);
		mxmlOptionsDelete(options);
		mxmlDelete(root);
		return NULL;
	}

	// cargar los tiles
	int columns = tileset->texture.width / map->tileWidth;
	int rows = tileset->texture.height / map->tileHeight;
	for (int j = 0; j < rows; j++)
	{
		for (int i = 0; i < columns; i++)
		{
			Tile** memTemp = (Tile**)realloc(map->tiles, (size_t)(map->nextTileIndex + 1) * sizeof(Tile*));
			if (memTemp != NULL)
			{
				map->tiles = memTemp;
				Tile* tile = (Tile*)malloc(sizeof(Tile));
				if (tile != NULL)
				{
					tile->x = i * map->tileWidth;
					tile->y = j * map->tileHeight;
					tile->width = map->tileWidth;
					tile->height = map->tileHeight;
					map->tiles[map->nextTileIndex] = tile;
					map->nextTileIndex += 1; // hace al visual studio feliz :D
				}
				else
				{
					printf("Error al asignar memoria para un tile.\n");
				}
			}
			else
			{
				printf("Error al asignar memoria para un tileset.\n");
			}
		}
	}

	mxmlOptionsDelete(options);
	mxmlDelete(root);

	return tileset;
}

TiledLayer* AddLayer(mxml_node_t* data, TileMap* map)
{
	const char* opaqueData = mxmlGetOpaque(data);
	if (opaqueData == NULL) return NULL;

	char* dataCopy = strdup(opaqueData);
	if (dataCopy == NULL) return NULL;

	TiledLayer* layer = (TiledLayer*)malloc(sizeof(TiledLayer));
	if (layer == NULL) return NULL;

	layer->width = map->mapWidth;
	layer->height = map->mapHeight;
	layer->dataCount = layer->width * layer->height;
	layer->data = (int*)malloc((size_t)layer->dataCount * sizeof(int));
	if (layer->data == NULL)
	{
		free(layer);
		return NULL;
	}
	int index = 0;
	char* context = NULL; // hace a visual studio feliz :D
	char* token = strtok_s(dataCopy, ",", &context);
	while (token)
	{
		layer->data[index] = atoi(token);
		index++;
		token = strtok_s(NULL, ",", &context);
	}

	free(dataCopy);
	return layer;
}

TileMapNode* InsertTileMapNode(TileMapNode* node, TileMap* tmap)
{
	if (node == NULL)
	{
		TileMapNode* newNode = (TileMapNode*)malloc(sizeof(TileMapNode));
		if (!newNode)
		{
			return NULL;
		}

		newNode->tmap = tmap;
		newNode->left = NULL;
		newNode->right = NULL;
		return newNode;
	}

	int cmp = strcmp(tmap->name, node->tmap->name);
	if (cmp < 0)
		node->left = InsertTileMapNode(node->left, tmap);
	else if (cmp > 0)
		node->right = InsertTileMapNode(node->right, tmap);
	else
		printf("El tilemap '%s' ya existe.\n", tmap->name);

	return node;
}

TileMap* TreeTileMapSearch(TileMapNode* node, const char* name)
{
	if (node == NULL) return NULL;

	int cmp = strcmp(name, node->tmap->name);
	if (cmp == 0)
		return node->tmap;
	else if (cmp < 0)
		return TreeTileMapSearch(node->left, name);
	else
		return TreeTileMapSearch(node->right, name);

	return NULL;
}

Tileset* GetTileset(TileMap* tmap, unsigned int index)
{
	if (tmap->tilesetsCount > 0)
	{
		for (int i = 0; i < tmap->tilesetsCount; i++)
		{
			if (index >= tmap->tilesets[i]->initialIndex && index <= tmap->tilesets[i]->finalIndex)
				return tmap->tilesets[i];
		}
	}

	return NULL;
}

void UnloadTreeTileMapNode(TileMapNode* node)
{
	if (node == NULL) return;

	UnloadTreeTileMapNode(node->left);
	UnloadTreeTileMapNode(node->right);

	free(node->tmap->name);
	// tilesets
	if (node->tmap->tilesetsCount > 0)
	{
		for (int i = 0; i < node->tmap->tilesetsCount; i++)
		{
			UnloadTexture(node->tmap->tilesets[i]->texture);
			free(node->tmap->tiles[i]);
		}
		free(node->tmap->tilesets);
	}
	// tiles
	if (node->tmap->nextTileIndex > 0)
	{
		for (int i = 0; i < node->tmap->nextTileIndex; i++)
		{
			free(node->tmap->tiles[i]);
		}
		free(node->tmap->tiles);
	}
	// layer
	if (node->tmap->layerCount > 0)
	{
		for (int i = 0; i < node->tmap->layerCount; i++)
		{
			free(node->tmap->layers[i]->data);
			free(node->tmap->layers[i]);
		}
		free(node->tmap->layers);
	}
	// RenderTexture
	UnloadRenderTexture(node->tmap->mapTexture);
	
	free(node->tmap);
	free(node);
}
*/
