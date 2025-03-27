#pragma once
#ifndef LEVEL_H
#define LEVEL_H

#include <flecs.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct Game;

typedef struct Level
{
  char* name;
  bool keepInMemory;
  bool renderInStack;
  bool updateInStack;
  ecs_world_t* world;

  void(*LoadLevel)(struct Game* game, struct Level* level);
  void(*InitEntities)(struct Game* game, struct Level* level);
  void(*UnloadLevel)(struct Game* game, struct Level* level);
  void(*UpdateLevel)(struct Game* game, struct Level* level);
  void(*RenderLevel)(struct Game* game, struct Level* level);
} Level;

Level* CreateLevel(const char* name, bool keepInMemory, bool renderInStack, bool updateInStack, void(*LoadLevel)(struct Game* game, struct Level* level), void(*UnloadLevel)(struct Game* game, Level* level), void(*InitEntities)(struct Game* game, struct Level* level));

#endif // !LEVEL_H

