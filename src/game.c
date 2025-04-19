#include "game.h"

/* Global game instance */
Game game;

ECS_COMPONENT_DECLARE(Info);
ECS_COMPONENT_DECLARE(Transform2D);
ECS_COMPONENT_DECLARE(Camera2d);
ECS_COMPONENT_DECLARE(SpriteRender);
ECS_COMPONENT_DECLARE(AnimationController);
ECS_COMPONENT_DECLARE(RenderLayer);
ECS_COMPONENT_DECLARE(Dyeing);
ECS_COMPONENT_DECLARE(MapRender);
ECS_COMPONENT_DECLARE(LuaScript);
ECS_COMPONENT_DECLARE(PythonScript);
ECS_COMPONENT_DECLARE(RectCollider);
ECS_COMPONENT_DECLARE(CircleCollider);

void DefineComponents(Level *level)
{
  ECS_COMPONENT_DEFINE(level->world, Info);
  ECS_COMPONENT_DEFINE(level->world, Transform2D);
  ECS_COMPONENT_DEFINE(level->world, Camera2d);
  ECS_COMPONENT_DEFINE(level->world, SpriteRender);
  ECS_COMPONENT_DEFINE(level->world, AnimationController);
  ECS_COMPONENT_DEFINE(level->world, RenderLayer);
  ECS_COMPONENT_DEFINE(level->world, Dyeing);
  ECS_COMPONENT_DEFINE(level->world, MapRender);
  ECS_COMPONENT_DEFINE(level->world, LuaScript);
  ECS_COMPONENT_DEFINE(level->world, PythonScript);
  ECS_COMPONENT_DEFINE(level->world, RectCollider);
  ECS_COMPONENT_DEFINE(level->world, CircleCollider);
}

void PushCstringToLua(lua_State* L, const char* str)
{
    if (str) {
        lua_pushstring(L, str);
    }
    else {
        lua_pushnil(L);
    }
}

int LuaPlayAnimation(lua_State* L)
{
    lua_getfield(L, 1, "world");
    ecs_world_t* world = (ecs_world_t*)lua_touserdata(L, -1);
    lua_pop(L, 1);

    if (!world)
    {
        printf("Error al recuperar el mundo en <PlayAnimation>\n");
        lua_pushnil(L);
        return 1;
    }

    lua_Integer entity;

    lua_getfield(L, 1, "id");
    if (!lua_isinteger(L, -1))
    {
        printf("Error al recuperar el id de la entidad en <PlayAnimation>\n");
        lua_pushnil(L);
        return 1;
    }
    entity = lua_tointeger(L, -1);
    lua_pop(L, 1);

    if (!ecs_has(world, entity, AnimationController))
    {
        printf("La entidad no posee el componente <AnimationController>\n");
        lua_pushnil(L);
        return 1;
    }
    SpriteRender* srender = ecs_get(world, entity, SpriteRender);
    AnimationController* controller = ecs_get(world, entity, AnimationController);
    // uno de los componentes no esta presente en la entidad
    if (!controller || !srender)
    {
        printf("Error al obtener los componentes de la entidad\n");
        lua_pushnil(L);
        return 1;
    }
    ResSprite* sprite = GetSprite(&game.resManager, srender->sprite);
    if(!sprite)
    {
        lua_pushnil(L);
        return 1;
    }

    const char* animID = luaL_checkstring(L, 2);
    
    ResAnimation* animation = GetSpriteAnimation(sprite, animID);
    if (!animation)
    {
        lua_pushnil(L);
        return 1;
    }

    if (controller->currentAnimation == NULL)
    {
        controller->currentAnimation = _strdup(animID);
        controller->currentFrame = animation->fIndex;
        controller->fIndex = animation->fIndex;
        controller->tIndex = animation->tIndex;
        controller->loop = animation->loop;
        controller->speed = animation->speed;
        controller->frameCounter = 0;
    }
    else
    {
        if (strcmp(controller->currentAnimation, animID) == 0) { lua_pushnil(L); return 1; }
        free(controller->currentAnimation);
        controller->currentAnimation = _strdup(animID);
        controller->currentFrame = animation->fIndex;
        controller->fIndex = animation->fIndex;
        controller->tIndex = animation->tIndex;
        controller->loop = animation->loop;
        controller->speed = animation->speed;
        controller->frameCounter = 0;
    }
        
    return 1;
}

int LuaGetComponentField(lua_State *L)
{
  luaL_checktype(L, 1, LUA_TTABLE);
  const char* key = luaL_checkstring(L, 2);

  lua_getfield(L, 1, "__ptr");
  void* component = lua_touserdata(L, -1);
  lua_pop(L, 1);
  
  lua_getfield(L, 1, "__component");
  const char* componentType = lua_tostring(L, -1);
  lua_pop(L, 1);

  lua_getfield(L, 1, "__entity");
  ecs_entity_t entity = lua_tointeger(L, -1);
  lua_pop(L, 1);

  lua_getfield(L, 1, "__world");
  ecs_world_t* world = (ecs_world_t*)lua_touserdata(L, -1);
  lua_pop(L, 1);

  if (!component || !world) {
      lua_pushnil(L);
      return 1;
  }
  
  if(strcmp(componentType, "Transform2D") == 0) {
      Transform2D* transform = (Transform2D*)component;
      if (strcmp(key, "x") == 0) { lua_pushnumber(L, transform->posx); }
      else if (strcmp(key, "y") == 0) { lua_pushnumber(L, transform->posy); }
      else if (strcmp(key, "sx") == 0) { lua_pushnumber(L, transform->scalex); }
      else if (strcmp(key, "sy") == 0){ lua_pushnumber(L, transform->scaley); }
      else if (strcmp(key, "rot") == 0) { lua_pushnumber(L, transform->rotation); }
      else { lua_pushnil(L); }
  }
  else if (strcmp(componentType, "Info") == 0) {
      Info* info = (Info*)component;
      if (strcmp(key, "name") == 0) { 
          PushCstringToLua(L, info->name);
      }
      else if (strcmp(key, "tag") == 0) {
          PushCstringToLua(L, info->tag);
      }
      else { lua_pushnil(L); }
  }
  else if (strcmp(componentType, "SpriteRender") == 0) {
      SpriteRender* srender = (SpriteRender*)component;
      if(strcmp(key, "visible") == 0){
          lua_pushboolean(L, srender->visible);
      }
      else if (strcmp(key, "opacity") == 0) {
          lua_pushnumber(L, srender->opacity);
      }
      else if (strcmp(key, "flipx") == 0) {
          lua_pushboolean(L, srender->flipx);
      }
      else if (strcmp(key, "flipy") == 0) {
          lua_pushboolean(L, srender->flipy);
      }
      else {
          lua_pushnil(L);
      }
  }
  else if (strcmp(componentType, "Camera2D") == 0) {
      Camera2d* camera = (Camera2d*)component;
      if(strcmp(key, "main") == 0){
          lua_pushboolean(L, camera->isMain);
      }
      else if (strcmp(key, "offsetx") == 0){
          lua_pushnumber(L, camera->offsetx);
      }
      else if (strcmp(key, "offsety") == 0) {
          lua_pushnumber(L, camera->offsety);
      }
      else if (strcmp(key, "zoom") == 0) {
          lua_pushnumber(L, camera->zoom);
      }
      else {
          lua_pushnil(L);
      }
  }
  else if (strcmp(componentType, "RectCollidere") == 0) {
      RectCollider* collider = (RectCollider*)component;
      if (strcmp(key, "offsetx") == 0) {
          lua_pushnumber(L, collider->offsetx);
      }
      else if (strcmp(key, "offsety") == 0) {
          lua_pushnumber(L, collider->offsety);
      }
      else if (strcmp(key, "w") == 0) {
          lua_pushnumber(L, collider->width);
      }
      else if (strcmp(key, "h") == 0) {
          lua_pushnumber(L, collider->height);
      }
      else if (strcmp(key, "static") == 0) {
          lua_pushboolean(L, collider->isStatic);
      }
      else if (strcmp(key, "solid") == 0) {
          lua_pushboolean(L, collider->isSolid);
      }
      else {
          lua_pushnil(L);
      }
  }
  else if (strcmp(componentType, "CircleCollider") == 0) {
      CircleCollider* collider = (CircleCollider*)component;
      if (strcmp(key, "offsetx") == 0) {
          lua_pushnumber(L, collider->offsetx);
      }
      else if (strcmp(key, "offsety") == 0) {
          lua_pushnumber(L, collider->offsety);
      }
      else if (strcmp(key, "radius") == 0) {
          lua_pushnumber(L, collider->radius);
      }
      else if (strcmp(key, "static") == 0) {
          lua_pushboolean(L, collider->isStatic);
      }
      else if (strcmp(key, "solid") == 0) {
          lua_pushboolean(L, collider->isSolid);
      }
      else {
          lua_pushnil(L);
      }
  }
  else if (strcmp(componentType, "Color") == 0) {
      Dyeing* dyeing = (Dyeing*)component;
      if (strcmp(key, "r") == 0) { lua_pushnumber(L, dyeing->r); }
      else if (strcmp(key, "g") == 0){ lua_pushnumber(L, dyeing->g); }
      else if (strcmp(key, "b") == 0) { lua_pushnumber(L, dyeing->b); }
      else { lua_pushnil(L); }
  }

  return 1;
}

