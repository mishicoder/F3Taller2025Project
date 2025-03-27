#include "tilemap.h"

TilesetPack* CreateTilesetPack(const char *name, const char *filename)
{
  TilesetPack* pack = (TilesetPack*)malloc(sizeof(TilesetPack));
  if(!pack) return NULL;

  FILE* tilesetsFile;
  if(!fopen_s(&tilesetsFile,filename, "r")){ free(pack); return NULL; }

  pack->name = _strdup(name);
  pack->nextTilesetIndex = 1;
  pack->tilesetsCount = 0;
  pack->tileCount = 0;
  pack->tiles = NULL;
  pack->tilesets = NULL;

  bool hasError = false;
  char buffer[1024];
  while(fgets(buffer, sizeof(buffer), tilesetsFile) != NULL)
  {
    size_t len = strlen(buffer);
    if(len > 0 && buffer[len - 1] == '\n')
      buffer[len - 1] = '\0';

    if(strlen(buffer) > 0 && buffer[0] != '#')
    {
      char* line = _strdup(buffer);
      if(line != NULL)
      {
        if(strcmp(line, "@break") == 0){ free(line); break; }
        
        char* context = NULL;
        char* key = strtok_s(line, "@", &context);
        char* val = strtok_s(NULL, "@", &context);
        if(key != NULL && val != NULL)
        {
          if(strcmp(key, "ts") == 0)
          {
            Tileset* newTileset = CreateTileset(val, pack->nextTilesetIndex);
            if(newTileset != NULL)
            {
              Tileset** memTemp = (Tileset**)realloc(pack->tilesets, (size_t)(pack->tilesetsCount + 1) * sizeof(Tileset*));
              if(memTemp)
              {
                pack->tilesets = memTemp;
                pack->tilesets[pack->tilesetsCount] = newTileset;
                pack->tilesetsCount += 1;
                pack->nextTilesetIndex = newTileset->finalIndex + 1;
              }
              else
              {
                free(newTileset);
                hasError = true;
              }
            }
            else
            {
              hasError = true;  
            }
          }
        }
        free(line);
      } 
    }
  }

  // handle errors 
  if(hasError)
  {
    if(pack->tilesetsCount > 0)
    {
      for(int i = 0; i < pack->tilesetsCount; i++)
      {
        if(pack->tilesets[i])
        {
          UnloadTexture(pack->tilesets[i]->texture);
          free(pack->tilesets[i]);
        }
      }
      free(pack->tilesets);
    }
    free(pack);
    return NULL;
  }

  if(pack->tilesetsCount == 0){ free(pack); return NULL; }

  // generate all tiles
  int totalTiles = 0;
  for(int i = 0; i < pack->tilesetsCount; i++)
    totalTiles += pack->tilesets[i]->tileCount;

  Tile** tMemTemp = (Tile**)malloc((size_t)(totalTiles + 1)  * sizeof(Tile*));
  if(!tMemTemp)
  {
    for(int i = 0; i < pack->tileCount; i++)
    {
      if(pack->tilesets[i])
      {
        UnloadTexture(pack->tilesets[i]->texture);
        free(pack->tilesets[i]);
      }
    }
    if(pack->tilesets) free(pack->tilesets);
    free(pack);
    return NULL;
  }
  pack->tiles = tMemTemp;
  pack->tiles[pack->tileCount] = NULL;
  pack->tileCount += 1;
  for(int i = 0; i < pack->tilesetsCount; i++)
  {
    int rows = pack->tilesets[i]->texture.height / pack->tilesets[i]->tileHeight;
    int cols = pack->tilesets[i]->texture.width / pack->tilesets[i]->tileWidth;
    for(int ty = 0; ty < rows; ty++)
    {
      for(int tx = 0; tx < cols; tx++)
      {
        Tile* newTile = (Tile*)malloc(sizeof(Tile));
        if(newTile)
        {
          newTile->x = tx * pack->tilesets[i]->tileWidth;
          newTile->y = ty * pack->tilesets[i]->tileHeight;
          newTile->width = pack->tilesets[i]->tileWidth;
          newTile->height = pack->tilesets[i]->tileHeight;
          pack->tiles[pack->tileCount] = newTile;
        }
        else
          pack->tiles[pack->tileCount] = NULL;
        pack->tileCount += 1;
      }
    }
  }

  return pack;
}

