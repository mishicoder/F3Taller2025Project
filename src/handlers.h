#ifndef HANDLERS_H
#define HANDLERS_H

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <flecs.h>
#include "components.h"

void ComponentInfoHook(void* ptr, int32_t count, const ecs_type_info_t *info);
void ComponentSpriteRenderHook(void* ptr, int32_t count, const ecs_type_info_t *info);
void ComponentAnimationHook(void* ptr, int32_t count, const ecs_type_info_t *info);
void ComponentMapRenderHook(void* ptr, int32_t count, const ecs_type_info_t *info);
void ComponentLuaScriptHook(void* ptr, int32_t count, const ecs_type_info_t *info);
void ComponentPythonScriptHook(void* ptr, int32_t count, const ecs_type_info_t *info);

#endif // !HANDLERS_H