int LuaGetComponent(lua_State *L)
{
    lua_getfield(L, 1, "world");
    ecs_world_t* world = (ecs_world_t*)lua_touserdata(L, -1);
    lua_pop(L, 1);

    if (!world)
    {
        lua_pushnil(L);
        return 1;
    }

    lua_Integer entity;

    lua_getfield(L, 1, "id");
    if (!lua_isinteger(L, -1)) {
        lua_pushnil(L);
        return 1;
    }
    entity = lua_tointeger(L, -1);
    lua_pop(L, 1);

    const char* componentID = luaL_checkstring(L, 2);
    void* componentPtr = NULL;

    if (strcmp(componentID, "Transform2D") == 0) {
        componentPtr = ecs_get_mut(world, entity, Transform2D);
    }
    else if (strcmp(componentID, "Info") == 0) {
        componentPtr = ecs_get_mut(world, entity, Info);
    }
    else if (strcmp(componentID, "Color") == 0) {
        componentPtr = ecs_get_mut(world, entity, Dyeing);
    }
    else if (strcmp(componentID, "SpriteRender") == 0) {
        componentPtr = ecs_get_mut(world, entity, SpriteRender);
    }
    else if (strcmp(componentID, "Camera2D") == 0) {
        componentPtr = ecs_get_mut(world, entity, Camera2d);
    }
    else if (strcmp(componentID, "RectCollider") == 0) {
        componentPtr = ecs_get_mut(world, entity, RectCollider);
    }
    else if (strcmp(componentID, "CircleCollider") == 0) {
        componentPtr = ecs_get_mut(world, entity, CircleCollider);
    }
    else {
        lua_pushnil(L);
        return 1;
    }

    if (!componentPtr) {
        lua_pushnil(L);
        return 1;
    }

    lua_newtable(L);
    lua_pushlightuserdata(L, componentPtr);
    lua_setfield(L, -2, "__ptr");
    lua_pushlightuserdata(L, world);
    lua_setfield(L, -2, "__world");
    lua_pushinteger(L, entity);
    lua_setfield(L, -2, "__entity");
    lua_pushstring(L, componentID);
    lua_setfield(L, -2, "__component");

    lua_newtable(L);
    lua_pushcfunction(L, LuaSetComponentField);
    lua_setfield(L, -2, "__newindex");
    lua_pushcfunction(L, LuaGetComponentField);
    lua_setfield(L, -2, "__index");
    lua_setmetatable(L, -2);

    return 1;
}

/*
* Transform2D e Info, se ignoran, ya que son componentes que posee
* la entidad por defecto, por lo que simplemente se tendría que llamar a
* self:GetComponent("Transform2D") y
* self:GetComponent("Info") desde lua y operar con sus propiedades
*/
int LuaAddComponent(lua_State* L)
{
    lua_getfield(L, 1, "world");
    ecs_world_t* world = (ecs_world_t*)lua_touserdata(L, -1);
    lua_pop(L, 1);

    if (!world) {
        lua_pushnil(L);
        return 1;
    }

    lua_Integer entity;

    lua_getfield(L, 1, "id");
    if (!lua_isinteger(L, -1))
    {
        printf("Error al recuperar el id de la entidad en <PlayAnimation>\n");
        lua_pushnil(L);
        return 1;
    }
    entity = lua_tointeger(L, -1);
    lua_pop(L, 1);

    
    const char* component = luaL_checkstring(L, 2);
    if(strcmp(component, "SpriteRender") == 0){
        if (ecs_has(world, entity, SpriteRender)) {
            lua_pushnil(L);
            return 1;
        }
        
        if (!lua_istable(L, 3)) {
            lua_pushnil(L);
            return 1;
        }
        lua_getfield(L, 3, "sprite");
        if (!lua_isstring(L, -1)) {
            lua_pushnil(L);
            return 1;
        }
        const char* spriteName = lua_tostring(L, -1);
        lua_pop(L, 1);

        ResSprite* spr = GetSprite(&game.resManager, spriteName);
        if (!spr) {
            printf("El sprite <%s> no existe en el sistema de recursos.\n", spriteName);
            lua_pushnil(L);
            return 1;
        }

        float opacity = 1.0f;
        bool visible = true;
        bool flipx = false;
        bool flipy = false;

        lua_getfield(L, 3, "opacity");
        if (lua_isnumber(L, -1)) {
            opacity = (float)lua_tonumber(L, -1);
        }
        lua_pop(L, 1);

        lua_getfield(L, 3, "visible");
        if (lua_isboolean(L, -1)) {
            visible = lua_toboolean(L, -1);
        }
        lua_pop(L, 1);

        lua_getfield(L, 3, "flipx");
        if (lua_isboolean(L, -1)) {
            flipx = lua_toboolean(L, -1);
        }
        lua_pop(L, 1);

        lua_getfield(L, 3, "flipy");
        if (lua_isboolean(L, -1)) {
            flipy = lua_toboolean(L, -1);
        }
        lua_pop(L, 1);

        ecs_set(world, entity, SpriteRender, { _strdup(spriteName), visible, opacity, flipx, flipy });

        lua_pushboolean(L, 1);
        return 1;
    }
    else if (strcmp(component, "Animation") == 0){
        if (ecs_has(world, entity, AnimationController)) {
            lua_pushnil(L);
            return 1;
        }

        if (!lua_istable(L, 3)) {
            lua_pushnil(L);
            return 1;
        }
        lua_getfield(L, 3, "sprite");
        if (!lua_isstring(L, -1)) {
            lua_pushnil(L);
            return 1;
        }
        const char* spriteName = lua_tostring(L, -1);
        lua_pop(L, 1);

        ResSprite* spr = GetSprite(&game.resManager, spriteName);
        if (!spr) {
            printf("El sprite <%s> no existe en el sistema de recursos.\n", spriteName);
            lua_pushnil(L);
            return 1;
        }

        lua_getfield(L, 3, "animation");
        const char* animName = lua_tostring(L, -1);
        lua_pop(L, 1);

        ecs_set(world, entity, AnimationController, { 
            _strdup(spriteName),
            animName ? _strdup(animName) : NULL,
            0,
            0,
            0,
            0,
            false,
            0
        });
        lua_pushboolean(L, 1);
        return 1;
    }
    else if (strcmp(component, "Camera2D") == 0) {
        if (ecs_has(world, entity, Camera2d)) {
            lua_pushnil(L);
            return 1;
        }

        if (!lua_istable(L, 3)) {
            lua_pushnil(L);
            return 1;
        }

        float offsetx = 0.0f;
        float offsety = 0.0f;
        float zoom = 1.0f;

        lua_getfield(L, 3, "offsetx");
        if (lua_isnumber(L, -1)) {
            offsetx = (float)lua_tonumber(L, -1);
        }
        lua_pop(L, 1);

        lua_getfield(L, 3, "offsety");
        if (lua_isnumber(L, -1)) {
            offsety = (float)lua_tonumber(L, -1);
        }
        lua_pop(L, 1);

        lua_getfield(L, 3, "zoom");
        if (lua_isnumber(L, -1)) {
            zoom = (float)lua_tonumber(L, -1);
        }
        lua_pop(L, 1);

        ecs_set(world, entity, Camera2d, { false, offsetx, offsety, zoom });
        lua_pushboolean(L, 1);
        return 1;
    }
    else if (strcmp(component, "Color") == 0){
        if (ecs_has(world, entity, Dyeing)) {
            lua_pushnil(L);
            return 1;
        }

        if (!lua_istable(L, 3)) {
            lua_pushnil(L);
            return 1;
        }

        float r = 1.0f;
        float g = 1.0f;
        float b = 1.0f;

        lua_getfield(L, 3, "red");
        if (lua_isnumber(L, -1)) {
            r = (float)lua_tonumber(L, -1);
        }
        lua_pop(L, 1);

        lua_getfield(L, 3, "green");
        if (lua_isnumber(L, -1)) {
            g = (float)lua_tonumber(L, -1);
        }
        lua_pop(L, 1);

        lua_getfield(L, 3, "blue");
        if (lua_isnumber(L, -1)) {
            b = (float)lua_tonumber(L, -1);
        }
        lua_pop(L, 1);

        ecs_set(world, entity, Dyeing, { r, g, b });
        lua_pushboolean(L, 1);
        return 1;
    }
    else if (strcmp(component, "MapRender") == 0){
        if (ecs_has(world, entity, MapRender)) {
            lua_pushnil(L);
            return 1;
        }

        if (!lua_istable(L, 3)) {
            lua_pushnil(L);
            return 1;
        }
        lua_getfield(L, 3, "map");
        if (!lua_isstring(L, -1)) {
            lua_pushnil(L);
            return 1;
        }
        const char* mapName = lua_tostring(L, -1);
        lua_pop(L, 1);

        ecs_set(world, entity, MapRender, { _strdup(mapName) });
        lua_pushboolean(L, 1);
        return 1;
    }
    else if (strcmp(component, "LuaScript") == 0){
        if (ecs_has(world, entity, LuaScript)) {
            lua_pushnil(L);
            return 1;
        }

        if (!lua_istable(L, 3)) {
            lua_pushnil(L);
            return 1;
        }

        lua_getfield(L, 3, "module");
        if (!lua_isstring(L, -1)) {
            lua_pushnil(L);
            return 1;
        }
        const char* moduleName = lua_tostring(L, -1);
        lua_pop(L, 1);

        ecs_set(world, entity, LuaScript, { _strdup(moduleName) });
        LuaScript* script = ecs_get(world, entity, LuaScript);
        script->isValid = true;
        InitLuaScript(script, entity, script->module, world, NULL, LuaGetComponent, LuaGetEntityByName, LuaGetEntityByTag, LuaPlayAnimation, LuaDestroyEntity, LuaAddComponent, LuaRemoveComponent, LuaCreate2DEntity, LuaQuitGame);

        if(script->isValid)
        {
            lua_getglobal(script->L, "OnCreate");
            if (lua_isfunction(script->L, -1)) {
                if (lua_pcall(script->L, 0, 0, 0) != LUA_OK) {
                    printf("Error en el script <%s>: %s\n", script->module, lua_tostring(script->L, -1));
                }
            }
            else {
                lua_pop(script->L, 1);
            }
        }

        lua_pushboolean(L, 1);
        return 1;
    }
    else if (strcmp(component, "PythonScript") == 0){
        // in dev...
    }
    else if (strcmp(component, "RectCollider") == 0) {
        if (ecs_has(world, entity, RectCollider)) {
            lua_pushnil(L);
            return 1;
        }

        if (!lua_istable(L, 3)) {
            lua_pushnil(L);
            return 1;
        }

        float offsetx = 0;
        float offsety = 0;
        int width = 50;
        int height = 50;
        bool isSolid = true;
        bool isStatic = true;

        lua_getfield(L, 3, "offsetx");
        if (lua_isnumber(L, -1)) {
            offsetx = (float)lua_tonumber(L, -1);
        }
        lua_pop(L, 1);

        lua_getfield(L, 3, "offsety");
        if (lua_isnumber(L, -1)) {
            offsety = (float)lua_tonumber(L, -1);
        }
        lua_pop(L, 1);

        lua_getfield(L, 3, "width");
        if (lua_isnumber(L, -1)) {
            width = (int)lua_tonumber(L, -1);
        }
        lua_pop(L, 1);

        lua_getfield(L, 3, "height");
        if (lua_isnumber(L, -1)) {
            height = (int)lua_tonumber(L, -1);
        }
        lua_pop(L, 1);

        lua_getfield(L, 3, "solid");
        if (lua_isboolean(L, -1)) {
            isSolid = lua_toboolean(L, -1);
        }
        lua_pop(L, 1);

        lua_getfield(L, 3, "static");
        if (lua_isboolean(L, -1)) {
            isStatic = lua_toboolean(L, -1);
        }
        lua_pop(L, 1);

        ecs_set(world, entity, RectCollider, { 0.0f, 0.0f, offsetx, offsety, width, height, isSolid, isStatic });
        lua_pushboolean(L, 1);
        return 1;
    }
    else if (strcmp(component, "CircleCollider") == 0){
        if (ecs_has(world, entity, CircleCollider)) {
            lua_pushnil(L);
            return 1;
        }

        if (!lua_istable(L, 3)) {
            lua_pushnil(L);
            return 1;
        }

        float offsetx = 0.0f;
        float offsety = 0.0f;
        float radius = 10.0f;
        bool isSolid = true;
        bool isStatic = true;

        lua_getfield(L, 3, "offsetx");
        if (lua_isnumber(L, -1)) {
            offsetx = (float)lua_tonumber(L, -1);
        }
        lua_pop(L, 1);

        lua_getfield(L, 3, "offsety");
        if (lua_isnumber(L, -1)) {
            offsety = (float)lua_tonumber(L, -1);
        }
        lua_pop(L, 1);

        lua_getfield(L, 3, "radius");
        if (lua_isnumber(L, -1)) {
            radius = (float)lua_tonumber(L, -1);
        }
        lua_pop(L, 1);

        lua_getfield(L, 3, "solid");
        if (lua_isboolean(L, -1)) {
            isSolid = lua_toboolean(L, -1);
        }
        lua_pop(L, 1);

        lua_getfield(L, 3, "static");
        if (lua_isboolean(L, -1)) {
            isStatic = lua_toboolean(L, -1);
        }
        lua_pop(L, 1);

        ecs_set(world, entity, CircleCollider, { 0.0f, 0.0f, offsetx, offsety, radius, isSolid, isStatic });
        lua_pushboolean(L, 1);
        return 1;
    }
    else {
        lua_pushnil(L);
        return 1;
    }

    return 1;
}