bool AddTilesetToPack(TilesetPack *pack, const char *filename)
{
  if(!pack) return false;

  Tileset* tileset = CreateTileset(filename, pack->nextTilesetIndex);
  if(!tileset) return false;

  Tileset** memTemp = (Tileset**)realloc(pack->tilesets, (size_t)(pack->tilesetsCount + 1) * sizeof(Tileset*));
  if(!memTemp) { UnloadTexture(tileset->texture); free(tileset); return false; }

  pack->tilesets = memTemp;
  pack->tilesets[pack->tilesetsCount + 1] = tileset;
  pack->tilesetsCount += 1;
  pack->nextTilesetIndex = tileset->finalIndex + 1;

  return true;
}

TilesetPackNode* InsertTilesetPackNode(TilesetPackNode *node, TilesetPack *pack)
{
  if(!node)
  {
    TilesetPackNode* newNode = (TilesetPackNode*)malloc(sizeof(TilesetPackNode));
    if(!newNode) return NULL;

    newNode->pack = pack;
    newNode->left = NULL;
    newNode->right = NULL;
    return newNode;
  }

  int result = strcmp(pack->name, node->pack->name);
  if(result < 0)
    node->left = InsertTilesetPackNode(node->left, pack);
  else if (result > 0)
    node->right = InsertTilesetPackNode(node->right, pack);

  return node;
}

TilesetPack* SearchTilesetPackInTree(TilesetPackNode *node, const char *name)
{
  if(!node) return NULL;

  int result = strcmp(name, node->pack->name);
  if(result == 0)
    return node->pack;
  if(result < 0)
    return SearchTilesetPackInTree(node->left, name);
  else
    return SearchTilesetPackInTree(node->right, name);

  return NULL;
}

Tileset* CreateTileset(const char *filename, int initialIndex)
{
  FILE* xmlFile;
  if(!fopen_s(&xmlFile, filename, "r"))
    return NULL;

  Tileset* tileset = (Tileset*)malloc(sizeof(Tileset));
  if(!tileset){ fclose(xmlFile); return NULL; }

  tileset->initialIndex = initialIndex;

  mxml_options_t* options = mxmlOptionsNew();
  mxmlOptionsSetTypeValue(options, MXML_TYPE_OPAQUE);
  mxml_node_t* root = mxmlLoadFile(NULL, options, xmlFile);
  fclose(xmlFile);

  if(!root){ free(tileset); mxmlOptionsDelete(options); return NULL; }

  // tilset node
  mxml_node_t* tsNode = mxmlFindElement(root, root, "tileset", NULL, NULL, MXML_DESCEND_ALL);
  if(!tsNode) { free(tileset); mxmlOptionsDelete(options); mxmlDelete(root); return NULL; }

  // onbtener los atributos
  int tileWidth = atoi(mxmlElementGetAttr(tsNode, "tileWidth"));
  int tileHeight = atoi(mxmlElementGetAttr(tsNode, "tileHeight"));
  tileset->tileWidth = tileWidth;
  tileset->tileHeight = tileHeight;
  tileset->finalIndex = (tileset->initialIndex + atoi(mxmlElementGetAttr(tsNode, "tileCount"))) - 1;
  tileset->tileCount = atoi(mxmlElementGetAttr(tsNode, "tileCount"));

  // ruta de la textura
  mxml_node_t* textureNode = mxmlFindElement(tsNode, root, "image", NULL, NULL, MXML_DESCEND_ALL);
  const char* texFilename = mxmlElementGetAttr(textureNode, "source");
  tileset->texture = LoadTexture(texFilename);
  if(tileset->texture.id == 0){ free(tileset); mxmlOptionsDelete(options); mxmlDelete(root); return NULL; }

  // unload memrory data
  mxmlOptionsDelete(options);
  mxmlDelete(root);

  return tileset;
}

