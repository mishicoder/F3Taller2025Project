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

	unsigned int hasError = 0; // indica que no existen errores en la carga
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
						Tileset* newTileset = CreateTileset(val, pack->nextTilesetIndex);
						if (newTileset != NULL)
						{
							Tileset** tsMemTemp = (Tileset**)realloc(pack->tilesets, (size_t)(pack->tilesetsCount + 1) * sizeof(Tileset*));
							if (tsMemTemp != NULL)
							{
								pack->tilesets = tsMemTemp;
								pack->tilesets[pack->tilesetsCount] = newTileset;
								pack->tilesetsCount += 1;
								pack->nextTilesetIndex = newTileset->finalIndex + 1;
							}
							else
							{
								printf("Imposible asignar memoria para el tileset. El tileset no será agregado.\n");
								hasError += 1;
							}
						}
						else 
						{
							printf("Error al cartar el tileset {%s}.\n", val);
							hasError += 1;
						}
					}
				}

				free(line);
			}
		}
	}

	if (hasError > 0)
	{
		printf("Se han encontrado errores en la carga, imposible generar el paquete de conjuntos de mosaicos.\n");

		if(pack->tilesetsCount > 0)
		{
			for (int i = 0; i < pack->tilesetsCount; i++)
			{
				if (pack->tilesets[i] != NULL)
					free(pack->tilesets[i]);
			}
			free(pack);
		}

		return NULL;
	}

	if (pack->tilesetsCount == 0)
	{
		printf("No se han encontrado conjuntos de mosaicos en el archivo, imposible crear un paquete vacio.\n");
		free(pack);
		return NULL;
	}

	// generar los tilests

	return pack;
}

int InsertTilesetToPack(TilesetPack* pack, const char* filename)
{
	Tileset* tileset = CreateTileset(filename, pack->nextTilesetIndex);
	if (tileset == NULL) return 0;

	Tileset** tsMemTemp = (Tileset**)realloc(pack->tilesets, (size_t)(pack->tilesetsCount + 1) * sizeof(Tileset*));
	if (tsMemTemp == NULL) return 0;

	pack->tilesets = tsMemTemp;
	pack->tilesets[pack->tilesetsCount + 1] = tileset;
	pack->tilesetsCount += 1;
	pack->nextTilesetIndex = tileset->finalIndex + 1;

	return 1;
}

Tileset* GetTileset(TilesetPack* pack, unsigned int index)
{
	if (pack == NULL) return NULL;

	for (int i = 0; i < pack->tilesetsCount; i++)
	{
		if (index >= pack->tilesets[i]->initialIndex && index <= pack->tilesets[i]->finalIndex)
			return pack->tilesets[i];
	}

	return NULL;
}

TilesetPackNode* InsertTilesetPackNode(TilesetPackNode* node, TilesetPack* pack)
{
	if (node == NULL)
	{
		TilesetPackNode* newNode = (TilesetPackNode*)malloc(sizeof(TilesetPackNode));
		if(newNode == NULL) 
		{
			printf("Imposible crear el nodo para el paquete {%s}.\n", pack->name);
			return NULL;
		}

		newNode->pack = pack;
		newNode->left = NULL;
		newNode->right = NULL;
		return newNode;
	}

	int nameCompare = strcmp(pack->name, node->pack->name);
	if (nameCompare < 0)
		node->left = InsertTilesetPackNode(node->left, pack);
	else if (nameCompare > 0)
		node->right = InsertTilesetPackNode(node->right, pack);
	else
		printf("El paquete de conjunto de mosaicos {%s} ya existe.\n", pack->name);

	return node;
}