int LuaDestroyEntity(lua_State* L)
{
    lua_getfield(L, 1, "world");
    ecs_world_t* world = (ecs_world_t*)lua_touserdata(L, -1);
    lua_pop(L, 1);

    lua_getfield(L, 1, "level");
    Level* level = (Level*)lua_touserdata(L, -1);
    lua_pop(L, 1);

    if (!level) {
        printf("Imposible recuperar el nivel de la entidad´.\n");
        lua_pushboolean(L, 0);
        return 1;
    }
    /*else {
        printf("Level pointer direction: %p\n", level);
        printf("Level name: %s\n", level->name);
    }*/

    if (!world) {
        lua_pushnil(L);
        return 1;
    }

    ecs_entity_t entity = (ecs_entity_t)luaL_checkinteger(L, 2);
    if(ecs_is_alive(world, entity))
    {
        AddEntityToDestroyQueue(level, entity);
        lua_pushboolean(L, 1);
        return 1;
    }

    lua_pushboolean(L, 0);
    return 1;
}

int LuaRemoveComponent(lua_State* L)
{
    lua_getfield(L, 1, "world");
    ecs_world_t* world = (ecs_world_t*)lua_touserdata(L, -1);
    lua_pop(L, 1);

    if (!world) {
        lua_pushnil(L);
        return 1;
    }

    lua_Integer entity;

    lua_getfield(L, 1, "id");
    if (!lua_isinteger(L, -1))
    {
        printf("Error al recuperar el id de la entidad en <RemoveComponent>\n");
        lua_pushnil(L);
        return 1;
    }
    entity = lua_tointeger(L, -1);
    lua_pop(L, 1);

    const char* component = luaL_checkstring(L, 2);
    if (strcmp(component, "SpriteRender") == 0){
        if (ecs_has(world, entity, SpriteRender)) {
            ecs_remove(world, entity, SpriteRender);
            lua_pushboolean(L, 1);
            return 1;
        }

        lua_pushnil(L);
        return 1;
    }
    else if (strcmp(component, "Animation") == 0){
        if (ecs_has(world, entity, AnimationController)) {
            ecs_remove(world, entity, AnimationController);
            lua_pushboolean(L, 1);
            return 1;
        }

        lua_pushnil(L);
        return 1;
    }
    else if (strcmp(component, "Camera2D") == 0){
        if (ecs_has(world, entity, Camera2d)) {
            ecs_remove(world, entity, Camera2d);
            lua_pushboolean(L, 1);
            return 1;
        }

        lua_pushnil(L);
        return 1;
    }
    else if (strcmp(component, "Color") == 0){
        if (ecs_has(world, entity, Dyeing)) {
            ecs_remove(world, entity, Dyeing);
            lua_pushboolean(L, 1);
            return 1;
        }

        lua_pushnil(L);
        return 1;
    }
    else if (strcmp(component, "MapRender") == 0){
        if (ecs_has(world, entity, MapRender)) {
            ecs_remove(world, entity, MapRender);
            lua_pushboolean(L, 1);
            return 1;
        }
        
        lua_pushnil(L);
        return 1;
    }
    else if (strcmp(component, "LuaScript") == 0){
        if (ecs_has(world, entity, LuaScript)) {
            ecs_remove(world, entity, LuaScript);
            lua_pushboolean(L, 1);
            return 1;
        }

        lua_pushnil(L);
        return 1;
    }
    else if (strcmp(component, "PythonScript") == 0){
        // in dev...
    }
    else if (strcmp(component, "RectCollider") == 0){
        if (ecs_has(world, entity, RectCollider)) {
            ecs_remove(world, entity, RectCollider);
            lua_pushboolean(L, 1);
            return 1;
        }

        lua_pushnil(L);
        return 1;
    }
    else if (strcmp(component, "CircleCollider") == 0){
        if (ecs_has(world, entity, CircleCollider)) {
            ecs_remove(world, entity, CircleCollider);
            lua_pushboolean(L, 1);
            return 1;
        }

        lua_pushnil(L);
        return 1;
    }
    else {
        lua_pushnil(L);
        return 1;
    }

    return 1;
}

int LuaCreate2DEntity(lua_State* L)
{
    ecs_world_t* world = (ecs_world_t*)lua_touserdata(L, lua_upvalueindex(1));
    const char* name = luaL_checkstring(L, 1);
    const char* tag = luaL_checkstring(L, 2);

    ecs_entity_t entity = 0;

    if (name) {
        struct ecs_entity_desc_t desc = { 0 };
        desc.name = name;

        entity = ecs_entity_init(world, &desc);
    }
    else {
        entity = ecs_new(world);
    }

    ecs_set(world, entity, Info, { name ? _strdup(name) : NULL, tag ? _strdup(tag) : NULL });
    ecs_set(world, entity, Transform2D, { 0.0f, 0.0f, 1.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f });

    lua_pushnumber(L, entity);
    return 1;
}

int LuaSetComponentField(lua_State *L)
{
  luaL_checktype(L, 1, LUA_TTABLE);
  const char* key = luaL_checkstring(L, 2);

  //lua_getmetatable(L, 1);
  lua_getfield(L, 1, "__ptr");
  void* component = lua_touserdata(L, -1);
  lua_pop(L, 1);

  if (!component) {
      lua_pushstring(L, "Componente no encontrado");
      lua_error(L);
  }

  lua_getfield(L, 1, "__component");
  const char* componentType = lua_tostring(L, -1);
  lua_pop(L, 1);

  lua_getfield(L, 1, "__world");
  ecs_world_t* world = (ecs_world_t*)lua_touserdata(L, -1);
  lua_pop(L, 1);

  lua_getfield(L, 1, "__entity");
  ecs_entity_t entity = (ecs_entity_t)lua_tointeger(L, -1);
  lua_pop(L, 1);

  if (strcmp(componentType, "Transform2D") == 0) {
      Transform2D* transform = (Transform2D*)component;
      if (!transform) return 0;

      if (strcmp(key, "x") == 0) { transform->posx = (float)luaL_checknumber(L, 3); }
      else if (strcmp(key, "y") == 0) { transform->posy = (float)luaL_checknumber(L, 3); }
      else if (strcmp(key, "sx") == 0) { transform->scalex = (float)luaL_checknumber(L, 3); }
      else if (strcmp(key, "sy") == 0) { transform->scaley = (float)luaL_checknumber(L, 3); }
      else if (strcmp(key, "rot") == 0) { transform->rotation = (float)luaL_checknumber(L, 3); }
      else if (strcmp(key, "sxy") == 0) { transform->scalex = (float)luaL_checknumber(L, 3); transform->scaley = (float)luaL_checknumber(L, 3); }
      ecs_modified(world, entity, Transform2D);
  }
  else if (strcmp(componentType, "Info") == 0) {
      Info* info = (Info*)component;
      if (!info) return 0;
      
      if (strcmp(key, "name") == 0) {
          const char* str = luaL_checkstring(L, 3);
          if (info->name) {
              free(info->name);
              info->name = NULL;
          }
          info->name = _strdup(str);
      }
      else if (strcmp(key, "tag") == 0) {
          const char* str = luaL_checkstring(L, 3);
          if (info->tag) {
              free(info->tag);
              info->tag = NULL;
          }
          info->tag = _strdup(str);
      }
      ecs_modified(world, entity, Info);
  }
  else if (strcmp(componentType, "SpriteRender") == 0) {
      SpriteRender* srender = (SpriteRender*)component;
      if (!srender) return 0;

      if (strcmp(key, "visible") == 0) {
          srender->visible = lua_toboolean(L, 3);
      }
      else if (strcmp(key, "opacity") == 0) {
          srender->opacity = (float)lua_tonumber(L, 3);
      }
      else if (strcmp(key, "flipx") == 0) {
          srender->flipx = lua_toboolean(L, 3);
      }
      else if (strcmp(key, "flipy") == 0) {
          srender->flipy = lua_toboolean(L, 3);
      }
      else if (strcmp(key, "flip") == 0) {
          srender->flipx = lua_toboolean(L, 3);
          srender->flipy = lua_toboolean(L, 3);
      }
  }
  else if (strcmp(componentType, "Camera2D") == 0) {
      Camera2d* camera = (Camera2d*)component;
      if (!camera) return 0;

      if (strcmp(key, "main") == 0) {
          camera->isMain = lua_toboolean(L, 3);
      }
      else if (strcmp(key, "offsetx") == 0) {
          camera->offsetx = lua_tonumber(L, 3);
      }
      else if (strcmp(key, "offsety") == 0) {
          camera->offsety = lua_tonumber(L, 3);
      }
      else if (strcmp(key, "zoom") == 0) {
          camera->zoom = lua_tonumber(L, 3);
      }
  }
  else if (strcmp(componentType, "RectCollider") == 0) {
      RectCollider* collider = (RectCollider*)component;
      if (!collider) return 0;

      if (strcmp(key, "offsetx") == 0) {
          collider->offsetx = lua_tonumber(L, 3);
      }
      else if (strcmp(key, "offsety") == 0) {
          collider->offsety = lua_tonumber(L, 3);
      }
      else if (strcmp(key, "w") == 0) {
          collider->width = (int)lua_tonumber(L, 3);
      }
      else if (strcmp(key, "h") == 0) {
          collider->height = (int)lua_tonumber(L, 3);
      }
      else if (strcmp(key, "static") == 0) {
          collider->isStatic = lua_toboolean(L, 3);
      }
      else if (strcmp(key, "solid") == 0) {
          collider->isSolid = lua_toboolean(L, 3);
      }
  }
  else if (strcmp(componentType, "CircleCollider") == 0) {
      CircleCollider* collider = (CircleCollider*)component;
      if (!collider) return 0;

      if (strcmp(key, "offsetx") == 0) {
          collider->offsetx = lua_tonumber(L, 3);
      }
      else if (strcmp(key, "offsety") == 0) {
          collider->offsety = lua_tonumber(L, 3);
      }
      else if (strcmp(key, "radius") == 0) {
          collider->radius = lua_tonumber(L, 3);
      }
      else if (strcmp(key, "static") == 0) {
          collider->isStatic = lua_toboolean(L, 3);
      }
      else if (strcmp(key, "solid") == 0) {
          collider->isSolid = lua_toboolean(L, 3);
      }
  }
  else if (strcmp(componentType, "Color") == 0) {
      Dyeing* dyeing = (Dyeing*)component;
      if (!dyeing) return 0;

      if (strcmp(key, "r") == 0) {
          dyeing->r = luaL_checknumber(L, 3); 
          if (dyeing->r > 1.0f) dyeing->r = 1.0f;
          else if (dyeing->r < 0.0f) dyeing->r = 0.0f;
      }
      else if (strcmp(key, "g") == 0) { 
          dyeing->g = luaL_checknumber(L, 3); 
          if (dyeing->g > 1.0f) dyeing->g = 1.0f;
          else if (dyeing->g < 0.0f) dyeing->g = 0.0f;
      }
      else if (strcmp(key, "b") == 0) { 
          dyeing->b = luaL_checknumber(L, 3); 
          if (dyeing->b > 1.0f) dyeing->b = 1.0f;
          else if (dyeing->b < 0.0f) dyeing->b = 0.0f;
      }
      ecs_modified(world, entity, Dyeing);
  }
 
  return 0;
}

