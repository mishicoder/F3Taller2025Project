#pragma once
#ifndef LUASCRIPTING_H
#define LUASCRIPTING_H
#include <lua.h>
#include <lualib.h>
#include <lauxlib.h>
#include <luaconf.h>
#include <flecs.h>
#include <raylib.h>
#include "components.h"

struct Level;

void InitLuaScript(LuaScript* script, ecs_entity_t entity, const char* module, ecs_world_t* world, struct Level* level, int(*LuaGetComponent)(lua_State* L), int (*LuaGetEntityByName)(lua_State* L), int (*LuaGetEntityByTag)(lua_State* L), int(*LuaPlayAnimation)(lua_State* L), int(*LuaDestroyEntity)(lua_State* L), int(*LuaAddComponent)(lua_State* L), int(*LuaRemoveComponent)(lua_State* L), int(*LuaCreate2DEntity)(lua_State* L), int (*LuaQuitGame)(lua_State* L));

void RegisterLuaFunctions(lua_State* L, ecs_world_t* world, struct Level* level, int(*LuaGetComponent)(lua_State* L), int(*LuaGetEntityByName)(lua_State* L), int (*LuaGetEntityByTag)(lua_State* L), int(*LuaPlayAnimation)(lua_State* L), int(*LuaDestroyEntity)(lua_State* L), int(*LuaAddComponent)(lua_State* L), int(*LuaRemoveComponent)(lua_State* L), int(*LuaCreate2DEntity)(lua_State* L), int (*LuaQuitGame)(lua_State* L));


void AssignEntityMethods(lua_State* L, lua_Integer entity, int(*LuaGetComponent)(lua_State * L));

// callbacks
void RegisterInputConstants(lua_State* L);
int LuaKeyDown(lua_State* L);
int LuaKeyDownOnce(lua_State* L);
int LuaKeyUp(lua_State* L);
int LuaKeyUpOnce(lua_State* L);
int LuaGetMousePosition(lua_State* L);
int LuaGetMouseDelta(lua_State* L);
void LuaOnEndAnimation(LuaScript* script, ecs_entity_t entity, void* level, const char* animation);
void LuaUpdate(LuaScript* script, ecs_entity_t entity, void* level);
void LuaOnCollision(LuaScript* script, ecs_entity_t entity, ecs_entity_t other, void* level);
void LuaOnDestroy(LuaScript* script, ecs_entity_t entity);

#endif // !LUASCRIPTING_H
