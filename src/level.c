#include "level.h"

Level* CreateLevel(const char* name, bool keepInMemory, bool renderInStack, bool updateInStack, void(*LoadLevel)(struct Game* game, struct Level* level), void(*UnloadLevel)(struct Game* game, Level* level), void(*InitEntities)(struct Game* game, struct Level* level))
{
  Level* level = (Level*)malloc(sizeof(Level));
  if(!level) return NULL;

  level->world = ecs_init();
  if(!level->world){ free(level); return NULL; }

  level->name = _strdup(name);
  if(!level->name){ ecs_fini(level->world); free(level); return NULL; }

  level->destroyQueueCount = 0;

  level->keepInMemory = keepInMemory;
  level->renderInStack = renderInStack;
  level->updateInStack = updateInStack;
  level->LoadLevel = LoadLevel;
  level->InitEntities = InitEntities;
  level->UnloadLevel = UnloadLevel;

  return level;
}