int LuaGetEntityByName(lua_State* L)
{
    ecs_world_t* world = (ecs_world_t*)lua_touserdata(L, lua_upvalueindex(1));
    const char* name = luaL_checkstring(L, 1);

    ecs_query_t* query = ecs_query_init(world, &(ecs_query_desc_t){
        .terms = {
            {.id = ecs_id(Info)}
        }
    });
    ecs_iter_t it = ecs_query_iter(world, query);
    while (ecs_query_next(&it)) 
    {
        for (int i = 0; i < it.count; i++)
        {
            Info* info = ecs_get(world, it.entities[i], Info);
            if (info->name && strcmp(info->name, name) == 0)
            {
                lua_newtable(L);
                lua_pushinteger(L, it.entities[i]);
                lua_setfield(L, -2, "id");
                ecs_iter_fini(&it);
                return 1;
            }
        }
    }

    ecs_query_fini(query);
    lua_pushnil(L);
    return 1;
}

int LuaGetEntityByTag(lua_State* L)
{
    ecs_world_t* world = (ecs_world_t*)lua_touserdata(L, lua_upvalueindex(1));
    const char* tag = luaL_checkstring(L, 1);

    ecs_query_t* query = ecs_query_init(world, &(ecs_query_desc_t){
        .terms = {
            {.id = ecs_id(Info)}
        }
    });
    ecs_iter_t it = ecs_query_iter(world, query);
    while (ecs_query_next(&it))
    {
        for (int i = 0; i < it.count; i++)
        {
            Info* info = ecs_get(world, it.entities[i], Info);
            if (info->tag && strcmp(info->tag, tag) == 0)
            {
                lua_newtable(L);
                lua_pushinteger(L, it.entities[i]);
                lua_setfield(L, -2, "id");

                lua_pushlightuserdata(L, world);
                lua_setfield(L, -2, "world");

                luaL_getmetatable(L, "EntityMetaTable");
                if (lua_isnil(L, -1)) {
                    lua_pop(L, 1);

                    luaL_newmetatable(L, "EntityMetaTable");

                    lua_pushcfunction(L, LuaGetComponent);
                    lua_setfield(L, -2, "GetComponent");

                    lua_pushcfunction(L, LuaPlayAnimation);
                    lua_setfield(L, -2, "PlayAnimation");

                    lua_pushcfunction(L, LuaDestroyEntity);
                    lua_setfield(L, -2, "Destroy");

                    lua_pushcfunction(L, LuaAddComponent);
                    lua_setfield(L, -2, "AddComponent");

                    lua_pushcfunction(L, LuaRemoveComponent);
                    lua_setfield(L, -2, "RemoveComponent");

                    lua_pushvalue(L, -1);
                    lua_setfield(L, -2, "__index");
                }

                lua_setmetatable(L, -2);

                ecs_iter_fini(&it);
                return 1;
            }
        }
    }

    lua_pushnil(L);
    return 1;
}

int LuaQuitGame(lua_State* L)
{
    int exitCode = luaL_checknumber(L, 2);
    game.isRunning = false;
    game.exitCode = exitCode;
    return 1;
}

void AddEntityToDestroyQueue(Level* level, ecs_entity_t entity)
{
    if (level->destroyQueueCount >= MAX_ENTITY_DESTROY_QUEUE)
        return; // No hay espacio para destruir la entidad

    level->destroyQueue[level->destroyQueueCount] = entity;
    level->destroyQueueCount += 1;
}

void InitGame(GameConfig config, void (*LoadResources)(void))
{
  game.config = config;
  game.LoadResources = LoadResources;
  game.cursors = NULL;
  game.cursorsCount = 0;
  game.currentCursor = NULL;
  game.levelStack = NULL;
  game.cacheLevels = NULL;
  InitResourcesManager(&game.resManager); 

  InitWindow(game.config.windowWidth, game.config.windowHeight, game.config.windowTitle);
  SetWindowState(FLAG_WINDOW_RESIZABLE);
  SetWindowMinSize(game.config.windowWidth, game.config.windowHeight);
  if(config.targetFPS == 0) config.targetFPS = 60;
  if(config.globalScale == 0.0f) config.globalScale = 1.0f;

  SetTargetFPS(game.config.targetFPS);

  if(!game.config.useEscapeToExit) SetExitKey(-1);

  if(game.config.windowFullscreen)
  {
    int fsWidth = GetMonitorWidth(GetCurrentMonitor());
    int fsHeight = GetMonitorHeight(GetCurrentMonitor());
    SetWindowSize(fsWidth, fsHeight);
    ToggleFullscreen();
  }

  game.tagQuery = NULL;
  game.isRunning = true;
  game.exitCode = 0; // default exit code

  if(game.LoadResources) game.LoadResources();
}

bool SetGameWindowIcon(const char *imgFilename)
{
  Image icon = LoadImage(imgFilename);
  if(!icon.data) return false;

  SetWindowIcon(icon);
  UnloadImage(icon);

  return true;
}

bool LoadCustomCursor(const char *name, const char *texFilename, float scalex, float scaley)
{
  Texture2D texture = LoadTexture(texFilename);
  if(texture.id == 0) return false;

  CustomCursor* cursor = (CustomCursor*)malloc(sizeof(CustomCursor));
  if(!cursor) { UnloadTexture(texture); return false; }

  CustomCursor** memTemp = (CustomCursor**)realloc(game.cursors, (size_t)(game.cursorsCount + 1) * sizeof(CustomCursor*));
  if(!memTemp){ UnloadTexture(texture); free(cursor); return false; }

  cursor->name = _strdup(name);
  cursor->texture = texture;
  cursor->scalex = scalex;
  cursor->scaley = scaley;
  cursor->frameWidth = 0;
  cursor->frameHeight = 0;
  cursor->currentFrame = -1;
  cursor->frames = 0;
  cursor->isAnimated = false;
  cursor->animSpeed = 0;
  cursor->frameCounter = 0;

  game.cursors = memTemp;
  game.cursors[game.cursorsCount] = cursor;
  game.cursorsCount += 1;

  return true;
}

bool LoadAnimatedCursor(const char *name, const char *texFilename, int frames, int fwidth, int fheight, int speed, float scalex, float scaley)
{
  Texture2D texture = LoadTexture(texFilename);
  if(texture.id == 0) return false;

  CustomCursor* cursor = (CustomCursor*)malloc(sizeof(CustomCursor));
  if(!cursor){ UnloadTexture(texture); return false; }

  CustomCursor** memTemp = (CustomCursor**)realloc(game.cursors, (size_t)(game.cursorsCount + 1) * sizeof(CustomCursor*));
  if(!memTemp){ UnloadTexture(texture); free(cursor); return false; }

  cursor->name = _strdup(name);
  cursor->texture = texture;
  cursor->frames = frames;
  cursor->animSpeed = speed;
  cursor->frameHeight = fwidth;
  cursor->frameHeight = fheight;
  cursor->currentFrame = 0;
  cursor->scalex = scalex;
  cursor->scaley = scaley;
  cursor->isAnimated = true;
  cursor->frameCounter = 0;

  game.cursors = memTemp;
  game.cursors[game.cursorsCount] = cursor;
  game.cursorsCount += 1;

  return true;
}