TilesetPack* TreeTilesetPackSearch(TilesetPackNode* node, const char* name)
{
	if (node == NULL) return NULL;

	int nameCompare = strcmp(name, node->pack->name);
	if (nameCompare == 0)
		return node->pack;
	else if (nameCompare < 0)
		return TreeTilesetPackSearch(node->left, name);
	else
		return TreeTilesetPackSearch(node->right, name);

	return NULL;
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

TileMap* CreateMap(const char* name, const char* filename, TilesetPack* pack)
{
	TileMap* tmap = (TileMap*)malloc(sizeof(TileMap));
	if (tmap == NULL)
	{
		printf("Error al asignar memoria para el tilemap: %s\n", name);
		return NULL;
	}

	tmap->name = name;
	tmap->layers = NULL;
	tmap->layerCount = 0;

	// cargar el archivo del mapa
	FILE* xmlFile = fopen(filename, "r");
	if (xmlFile == NULL)
	{
		printf("Error al cargar el archivo para el mapa: %s\n", name);
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
		free(tmap);
		mxmlOptionsDelete(options);
		return NULL;
	}

	// leer los datos del mapa y los tiles
	mxml_node_t* mapNode = mxmlFindElement(tree, tree, "map", NULL, NULL, MXML_DESCEND_ALL);
	if (mapNode == NULL)
	{
		printf("Error al cargar el nodo map para el mapa: %s\n", name);
		free(tmap);
		mxmlOptionsDelete(options); 
		mxmlDelete(tree);
		return NULL;
	}

	// Datos del mapa
	tmap->tileWidth = atoi(mxmlElementGetAttr(mapNode, "tilewidth"));
	tmap->tileHeight = atoi(mxmlElementGetAttr(mapNode, "tileheight"));
	tmap->mapWidth = atoi(mxmlElementGetAttr(mapNode, "width"));
	tmap->mapHeight = atoi(mxmlElementGetAttr(mapNode, "height"));

	// cargar las capas
	unsigned int hasErrors = 0;
	mxml_node_t* layerNode = mxmlFindElement(tree, tree, "layer", NULL, NULL, MXML_DESCEND_ALL);
	while (layerNode != NULL)
	{
		mxml_node_t* layerData = mxmlFindElement(layerNode, layerNode, "data", NULL, NULL, MXML_DESCEND_ALL);
		if (layerData == NULL) hasErrors++;

		TiledLayer* layer = CreateLayer(layerData, tmap->mapWidth, tmap->mapHeight);
		if (layer == NULL) hasErrors++;

		TiledLayer** layersMemTemp = (TiledLayer**)malloc((size_t)(tmap->layerCount + 1) * sizeof(TiledLayer*));
		if (layersMemTemp != NULL && layer != NULL) 
		{
			tmap->layers = layersMemTemp;
			tmap->layers[tmap->layerCount] = layer;
			tmap->layerCount += 1; // hacer a visual studio feliz	
		}
		else
			hasErrors++;
		

		layerNode = mxmlFindElement(layerNode, tree, "layer", NULL, NULL, MXML_DESCEND_ALL);
	}

	if (hasErrors > 0)
	{
		printf("Se han encontrado {%d} en la carga, imposible crear el mapa.\n", hasErrors);
		if (tmap->layerCount > 0)
		{
			for (int i = 0; i < tmap->layerCount; i++)
			{
				if (tmap->layers[i] != NULL)
					free(tmap->layers[i]);
			}
		}
		free(tmap);
		mxmlOptionsDelete(options);
		mxmlDelete(tree);
		return NULL;
	}
	if (tmap->layerCount == 0)
	{
		printf("No se han encontrado capas para el mapa {%s}, imposible crear un mapa sin capas.\n", tmap->name);
		if (tmap->layerCount > 0)
		{
			for (int i = 0; i < tmap->layerCount; i++)
			{
				if (tmap->layers[i] != NULL)
					free(tmap->layers[i]);
			}
		}
		free(tmap);
		mxmlOptionsDelete(options);
		mxmlDelete(tree);
		return NULL;
	}

	// crear textura
	tmap->mapTexture = LoadRenderTexture(
		tmap->tileWidth * tmap->mapWidth,
		tmap->tileHeight * tmap->mapHeight
	);

	// Dibujar las capas
	BeginTextureMode(tmap->mapTexture);
	ClearBackground(BLANK);

	for (int i = 0; i < tmap->layerCount; i++)
	{
		unsigned int tileIndex = 0;
		for (int ty = 0; ty < tmap->mapHeight; ty++)
		{
			for (int tx = 0; tx < tmap->mapWidth; tx++)
			{
				// Cero indica que el espacio está vacío.
				if (tmap->layers[i]->data[tileIndex] != 0)
				{
					Tileset* tileset = GetTileset(pack, tmap->layers[i]->data[tileIndex]);
					if (tileset != NULL)
					{
						if (pack->tiles[tmap->layers[i]->data[tileIndex]] != NULL)
						{
							Rectangle src = {
								pack->tiles[tmap->layers[i]->data[tileIndex]]->x,
								pack->tiles[tmap->layers[i]->data[tileIndex]]->y,
								tmap->tileWidth,
								tmap->tileHeight
							};
							Rectangle dst = {
								tx * tmap->tileWidth,
								ty * tmap->tileHeight,
								tmap->tileWidth,
								tmap->tileHeight
							};

							DrawTexturePro(tileset->texture, src, dst, (Vector2) { 0.0f, 0.0f }, 0.0f, WHITE);
						}
						tileIndex++;
					}
				}
			}
		}
	}

	EndTextureMode();

	mxmlOptionsDelete(options);
	mxmlDelete(tree);
	return tmap;
}

TiledLayer* CreateLayer(mxml_node_t* data, unsigned int mapWidth, unsigned int mapHeight)
{
	if (data == NULL) return NULL;

	const char* opaqueData = mxmlGetOpaque(data);
	if (opaqueData == NULL) return NULL;

	char* dataCopy = strdup(opaqueData);
	if (dataCopy == NULL) return NULL;

	TiledLayer* layer = (TiledLayer*)malloc(sizeof(TiledLayer));
	if (layer == NULL) return NULL;

	layer->width = mapWidth;
	layer->height = mapHeight;
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

TileMapNode* InsertTileMapNode(TileMapNode* node, TileMap* map)
{
	if (node == NULL)
	{
		TileMapNode* newNode = (TileMapNode*)malloc(sizeof(TileMapNode));
		if (newNode == NULL) 
		{
			printf("Imposible crear el nodo para el mapa {%s}.\n", map->name);
			return NULL;
		}

		newNode->tmap = map;
		newNode->left = NULL;
		newNode->right = NULL;
		return newNode;
	}

	int nameCompare = strcmp(map->name, node->tmap->name);
	if (nameCompare < 0)
		return InsertTileMapNode(node->left, map);
	else if (nameCompare > 0)
		return InsertTileMapNode(node->right, map);
	else
		printf("El TileMap con el nombre {%s} ya existe.\n", map->name);

	return node;
}

TileMap* TreeTileMapSearch(TileMapNode* node, const char* name)
{
	if (node == NULL) return NULL;

	int nameCompare = strcmp(name, name, node->tmap->name);
	if (nameCompare == 0)
		return node->tmap;
	else if (nameCompare < 0)
		return TreeTileMapSearch(node->left, name);
	else
		return TreeTileMapSearch(node->right, name);

	return NULL;
}

void UnloadTilesetsPack(TilesetPack* pack)
{
	if (pack == NULL) return;

	// eliminar tiles
	if (pack->tileCount > 0)
	{
		for (int i = 0; i < pack->tileCount; i++)
		{
			if (pack->tiles[i] != NULL)
				free(pack->tiles[i]);
		}
		free(pack->tiles);
	}
}
