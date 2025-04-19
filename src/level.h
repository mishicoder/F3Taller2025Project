#pragma once
#ifndef LEVEL_H
#define LEVEL_H

#include <flecs.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// de momento, solo 1024 entidades pueden ser destruidar en cada frame
// su futura mejora puede resultar en una lista enlazada en base a identificadores
// otra opción es emplear un array numérico dinámico haciendo uso de memoria dinámkca.
#define MAX_ENTITY_DESTROY_QUEUE 1024

struct Game;

typedef struct Level
{
  char* name;
  bool keepInMemory;
  bool renderInStack;
  bool updateInStack;
  ecs_world_t* world;

  // entities deletion
  ecs_entity_t destroyQueue[MAX_ENTITY_DESTROY_QUEUE];
  int destroyQueueCount;

  void(*LoadLevel)(struct Game* game, struct Level* level);
  void(*InitEntities)(struct Game* game, struct Level* level);
  void(*UnloadLevel)(struct Game* game, struct Level* level);
  void(*UpdateLevel)(struct Game* game, struct Level* level);
  void(*RenderLevel)(struct Game* game, struct Level* level);
  // Elimina las entidades pendientes a eliminar en el siguiente cuadro
  void(*FlushEntities)(struct Level* level);
} Level;

Level* CreateLevel(const char* name, bool keepInMemory, bool renderInStack, bool updateInStack, void(*LoadLevel)(struct Game* game, struct Level* level), void(*UnloadLevel)(struct Game* game, Level* level), void(*InitEntities)(struct Game* game, struct Level* level));

#endif // !LEVEL_H