void RunGame()
{
    // Ejecuta los OnStart de los scripts del nivel por defecto
    Level* currentLevel = GetCurrentLevel();
    if (currentLevel) {
        currentLevel->InitEntities(&game, currentLevel);
    }

    // change to use "isRunning" game variable for application control with lua
    while(!WindowShouldClose() && game.isRunning)
    {
    BeginDrawing();
    ClearBackground(game.config.windowColor);

    LevelNode* node = game.levelStack;
    while(node)
    {
        Level* level = node->level;
        if(node == game.levelStack || level->updateInStack){
            if(level->UpdateLevel != NULL) 
            {
                level->UpdateLevel(&game, level);
            }
        }
        if(node == game.levelStack || level->renderInStack){
            if(level->RenderLevel != NULL) 
            {
                level->RenderLevel(&game, level);
            }
        }
        if (node == game.levelStack || level->updateInStack) {
            if (level->FlushEntities != NULL) {
                level->FlushEntities(level);
            }
        }
        node = node->next;
    }
    if(game.levelStack == NULL){
        DrawText("No existen niveles en el juego.", 20, 20, 24, WHITE);
    }

    if(game.currentCursor)
    {
        DrawTexturePro(
        game.currentCursor->texture, 
        (Rectangle){ 0, 0, game.currentCursor->texture.width, game.currentCursor->texture.height }, 
        (Rectangle){ GetMousePosition().x, GetMousePosition().y, game.currentCursor->texture.width * game.currentCursor->scalex, game.currentCursor->texture.height * game.currentCursor->scaley }, 
        (Vector2){0,0}, 0.0f, WHITE);
    }

    EndDrawing();
    }
}

bool SetCustomCursor(const char *name)
{
  if(!name) return false;

  if(game.currentCursor)
    if(strcmp(game.currentCursor->name, name) == 0)
      return false;

  for(int i = 0; i < game.cursorsCount; i++)
  {
    if(strcmp(game.cursors[i]->name, name) == 0)
    {
      if(game.currentCursor) 
      {
        if(game.currentCursor->isAnimated)
        {
          game.currentCursor->currentFrame = 0;
          game.currentCursor->frameCounter = 0;
        }
      }
      game.currentCursor = game.cursors[i];
      HideCursor();
      return true;
    }
  }

  return false;
}

bool LoadSprite(const char *texFilename, const char *name)
{
  TextureResult result = AddTexture(&game.resManager, texFilename);
  if(!result.success) return false;

  ResSprite* sprite = CreateSprite(name, result.textureIndex, 0, 0, result.textureWidth, result.textureHeight, (Vector2){0,0}, 0);
  if(!sprite){ PushTextureIndexForError(&game.resManager, result.textureIndex); return false; }

  AddSprite(&game.resManager, sprite);

  return true;
}

bool LoadSpriteWithOptions(const char *texFilename, const char *sprFilename)
{
  FILE* file;
  if(fopen_s(&file, sprFilename, "r") != 0){  printf("Error en la carga del archivo .sprite\n"); return false; }

  TextureResult result = AddTexture(&game.resManager, texFilename);
  if(result.success == false){  printf("Error en la carga de la textura\n"); fclose(file); return false; }

  ResSprite* sprite = CreateSprite("null", -1, -1, -1, -1, -1, (Vector2){0,0}, -1);
  if(!sprite){ printf("Error en la creacion del sprite\n"); fclose(file); PushTextureIndexForError(&game.resManager, result.textureIndex); return false; }

  int sliceX = 1;
  int sliceY = 1;

  char buffer[512]; // el buffer tiene un tamaño límite de 512 caracteres.
  char sname[256]; // límite en el nomnbre de 255(se reserva uno para '\0') en el nombre de la animación.

  while(fgets(buffer, sizeof(buffer), file))
  {
    if(buffer[0] == '#') { continue;} // ignora los comentarios

    if(strstr(buffer, "@Info"))
    {
      while(fgets(buffer, sizeof(buffer), file) && buffer[0] == ' ')
      {
        if(sscanf_s(buffer, "  name: %255s", sname, (unsigned)_countof(sname))) { continue;}
        sscanf_s(buffer, "  slice_x: %d", &sliceX);
        sscanf_s(buffer, "  slice_y: %d", &sliceY);
      }

      printf("Slice x: %d | Slice y: %d\n", sliceX, sliceY);
      int framesCount = sliceX * sliceY;
      sprite->framesCount = framesCount;
      sprite->frames = (ResFrame**)realloc(sprite->frames, framesCount * sizeof(ResFrame*));

      int frameWidth = result.textureWidth / sliceX;
      int frameHeight = result.textureHeight / sliceY;
      for(int i = 0; i < sliceY; i++)
      {
        for(int j = 0; j < sliceX; j++)
        {
            AddFrameToSprite(sprite, frameWidth * j, frameHeight * i, frameWidth, frameHeight);
        }
      }
    }
    else if(strstr(buffer, "@Animation"))
    {
      char name[256];
      int fIndex = 0;
      int tIndex = 0;
      bool loop = false;
      int speed = 8;
      while(fgets(buffer, sizeof(buffer), file) && buffer[0] == ' ')
      {
        if(sscanf_s(buffer, "  name: %s", name, (unsigned)_countof(name))) continue;
        sscanf_s(buffer, "  from: %d", &fIndex);
        sscanf_s(buffer, "  to: %d", &tIndex);
        sscanf_s(buffer, "  speed: %d", &speed);
        if(strstr(buffer, "loop: true")) loop = true;
      }
      AddAnimationToSprite(sprite, (char*)name, fIndex, tIndex, loop, speed);
    }
  }
  
  free(sprite->name);
  sprite->name = _strdup(sname);
  sprite->textureIndex = result.textureIndex;
   
  AddSprite(&game.resManager, sprite);
  
  fclose(file);
  return true;
}

bool LoadSpriteAtlas(const char *texFilename, const char *atlasFilename)
{
  FILE* file;
  if(fopen_s(&file, atlasFilename, "r") != 0){ return false; }

  TextureResult result = AddTexture(&game.resManager, texFilename);
  if(!result.success){ fclose(file); return false; }

  char buffer[512];
  char sname[256];

  while(fgets(buffer, sizeof(buffer), file))
  {
    if(buffer[0] == '#') continue; // ignorar comentarios
    if(strstr(buffer, "@Sprite"))
    {
      int x = 0, y = 0, w = 0, h = 0;
      while(fgets(buffer, sizeof(buffer), file) && buffer[0] == ' ')
      {
        if(sscanf_s(buffer, "  name: %s", sname, (unsigned)_countof(sname))) continue;
        sscanf_s(buffer, "  x: %d", &x);
        sscanf_s(buffer, "  y: %d", &y);
        sscanf_s(buffer, "  w: %d", &w);
        sscanf_s(buffer, "  h: %d", &h);
      }

      ResSprite* sprite = CreateSprite((char*)sname, result.textureIndex, x, y, w, h, (Vector2){0,0}, 0);
      if(sprite) AddSprite(&game.resManager, sprite);
    }
  }

  fclose(file);
  return true;
}

bool LoadTilesetPack(const char *filename, const char *name)
{
  TilesetPack* pack = CreateTilesetPack(name, filename);
  if(!pack) return false;

  AddTilesetPack(&game.resManager, pack);
  return true;
}

bool LoadTilesetToPack(const char *filename, const char *packname)
{
  TilesetPack* pack = GetTilesetPack(&game.resManager, packname);
  if(!pack) return false;

  bool result = AddTilesetToPack(pack, filename);
  return result;
}

bool LoadTileMap(const char *filename, const char *packname, const char *name)
{
  TilesetPack* pack = GetTilesetPack(&game.resManager, packname);
  if(!packname) return false;

  TileMap* map = CreateMap(name, filename, pack);
  if(!map) return false;

  AddTileMap(&game.resManager, map);
  return true;
}

void SetLevel(const char *name, bool keepInMemory, bool renderInStack, bool updateInStack, void (*OnLoad)(Game *game, Level *level))
{
  while(game.levelStack)
  {
    PopLevel();
  }
  PushLevel(name, keepInMemory, renderInStack, updateInStack, OnLoad);
}

void PushLevel(const char *name, bool keepInMemory, bool renderInStack, bool updateInStack, void (*OnLoad)(Game *game, Level *level))
{
  LevelNode* cached = game.cacheLevels;
  LevelNode* prev = NULL;
  while(cached)
  {
    if(strcmp(cached->level->name, name) == 0)
    {
      if(prev) prev->next = cached->next;
      else game.cacheLevels = cached->next;

      cached->next = game.levelStack;
      game.levelStack = cached;
      return;
    }
    prev = cached;
    cached = cached->next;
  }

  Level* level = CreateLevel(name, keepInMemory, renderInStack, updateInStack, OnLoad, NULL, InitEntities);
  if(!level) return;

  LevelNode* node = (LevelNode*)malloc(sizeof(LevelNode));
  if(!node){ free(level->name); free(level); return; }

  node->level = level;
  node->next = game.levelStack;
  game.levelStack = node;

  DefineComponents(level);
  RegisterHooks(level);
  level->UnloadLevel = NULL;
  level->UpdateLevel = UpdateLevel;
  level->RenderLevel = RenderLevel;
  level->FlushEntities = FLushDestroyEntities;
  if(level->LoadLevel) level->LoadLevel(&game, level);
}

void PopLevel()
{
  if(!game.levelStack) return;

  LevelNode* temp = game.levelStack;
  game.levelStack = game.levelStack->next;

  if(temp->level->keepInMemory)
  {
    temp->next = game.cacheLevels;
    game.cacheLevels = temp;
  }
  else
  {
    UnloadLevel(temp->level);
    free(temp);
  }
}

Level* GetCurrentLevel()
{
    return game.levelStack->level;
}

void UnloadLevel(Level *level)
{
  if(!level) return;
  
  // registrar los hooks de los componentes

  free(level->name);
  ecs_fini(level->world);
  free(level);
}

ecs_entity_t Create2DEntity(Level *level, const char *name, const char *tag)
{
  //DefineComponents(level);

  ecs_entity_t entity = 0;

  if(name != NULL)
  {
    struct ecs_entity_desc_t desc = { 0 };
    desc.name = name;

    entity = ecs_entity_init(level->world, &desc);
    printf("Se creo la entidad con tag <%s> y name <%s>\n", tag, name);

    ecs_set(level->world, entity, Info, { _strdup(name), _strdup(tag) });
    ecs_set(level->world, entity, Transform2D, { 0.0f, 0.0f, 1.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f });

    return entity;
  }
  entity = ecs_new(level->world);
  ecs_set(level->world, entity, Info, { NULL, NULL });
  ecs_set(level->world, entity, Transform2D, { 0.0f, 0.0f, 1.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f });
  return entity;
}