Tileset* GetTileset(TilesetPack *pack, int index)
{
  if(!pack) return NULL;

  for(int i = 0; i < pack->tilesetsCount; i++)
  {
    if(pack->tilesets[i])
    {
      if(index >= pack->tilesets[i]->initialIndex && index <= pack->tilesets[i]->finalIndex)
        return pack->tilesets[i];
    }
  }

  return NULL;
}

MapLayer* CreateLayer(mxml_node_t *data, int mapWidth, int mapHeight)
{
  if(!data) return NULL;
  
  const char* opaqueData = mxmlGetOpaque(data);
  if(!opaqueData) return NULL;

  char* dataCopy = _strdup(opaqueData);
  if(!dataCopy){ return NULL; }

  MapLayer* layer = (MapLayer*)malloc(sizeof(MapLayer));
  if(!layer){ free(dataCopy); return NULL; }

  layer->width = mapWidth;
  layer->height = mapHeight;
  layer->dataCount = mapWidth * mapHeight;
  layer->data = (int*)malloc((size_t)(layer->dataCount) * sizeof(int));
  if(!layer->data){ free(dataCopy); free(layer); return NULL; }
  int index = 0;
  char* context = NULL;
  char* token = strtok_s(dataCopy, ",", &context);
  while(token)
  {
    layer->data[index] = atoi(token);
    index++;
    token = strtok_s(NULL, ",", &context);
  }

  free(dataCopy);
  return layer;
}

TileMap* CreateMap(const char *name, const char *filename, TilesetPack *pack)
{
  TileMap* map = (TileMap*)malloc(sizeof(TileMap));
  if(!map) return NULL;

  FILE* xmlFile;
  if(!fopen_s(&xmlFile, filename, "r")){ free(map); return NULL; }

  // root node
  mxml_options_t* options = mxmlOptionsNew();
  mxmlOptionsSetTypeValue(options, MXML_TYPE_OPAQUE);
  mxml_node_t* root = mxmlLoadFile(NULL, options, xmlFile);
  fclose(xmlFile);
  if(!root){ free(map); mxmlOptionsDelete(options); return NULL; }

  // map node
  mxml_node_t* nodeMap = mxmlFindElement(root, root, "map", NULL, NULL, MXML_DESCEND_ALL);
  if(!nodeMap){ free(map); mxmlOptionsDelete(options); mxmlDelete(root); return NULL; }

  map->name = _strdup(name);
  map->layers = NULL;
  map->layersCount = 0;
  map->tileWidth = atoi(mxmlElementGetAttr(nodeMap, "tileWidth"));
  map->tileHeight = atoi(mxmlElementGetAttr(nodeMap, "tileHeight"));
  map->mapWidth = atoi(mxmlElementGetAttr(nodeMap, "width"));
  map->mapHeight = atoi(mxmlElementGetAttr(nodeMap, "height"));

  bool anyError = false;

  // carga de las capas
  mxml_node_t* nodeLayer = mxmlFindElement(root, root, "layer", NULL, NULL, MXML_DESCEND_ALL);
  while(nodeLayer)
  {
    mxml_node_t* ldata = mxmlFindElement(nodeLayer, nodeLayer, "data", NULL, NULL, MXML_DESCEND_ALL);
    if(!ldata){ anyError = true; continue; }

    MapLayer* layer = CreateLayer(ldata, map->mapWidth, map->mapHeight);
    if(!layer){ anyError = true; continue; }

    MapLayer** lMemTemp = (MapLayer**)realloc(map->layers, (size_t)(map->layersCount + 1) * sizeof(MapLayer*));
    if(!lMemTemp){ free(layer->data); free(layer); anyError = true; continue; }

    map->layers = lMemTemp;
    map->layers[map->layersCount] = layer;
    map->layersCount += 1;

    nodeLayer = mxmlFindElement(nodeLayer, root, "layer", NULL, NULL, MXML_DESCEND_ALL);
  }

  if(anyError || map->layersCount == 0)
  {
    if(map->layersCount > 0)
    {
      for(int i = 0; i < map->layersCount; i++)
      {
        if(map->layers[i])
        {
          free(map->layers[i]->data);
          free(map->layers[i]);
        }
      }
      free(map->layers);
    }
    free(map);
    mxmlOptionsDelete(options);
    mxmlDelete(root);
    return NULL;
  }

  // generate texture
  map->texture = LoadRenderTexture(map->tileWidth * map->mapWidth, map->tileHeight * map->mapHeight);
  BeginTextureMode(map->texture);
  ClearBackground(BLANK);

  for(int i = 0; i < map->layersCount; i++)
  {
    int tileIndex = 0;
    for(int ty = 0; ty < map->mapHeight; ty++)
    {
      for(int tx = 0; tx < map->mapWidth; tx++)
      {
        Tileset* tileset = GetTileset(pack, map->layers[i]->data[tileIndex]);
        if(tileset)
        {
          int x = pack->tiles[map->layers[i]->data[tileIndex]]->x;
          int y = pack->tiles[map->layers[i]->data[tileIndex]]->y;
          int w = pack->tiles[map->layers[i]->data[tileIndex]]->width;
          int h = pack->tiles[map->layers[i]->data[tileIndex]]->height;
          Rectangle src = { .x = x, .y = y, .width = w, .height = h };
          Rectangle dst = { .x = tx * w, .y = ty * h, w, h };
          DrawTexturePro(tileset->texture, src, dst, (Vector2){0,0}, 0.0, WHITE);
        }
      }
      tileIndex++;
    }
  }

  EndTextureMode();

  mxmlOptionsDelete(options);
  mxmlDelete(root);
  return map;
}

