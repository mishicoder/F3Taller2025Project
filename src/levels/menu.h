#pragma once
#ifndef MENU_H
#define MENU_H

#include <stdio.h>
#include <flecs.h>
#include "../core/game.h"

void Menu_Run(Game* game, GameLevel* level, ecs_entity_t entity);

#endif // !MENU_H