void* GetComponent(Level *level, ecs_entity_t entity, const char *componentID)
{
  //DefineComponents(level);

  if(strcmp(componentID, INFO_ID) == 0)
    return ecs_has(level->world, entity, Info) ? (Info*)ecs_get(level->world, entity, Info) : NULL;
  else if(strcmp(componentID, TRANSFORM_2D_ID) == 0)
    return ecs_has(level->world, entity, Transform2D) ? (Transform2D*)ecs_get(level->world, entity, Transform2D) : NULL;
  else if(strcmp(componentID, CAMERA_2D_ID) == 0)
    return ecs_has(level->world, entity, Camera2d) ? (Camera2d*)ecs_get(level->world, entity, Camera2d) : NULL;
  else if(strcmp(componentID, SPRITE_RENDERER_ID) == 0)
    return ecs_has(level->world, entity, SpriteRender) ? (SpriteRender*)ecs_get(level->world, entity, SpriteRender) : NULL;
  else if(strcmp(componentID, ANIMATION_CONTROLLER_ID) == 0)
    return ecs_has(level->world, entity, AnimationController) ? (AnimationController*)ecs_get(level->world, entity, AnimationController) : NULL;
  else if(strcmp(componentID, RENDER_LAYER_ID) == 0)
    return ecs_has(level->world, entity, RenderLayer) ? (RenderLayer*)ecs_get(level->world, entity, RenderLayer) : NULL;
  else if(strcmp(componentID, DYEING_ID) == 0)
    return ecs_has(level->world, entity, Dyeing) ? (Dyeing*)ecs_get(level->world, entity, Dyeing) : NULL;
  else if(strcmp(componentID, MAP_RENDER_ID) == 0)
    return ecs_has(level->world, entity, MapRender) ? (MapRender*)ecs_get(level->world, entity, MapRender) : NULL;
  else if(strcmp(componentID, LUA_SCRIPT_ID) == 0)
    return ecs_has(level->world, entity, LuaScript) ? (LuaScript*)ecs_get(level->world, entity, LuaScript) : NULL;
  else if(strcmp(componentID, PYTHON_SCRIPT_ID) == 0)
    return ecs_has(level->world, entity, PythonScript) ? (PythonScript*)ecs_get(level->world, entity, PythonScript) : NULL;
  else if(strcmp(componentID, RECT_COLLIDER_ID) == 0)
    return ecs_has(level->world, entity, RectCollider) ? (RectCollider*)ecs_get(level->world, entity, RectCollider) : NULL;
  else if(strcmp(componentID, CIRCLE_COLLIDER_ID) == 0)
    return ecs_has(level->world, entity, CircleCollider) ? (CircleCollider*)ecs_get(level->world, entity, CircleCollider) : NULL;

  return NULL;
}

void DestroyEntity(ecs_world_t* world, ecs_entity_t entity)
{
  LuaScript* lscript = (LuaScript*)ecs_get(world, entity, LuaScript);
  if(lscript) 
  {
    // ejecutar el {on_destroy} del script de lua.
      if (lscript->isValid) { LuaOnDestroy(lscript, entity); }
  }

  PythonScript* pscript = (PythonScript*)ecs_get(world, entity, PythonScript);
  if(pscript)
  {
    // ejecutar el {on_destroy} del script de python.
  }

  ecs_delete(world, entity);
}

ecs_entity_t AddEntity2DChild(Level *level, ecs_entity_t parent, const char *name, const char *tag)
{
  ecs_entity_t entity = Create2DEntity(level, name, tag);
  ecs_add_pair(level->world, entity, EcsChildOf, parent);

  return entity;
}

ecs_entity_t GetChildFromIndex(Level *level, ecs_entity_t parent, int index)
{
  ecs_query_t* query = ecs_query_init(level->world, &(ecs_query_desc_t){
    .terms = {
      { .id = ecs_pair(EcsChildOf, parent) }
    }
  });

  ecs_entity_t child = 0;
  ecs_iter_t it = ecs_query_iter(level->world, query);
  int count = 0;
  while(ecs_query_next(&it))
  {
    if(count + it.count > index)
    {
      child = it.entities[index - count];
      break;
    }
    count += it.count;
  }
  ecs_iter_fini(&it);
  ecs_query_fini(query);
  return child;
}

void* AddComponent(Level *level, ecs_entity_t entity, const char *component, const char *cdata)
{
  if(strcmp(component, CAMERA_2D_ID) == 0)
  {
    // string esperado: main=true,offsetx=float,offsety=float,zoom=float
    if(ecs_has(level->world, entity, Camera2d)) return (Camera2d*)ecs_get(level->world, entity, Camera2d);

    char* data = _strdup(cdata);
    if(!data){ return NULL; }

    bool isMain = false;
    float offsetx = 0.0f, offsety = 0.0f, zoom = 1.0f;

    ParseBool(data, "main", &isMain);
    ParseFloat(data, "offsetx", &offsetx);
    ParseFloat(data, "offsety", &offsety);
    ParseFloat(data, "zoom", &zoom);

    ecs_set(level->world, entity, Camera2d, { isMain, offsetx, offsety, zoom });
    free(data);
    return (Camera2d*)ecs_get(level->world, entity, Camera2d);
  }
  else if(strcmp(component, SPRITE_RENDERER_ID) == 0)
  {
    if(ecs_has(level->world, entity, SpriteRender)) return (SpriteRender*)ecs_get(level->world, entity, SpriteRender);

    char* data = _strdup(cdata);
    if(!data) return NULL;

    char* spriteName = ParseDynamicString(data, "sprite");
    if(!spriteName)
    {
      free(data);
      printf("Error al procesar el nombre del sprite\n");
      return NULL;
    }
    bool visible = true, flipx = false, flipy = false;
    float opacity = 1.0f;

    ParseBool(data, "visible", &visible);
    ParseBool(data, "flipx", &flipx);
    ParseBool(data, "flipy", &flipy);
    ParseFloat(data, "opacity", &opacity);
    
    ecs_set(level->world, entity, SpriteRender, { _strdup(spriteName), visible, opacity, flipx, flipy });
    free(data);
    free(spriteName);
    return (SpriteRender*)ecs_get(level->world, entity, SpriteRender);
  }
  else if(strcmp(component, ANIMATION_CONTROLLER_ID) == 0)
  {
    if(ecs_has(level->world, entity, AnimationController)) return (AnimationController*)ecs_get(level->world, entity, AnimationController);

    char* data = _strdup(cdata);
    if(!data) return NULL;

    char* spriteName = ParseDynamicString(data, "sprite");
    if(!spriteName) { free(data); return NULL; }

    ecs_set(level->world, entity, AnimationController, { _strdup(spriteName), NULL, 0, 0, 0, 0, false, 0 });
    free(data);
    return (AnimationController*)ecs_get(level->world, entity, AnimationController);
  }
  else if(strcmp(component, RENDER_LAYER_ID) == 0)
  {
    if(ecs_has(level->world, entity, RenderLayer)) return (RenderLayer*)ecs_get(level->world, entity, RenderLayer);

    char* data = _strdup(cdata);
    if(!data) return NULL;

    int index = 0;
    ParseInt(data, "index", &index);

    ecs_set(level->world, entity, RenderLayer, { index });
    free(data);
    return (RenderLayer*)ecs_get(level->world, entity, RenderLayer);
  }
  else if(strcmp(component, DYEING_ID) == 0)
  {
    if(ecs_has(level->world, entity, Dyeing)) return (Dyeing*)ecs_get(level->world, entity, Dyeing);

    char* data = _strdup(cdata);
    if(!data) return NULL;

    float r = 1.0f, g = 1.0f, b = 1.0f;
    ParseFloat(data, "red", &r);
    ParseFloat(data, "green", &g);
    ParseFloat(data, "blue", &b);

    ecs_set(level->world, entity, Dyeing, { r, g, b });
    free(data);
    return (Dyeing*)ecs_get(level->world, entity, Dyeing);
  }
  else if(strcmp(component, MAP_RENDER_ID) == 0)
  {
    if(ecs_has(level->world, entity, MapRender)) return (MapRender*)ecs_get(level->world, entity, MapRender);

    char* data = _strdup(cdata);
    if(!data) return NULL;

    char* mapName = ParseDynamicString(data, "map");
    if(!mapName) { free(data); return NULL; }

    ecs_set(level->world, entity, MapRender, { _strdup(mapName) });

    free(data);
    free(mapName);
    return (MapRender*)ecs_get(level->world, entity, MapRender);
  }
  else if(strcmp(component, LUA_SCRIPT_ID) == 0)
  {
    if(ecs_has(level->world, entity, LuaScript)) return (LuaScript*)ecs_get(level->world, entity, LuaScript);

    char* data = _strdup(cdata);
    if(!data) return NULL;

    char* smodule = ParseDynamicString(data, "module");
    if(!smodule) { free(data); return NULL; }

    ecs_set(level->world, entity, LuaScript, { _strdup(smodule) });
    LuaScript* script = (LuaScript*)ecs_get(level->world, entity, LuaScript);
    script->isValid = true;
    InitLuaScript(script, entity, script->module, level->world, level, LuaGetComponent, LuaGetEntityByName, LuaGetEntityByTag, LuaPlayAnimation, LuaDestroyEntity, LuaAddComponent, LuaRemoveComponent, LuaCreate2DEntity, LuaQuitGame);
    //RegisterLuaFunctions(script->L, level->world, LuaGetComponent);
    //AssignEntityMethods(script->L, entity, LuaGetComponent);

    free(data);
    free(smodule);
    return (LuaScript*)ecs_get(level->world, entity, LuaScript);
  }
  else if(strcmp(component, PYTHON_SCRIPT_ID) == 0)
  {
    if(ecs_has(level->world, entity, PythonScript)) return (PythonScript*)ecs_get(level->world, entity, PythonScript);

    char* data = _strdup(cdata);
    if(!data) return NULL;

    char* smodule = ParseDynamicString(data, "module");
    if(!smodule){ free(data); return NULL; }

    ecs_set(level->world, entity, PythonScript, { _strdup(smodule) });

    free(data);
    free(smodule);
    return (PythonScript*)ecs_get(level->world, entity, PythonScript);
  }
  else if(strcmp(component, RECT_COLLIDER_ID) == 0)
  {
    if(ecs_has(level->world, entity, RectCollider)) return (RectCollider*)ecs_get(level->world, entity, RectCollider);

    char* data = _strdup(cdata);
    if(!data) return NULL;

    float offsetx = 0.0f, offsety = 0.0f;
    int width = 50, height = 50;
    bool isSolid = true, isStatic = true;

    ParseFloat(data, "offsetx", &offsetx);
    ParseFloat(data, "offsety", &offsety);
    ParseInt(data, "width", &width);
    ParseInt(data, "height", &height);
    ParseBool(data, "solid", &isSolid);
    ParseBool(data, "static", &isStatic);

    ecs_set(level->world, entity, RectCollider, { 0.0f, 0.0f, offsetx, offsety, width, height, isSolid, isStatic });
    free(data);

    return (RectCollider*)ecs_get(level->world, entity, RectCollider);
  }
  else if(strcmp(component, CIRCLE_COLLIDER_ID) == 0)
  {
    if(ecs_has(level->world, entity, CircleCollider)) return (CircleCollider*)ecs_get(level->world, entity, CircleCollider);

    char* data = _strdup(cdata);
    if(!data) return NULL;

    float offsetx = 0.0f, offsety = 0.0f, radius = 20.0f;
    bool isSolid = true, isStatic = true;

    ParseFloat(data, "offsetx", &offsetx);
    ParseFloat(data, "offsety", &offsety);
    ParseFloat(data, "radius", &radius);
    ParseBool(data, "solid", &isSolid);
    ParseBool(data, "static", &isStatic);

    free(data);
    return (CircleCollider*)ecs_get(level->world, entity, CircleCollider);
  }

  return NULL;
}