TileMapNode* InsertTileMapNode(TileMapNode *node, TileMap *map)
{
  if(node == NULL)
  {
    TileMapNode* newNode = (TileMapNode*)malloc(sizeof(TileMapNode));
    if(!newNode) return NULL;

    newNode->map = map;
    newNode->left = NULL;
    newNode->right = NULL;
    return newNode;
  }

  int result = strcmp(map->name, node->map->name);
  if(result < 0)
    return InsertTileMapNode(node->left, map);
  else if (result > 0)
    return InsertTileMapNode(node->right, map);

  return node;
}

TileMap* SearchTileMapInTree(TileMapNode *node, const char *name)
{
  if(!node) return NULL;

  int result = strcmp(name, node->map->name);
  if(result == 0) return node->map;
  else if(result < 0) return SearchTileMapInTree(node->left, name);
  else return SearchTileMapInTree(node->right, name);

  return NULL;
}

void UnloadTilesetPack(TilesetPack *pack)
{
  if(!pack) return;

  for(int i = 0; i < pack->tileCount; i++)
  {
    if(pack->tiles[i]) free(pack->tiles[i]);
  }
  if(pack->tiles) free(pack->tiles);

  for(int i = 0; i < pack->tilesetsCount; i++)
  {
    if(pack->tilesets[i]){ UnloadTexture(pack->tilesets[i]->texture); free(pack->tilesets[i]); }
  }
  if(pack->tilesets) free(pack->tilesets);

  free(pack->name);
  free(pack);
}

void UnloadTilesetPackNode(TilesetPackNode *node)
{
  if(!node) return;

  UnloadTilesetPackNode(node->left);
  UnloadTilesetPackNode(node->right);

  UnloadTilesetPack(node->pack);
  free(node);
}

void UnloadTileMapNode(TileMapNode *node)
{
  if(!node) return;

  UnloadTileMapNode(node->left);
  UnloadTileMapNode(node->right);

  for(int i = 0; i < node->map->layersCount; i++)
  {
    free(node->map->layers[i]->data);
    free(node->map->layers[i]);
  }
  if(node->map->layers) free(node->map->layers);

  free(node->map->name);
  free(node->map);
  free(node);
}
