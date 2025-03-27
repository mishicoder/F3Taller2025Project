#include "handlers.h"

void ComponentInfoHook(void *ptr, int32_t count, const ecs_type_info_t *info)
{
  Info* component = ptr;
  if(component->tag) {
    printf("Entidad con tag: %s liberada.\n", component->tag);
    free(component->tag);
  }
  if(component->name) free(component->name);
}

void ComponentSpriteRenderHook(void *ptr, int32_t count, const ecs_type_info_t *info)
{
  SpriteRender* component = ptr;
  if(component->sprite) {
    printf("Sprite: %s liberado.\n", component->sprite);
    free(component->sprite);
  }
}

void ComponentAnimationHook(void *ptr, int32_t count, const ecs_type_info_t *info)
{
  AnimationController* component = ptr;
  if (component->sprite) free(component->sprite);
  if (component->currentAnimation) free(component->currentAnimation);
}

void ComponentMapRenderHook(void *ptr, int32_t count, const ecs_type_info_t *info)
{
  MapRender* component = ptr;
  if(component->map) free(component->map);
}

void ComponentLuaScriptHook(void *ptr, int32_t count, const ecs_type_info_t *info)
{
  LuaScript* component = ptr;
  if(component->module) free(component->module);
  // free Lua pointer
  if(component->L){
    printf("Contexto de Lua <L> liberado.\n");
    lua_close(component->L);
    component->L = NULL;
  }
}

void ComponentPythonScriptHook(void *ptr, int32_t count, const ecs_type_info_t *info)
{
  PythonScript* component = ptr;
  if(component->module) free(component->module);
}