void RegisterHooks(Level *level)
{
  ecs_set_hooks(level->world, Info, { .dtor = ComponentInfoHook });
  ecs_set_hooks(level->world, SpriteRender, { .dtor = ComponentSpriteRenderHook });
  ecs_set_hooks(level->world, AnimationController, { .dtor = ComponentAnimationHook });
  ecs_set_hooks(level->world, MapRender, { .dtor = ComponentMapRenderHook });
  ecs_set_hooks(level->world, LuaScript, { .dtor = ComponentLuaScriptHook });
  ecs_set_hooks(level->world, PythonScript, { .dtor = ComponentPythonScriptHook });
}

ecs_entity_t GetMainCamera(Level *level)
{
  ecs_query_t* query = ecs_query_init(level->world, &(ecs_query_desc_t){
    .terms = {
      { .id = ecs_id(Camera2d) }
    }
  });

  ecs_entity_t camera = 0;

  ecs_iter_t it = ecs_query_iter(level->world, query);
  while(ecs_query_next(&it))
  {
    for(int i = 0; i < it.count; i++)
    {
      ecs_entity_t ent = it.entities[i];
      Camera2d* comp = (Camera2d*)ecs_get(level->world, ent, Camera2d);
      if(comp->isMain)
      {
        camera = ent;
        break;
      }
    }
  }
  ecs_query_fini(query);

  return camera;
}

void InitEntities(Game* gameInstance, Level* level)
{
    ecs_query_t* query = ecs_query_init(level->world, &(ecs_query_desc_t){
        .terms = {
            { .id = ecs_id(LuaScript)}
        }
    });
    ecs_iter_t it = ecs_query_iter(level->world, query);
    while (ecs_query_next(&it))
    {
        for (int i = 0; i < it.count; i++) 
        {
            LuaScript* script = ecs_get(level->world, it.entities[i], LuaScript);
            if(script->isValid)
            {
                lua_getglobal(script->L, "OnCreate");
                if (lua_isfunction(script->L, -1)) {
                    if (lua_pcall(script->L, 0, 0, 0) != LUA_OK) {
                        printf("Error en el script <%s>: %s\n", script->module, lua_tostring(script->L, -1));
                    }
                }
                else {
                    lua_pop(script->L, 1);
                }
            }
        }
    }
    
    //ecs_query_fini(query);
}

void UpdateLevel(Game* gameInstance, Level *level)
{
  /*******************************************************************************************
   * SCRIPTS
  *******************************************************************************************/
  ecs_query_t* scriptsQuery = ecs_query_init(level->world, &(ecs_query_desc_t){
    .terms = {
      { .id = ecs_id(LuaScript) }
    }
  });
  ecs_iter_t scriptsIter = ecs_query_iter(level->world, scriptsQuery);
  while(ecs_query_next(&scriptsIter))
  {
    for(int i = 0; i < scriptsIter.count; i++)
    {
      ecs_entity_t entity = scriptsIter.entities[i];
      LuaScript* script = (LuaScript*)ecs_get(level->world, entity, LuaScript);
      if(script->isValid) LuaUpdate(script, entity, level);
      //LuaSetComponentField(script->L);
    }
  }
  ecs_query_fini(scriptsQuery);

  /*******************************************************************************************
   * ANIMATIONS
  *******************************************************************************************/
  ecs_query_t* animationQuery = ecs_query_init(level->world, &(ecs_query_desc_t){
    .terms = {
      { .id = ecs_id(AnimationController) }
    }
  });
  ecs_iter_t animationIter = ecs_query_iter(level->world, animationQuery);
  while(ecs_query_next(&animationIter))
  {
    for(int i = 0; i < animationIter.count; i++)
    {
      ecs_entity_t ent = animationIter.entities[i];
      AnimationController* component = (AnimationController*)ecs_get(level->world, ent, AnimationController);
      if(component->currentAnimation == NULL) continue;
      component->frameCounter += 1;

      if(component->frameCounter >= (game.config.targetFPS / component->speed))
      {
        component->frameCounter = 0;
        component->currentFrame += 1;
        if(component->currentFrame > component->tIndex)
        {
            LuaScript* script = ecs_get(level->world, ent, LuaScript);
            if (script) {
                if (script->isValid) { LuaOnEndAnimation(script, ent, level, component->currentAnimation); }
            }
          if(component->loop)
            component->currentFrame = component->fIndex;
          else
            component->currentFrame = component->tIndex;
        }
      }
    }
  }
  ecs_query_fini(animationQuery);

  /*******************************************************************************************
   * UPDATE CHILDS
  *******************************************************************************************/
  ecs_query_t* childsQuery = ecs_query_init(level->world, &(ecs_query_desc_t){
    .terms = {
      { .id = ecs_id(Transform2D) },
      { .id = ecs_pair(EcsChildOf, EcsWildcard) }
    }
  });
  ecs_iter_t childsIter = ecs_query_iter(level->world, childsQuery);
  while(ecs_query_next(&childsIter))
  {
    for(int i = 0; i < childsIter.count; i++)
    {
      ecs_entity_t ent = childsIter.entities[i];
      ecs_entity_t par = ecs_get_target(level->world, ent, EcsChildOf, 0);
      if(par)
      {
        const Transform2D* t = ecs_get(level->world, par, Transform2D);
        if(t)
        {
          Transform2D* childT = (Transform2D*)ecs_get(level->world, ent, Transform2D);

          childT->relx = t->posx + childT->posx;
          childT->rely = t->posy + childT->posy;
          //childT->relsx = t->scalex * childT->scalex;
          //childT->relsy = t->scaley * childT->scaley;
        }
      }
    }
  }
  ecs_query_fini(childsQuery);

  /*******************************************************************************************
   * COLLISIONS
  *******************************************************************************************/
  ecs_query_t* colliderQuery = ecs_query_init(level->world, &(ecs_query_desc_t){
    .terms = {
      { .id = ecs_id(CircleCollider), .oper = EcsOr },
      { .id = ecs_id(RectCollider) }
    }
  });
  ecs_iter_t colliderIter = ecs_query_iter(level->world, colliderQuery);
  while(ecs_query_next(&colliderIter))
  {
    for(int i = 0; i < colliderIter.count; i++)
    {
      ecs_entity_t entity = colliderIter.entities[i];
      Transform2D* transform = (Transform2D*)ecs_get(level->world, entity, Transform2D);
      RectCollider* rectCollider = (RectCollider*)ecs_get(level->world, entity, RectCollider);
      CircleCollider* circleCollider = (CircleCollider*)ecs_get(level->world, entity, CircleCollider);
      if(rectCollider)
      {
        rectCollider->posx = transform->posx + (rectCollider->offsetx * game.config.globalScale * transform->scalex);
        rectCollider->posy = transform->posy + (rectCollider->offsety * game.config.globalScale * transform->scaley);
      }
      else if(circleCollider)
      {
        circleCollider->posx = transform->posx + circleCollider->offsetx;
        circleCollider->posy = transform->posy + circleCollider->offsety;
      }
    }

    for(int i = 0; i < colliderIter.count; i++)
    {
      ecs_entity_t ent = colliderIter.entities[i];
      RectCollider* rectCollider = (RectCollider*)ecs_get(level->world, ent, RectCollider);
      CircleCollider* circleCollider = (CircleCollider*)ecs_get(level->world, ent, CircleCollider);
      Transform2D* transform = (Transform2D*)ecs_get(level->world, ent, Transform2D);

      ecs_iter_t innerIter = ecs_query_iter(level->world, colliderQuery);
      while(ecs_query_next(&innerIter))
      {
        for(int j = 0; j < innerIter.count; j++)
        {
          ecs_entity_t iEnt = innerIter.entities[j];
          if(ent == iEnt) continue;
          if(!ecs_is_valid(level->world, iEnt)) continue;

          RectCollider* irCollider = (RectCollider*)ecs_get(level->world, iEnt, RectCollider);
          CircleCollider* icCollider = (CircleCollider*)ecs_get(level->world, iEnt, CircleCollider);
          Transform2D* iTransform = (Transform2D*)ecs_get(level->world, iEnt, Transform2D);

          if(rectCollider && icCollider)
          {
            if(IntersectionCircleRectTransformImplementation(transform, 
              (Rectangle){rectCollider->posx, rectCollider->posy, rectCollider->width, rectCollider->height},
              (Circle){icCollider->posx, icCollider->posy, icCollider->radius}, icCollider->isSolid))
            {
              if(ecs_has(level->world, ent, LuaScript)){
                  /* Llamar a la funcion "on_collide" desde lua */ 
                  LuaScript* script = (LuaScript*)ecs_get(level->world, ent, LuaScript);
                  if (script->isValid) { LuaOnCollision(script, ent, iEnt, level); }
              }
              else if(ecs_has(level->world, ent, PythonScript)){ /* Llamar a la funcion "on_collide" desde python */ }
            }
          }
          else if(rectCollider && irCollider)
          {
            if(IntersectionRectRectTransformImplementation(transform, 
              (Rectangle){rectCollider->posx, rectCollider->posy, rectCollider->width, rectCollider->height}, 
              (Rectangle){irCollider->posx, irCollider->posy, irCollider->width, irCollider->height}, irCollider->isSolid, irCollider->isStatic))
            {
              if(ecs_has(level->world, ent, LuaScript)){ 
                  /* Llamar a la funcion "on_collide" desde lua */ 
                  LuaScript* script = (LuaScript*)ecs_get(level->world, ent, LuaScript);
                  if (script->isValid) { LuaOnCollision(script, ent, iEnt, level); }
              }
              else if(ecs_has(level->world, ent, PythonScript)){ /* Llamar a la funcion "on_collide" desde python */ }
            }
          }
        }
      }
    }
  }
  ecs_query_fini(colliderQuery);

  /*******************************************************************************************
   * RE-UPDATE CHILDS
  *******************************************************************************************/
  ecs_query_t* childsQueryPos = ecs_query_init(level->world, &(ecs_query_desc_t){
    .terms = {
      { .id = ecs_id(Transform2D) },
      { .id = ecs_pair(EcsChildOf, EcsWildcard) }
    }
  });
  ecs_iter_t childsIterPos = ecs_query_iter(level->world, childsQueryPos);
  while(ecs_query_next(&childsIterPos))
  {
    for(int i = 0; i < childsIterPos.count; i++)
    {
      ecs_entity_t ent = childsIterPos.entities[i];
      ecs_entity_t par = ecs_get_target(level->world, ent, EcsChildOf, 0);
      if(par)
      {
        const Transform2D* t = ecs_get(level->world, par, Transform2D);
        if(t)
        {
          Transform2D* childT = (Transform2D*)ecs_get(level->world, ent, Transform2D);

          childT->relx = t->posx + childT->posx;
          childT->rely = t->posy + childT->posy;
          //childT->relsx = t->scalex * childT->scalex * game.config.globalScale;
          //childT->relsy = t->scaley * childT->scaley * game.config.globalScale;
        }
      }
    }
  }
  ecs_query_fini(childsQueryPos);
}

