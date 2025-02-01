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
	if (nameCompare > 0)
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
	if (nameCompare < 0)
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