void RenderLevel(Game* gameInstance, Level *level)
{
  
  ecs_entity_t mainCamera = GetMainCamera(level);
  if(mainCamera == 0)
  {
    DrawText("Debe existir una camara principal para renderizar el nivel.", 10, 10, 24, WHITE);
    return;
  }

  Camera2d* cameraComponent = (Camera2d*)ecs_get(level->world, mainCamera, Camera2d);
  Transform2D* cameraTransform = (Transform2D*)ecs_get(level->world, mainCamera, Transform2D);
  Camera2D renderCamera = { 0 };
  renderCamera.offset = (Vector2){cameraComponent->offsetx, cameraComponent->offsety};
  renderCamera.target = (Vector2){ cameraTransform->posx, cameraTransform->posy };
  renderCamera.rotation = cameraTransform->rotation;
  renderCamera.zoom = cameraComponent->zoom;

  BeginMode2D(renderCamera);
  ClearBackground(game.config.windowColor);

  ecs_query_t* renderQuery = ecs_query_init(level->world, &(ecs_query_desc_t){
    .terms = {
      { .id = ecs_id(MapRender), .oper = EcsOr },
      { .id = ecs_id(AnimationController), .oper = EcsOr },
      { .id = ecs_id(SpriteRender) },
    }
  });

  ecs_iter_t renderIter = ecs_query_iter(level->world, renderQuery);
  while(ecs_query_next(&renderIter))
  {
    for(int i = 0; i < renderIter.count; i++)
    {
      ecs_entity_t entity = renderIter.entities[i];

      Transform2D* transform = (Transform2D*)ecs_get(level->world, entity, Transform2D);
      Dyeing* color = (Dyeing*)ecs_get(level->world, entity, Dyeing);
      SpriteRender* spriteRender = (SpriteRender*)ecs_get(level->world, entity, SpriteRender);
      AnimationController* animation = (AnimationController*)ecs_get(level->world, entity, AnimationController);
      MapRender* mapRender = (MapRender*)ecs_get(level->world, entity, MapRender);

      if(animation)
      {
        ResSprite* sprite = GetSprite(&game.resManager, spriteRender->sprite);
        if(!sprite)
        {
            printf("No exist el sprite\n");
            return;
        }

        Texture2D tex = game.resManager.textures[sprite->textureIndex];
        if(animation->currentAnimation)
        {
            
          ResFrame* frame = GetSpriteFrame(sprite, animation->currentFrame);
          if(frame && spriteRender->visible)
          {
            DrawTexturePro(tex, 
            (Rectangle){frame->x, frame->y, spriteRender->flipx ? (-frame->width) : frame->width, spriteRender->flipy ? (-frame->height) : frame->height}, 
            (Rectangle){
              transform->relx == 0.0f ? transform->posx : transform->relx + sprite->origin.x,
              transform->rely == 0.0f ? transform->posy : transform->rely + sprite->origin.y,
                  transform->relsx == 0.0f ? frame->width * transform->scalex : frame->width * transform->relsx,
                  transform->relsy == 0.0f ? frame->height * transform->scaley : frame->height * transform->relsy
            }, sprite->origin, transform->rotation,
            color ? (Color){
                  (int)255 * color->r, 
                  (int)255 * color->b, 
                  (int)255 * color->g, (int)255 * spriteRender->opacity} : (Color){255, 255, 255, (int)255 * spriteRender->opacity});
          }
        }
        else
        {
          ResFrame* zeroFrame = GetSpriteFrame(sprite, 0);
          if(zeroFrame && spriteRender->visible)
          {
            DrawTexturePro(tex, 
            (Rectangle){0,0,spriteRender->flipx ? (-zeroFrame->width) : zeroFrame->width, spriteRender->flipy ? (-zeroFrame->height) : zeroFrame->height}, 
            (Rectangle){
              transform->relx == 0.0f ? transform->posx : transform->relx + sprite->origin.x,
              transform->rely == 0.0f ? transform->posy : transform->rely + sprite->origin.y,
              transform->relsx == 0.0f ? zeroFrame->width * transform->scalex * game.config.globalScale : zeroFrame->width * transform->relsx * game.config.globalScale,
              transform->relsy == 0.0f ? zeroFrame->height * transform->scaley * game.config.globalScale : zeroFrame->height * transform->relsy * game.config.globalScale}, 
              sprite->origin, 
              transform->rotation, 
              color ? (Color){
                (int)255 * color->r, 
                (int)255 * color->g, 
                (int)255 * color->b, (int)255 * spriteRender->opacity} : (Color){255, 255, 255, (int)255 * spriteRender->opacity});
          }
        }
      }
      else if(mapRender)
      {
        TileMap* map = GetTileMap(&game.resManager, mapRender->map);
        if(!map)continue;

        DrawTexturePro(map->texture.texture, 
          (Rectangle){ 0, 0, map->texture.texture.width, -map->texture.texture.height }, 
          (Rectangle){
            transform->relx == 0.0f ? transform->posx : transform->relx,
            transform->rely == 0.0f ? transform->posy : transform->rely,
            transform->relsx == 0.0f ? map->texture.texture.width * transform->scalex * game.config.globalScale : map->texture.texture.width * transform->relsx,
            transform->relsy == 0.0f ? map->texture.texture.height * transform->scaley * game.config.globalScale : map->texture.texture.height * transform->relsy
          }, (Vector2){0,0}, transform->rotation,
          color ? (Color){
                (int)255 * color->r, 
                (int)255 * color->g, 
                (int)255 * color->b, 255} : (Color){255, 255, 255, 255});
      }
      else if(spriteRender && !animation)
      {
        ResSprite* sprite = GetSprite(&game.resManager, spriteRender->sprite);
        if(!sprite) return;

        if(spriteRender->visible)
        {
          Texture2D tex = game.resManager.textures[sprite->textureIndex];
          int opacity = 255 * spriteRender->opacity;
          DrawTexturePro(tex, 
            (Rectangle){sprite->x, sprite->y, spriteRender->flipx ? -sprite->width : sprite->width, spriteRender->flipy ? -sprite->height : sprite->height}, 
            (Rectangle){
              transform->relx == 0.0f ? transform->posx : transform->relx + sprite->origin.x,
              transform->rely == 0.0f ? transform->posy : transform->rely + sprite->origin.y,
              transform->relsx == 0.0f ? sprite->width * transform->scalex * game.config.globalScale : sprite->width * transform->relsx,
              transform->relsy == 0.0f ? sprite->height * transform->scaley * game.config.globalScale : sprite->height * transform->relsy},
              sprite->origin, transform->rotation, 
              color ? (Color){
                (int)255 * color->r, 
                (int)255 * color->g, 
                (int)255 * color->b, (int)255 * spriteRender->opacity} : (Color){255, 255, 255, opacity});
        }
      }
    }
  }
  ecs_query_fini(renderQuery);

  // render debug

  EndMode2D();

  // render UI
}

void FLushDestroyEntities(Level* level)
{
    for (int i = 0; i < level->destroyQueueCount; i++)
    {
        DestroyEntity(level->world, level->destroyQueue[i]);
        level->destroyQueue[i] = 0;
    }
    level->destroyQueueCount = 0;
}

int GameDestroy()
{
  // Unload level stack
  while(game.levelStack)
  {
    LevelNode* temp = game.levelStack;
    game.levelStack = game.levelStack->next;
    UnloadLevel(temp->level);
    free(temp);
  }

  while(game.cacheLevels)
  {
    LevelNode* temp = game.cacheLevels;
    game.cacheLevels = game.cacheLevels->next;
    UnloadLevel(temp->level);
    free(temp);
  }

  // Unload cursors
  for(int i = 0; i < game.cursorsCount; i++)
  {
    UnloadTexture(game.cursors[i]->texture);
    free(game.cursors[i]->name);
    free(game.cursors[i]);
  }
  if(game.cursors) free(game.cursors);

  //CleanupLuaCallbacks(game.LuaCallbacksSate);
  //lua_close(game.LuaCallbacksSate);

  UnloadResourcesManager(&game.resManager);
  CloseWindow();

  return game.exitCode;
}
