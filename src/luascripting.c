#include "luascripting.h"

void InitLuaScript(LuaScript* script, ecs_entity_t entity, const char* module, ecs_world_t* world, int(*LuaGetComponent)(lua_State* L), int (*LuaGetEntityByName)(lua_State* L), int (*LuaGetEntityByTag)(lua_State* L), int(*LuaPlayAnimation)(lua_State* L))
{
  script->L = luaL_newstate();
  luaL_openlibs(script->L);

  RegisterInputConstants(script->L);
  RegisterLuaFunctions(script->L, world, LuaGetComponent, LuaGetEntityByName, LuaGetEntityByTag, LuaPlayAnimation);

  lua_getglobal(script->L, "Entities");
  if(!lua_istable(script->L, -1)){
    lua_pop(script->L, 1);
    lua_newtable(script->L);
    lua_setglobal(script->L, "Entities");
  }

  lua_getglobal(script->L, "Entities");

  lua_newtable(script->L);

  lua_pushinteger(script->L, entity);
  lua_setfield(script->L, -2, "id");

  lua_pushlightuserdata(script->L, world);
  lua_setfield(script->L, -2, "world");

  lua_newtable(script->L);
  lua_pushlightuserdata(script->L, world);
  lua_pushcclosure(script->L, LuaGetComponent, 1);
  lua_setfield(script->L, -2, "GetComponent");

  lua_pushlightuserdata(script->L, world);
  lua_pushcclosure(script->L, LuaPlayAnimation, 1);
  lua_setfield(script->L, -2, "PlayAnimation");

  lua_pushvalue(script->L, -1);
  lua_setfield(script->L, -2, "__index");

  lua_setmetatable(script->L, -2);

  lua_pushinteger(script->L, entity);
  lua_pushvalue(script->L, -2);
  lua_settable(script->L, -4);

  lua_setglobal(script->L, "self");

  lua_pop(script->L, 1);


  if (luaL_loadfile(script->L, module) || lua_pcall(script->L, 0, 0, 0)) {
      printf("Error al cargar el script <%s>: %s\n", module, lua_tostring(script->L, -1));
      lua_pop(script->L, 1);
      script->isValid = false;
      lua_close(script->L);
      script->L = NULL;
      return;
  }
  
}

void RegisterLuaFunctions(lua_State *L, ecs_world_t *world, int(*LuaGetComponent)(lua_State * L), int(*LuaGetEntityByName)(lua_State* L), int (*LuaGetEntityByTag)(lua_State* L), int(*LuaPlayAnimation)(lua_State* L))
{
    lua_register(L, "KeyDown", LuaKeyDown);
    lua_register(L, "KeyDownOnce", LuaKeyDownOnce);
    lua_register(L, "KeyUp", LuaKeyUp);
    lua_register(L, "KeyUpOnce", LuaKeyUpOnce);
    lua_register(L, "GetMousePosition", LuaGetMousePosition);
    lua_register(L, "GetMouseDelta", LuaGetMouseDelta);

    lua_pushlightuserdata(L, world);
    lua_pushcclosure(L, LuaGetComponent, 1);
    lua_setglobal(L, "GetComponent");

    lua_pushlightuserdata(L, world);
    lua_pushcclosure(L, LuaPlayAnimation, 1);
    lua_setglobal(L, "PlayAnimation");

    lua_pushlightuserdata(L, world);
    lua_pushcclosure(L, LuaGetEntityByName, 1);
    lua_setglobal(L, "GetEntityByName");

    lua_pushlightuserdata(L, world);
    lua_pushcclosure(L, LuaGetEntityByTag, 1);
    lua_setglobal(L, "GetEntityByTag");
}

void AssignEntityMethods(lua_State *L, lua_Integer entity, int(*LuaGetComponent)(lua_State * L))
{
  lua_getglobal(L, "Entities");
  lua_pushinteger(L, entity);
  lua_gettable(L, -2);

  if(lua_istable(L, -1)){
    lua_pushcfunction(L, LuaGetComponent);
    lua_setfield(L, -2, "GetComponent");
  }

  lua_pop(L, 1);
}

void RegisterInputConstants(lua_State* L)
{
    lua_newtable(L);

    lua_pushinteger(L, KEY_APOSTROPHE);
    lua_setfield(L, -2, "KEY_APOSTROPHE");
    lua_pushinteger(L, KEY_COMMA);
    lua_setfield(L, -2, "KEY_COMMA");
    lua_pushinteger(L, KEY_MINUS);
    lua_setfield(L, -2, "KEY_MINUS");
    lua_pushinteger(L, KEY_PERIOD);
    lua_setfield(L, -2, "KEY_PERIOD");
    lua_pushinteger(L, KEY_SLASH);
    lua_setfield(L, -2, "KEY_SLASH");
    lua_pushinteger(L, KEY_ZERO);
    lua_setfield(L, -2, "KEY_ZERO");
    lua_pushinteger(L, KEY_ONE);
    lua_setfield(L, -2, "KEY_ONE");
    lua_pushinteger(L, KEY_TWO);
    lua_setfield(L, -2, "KEY_TWO");
    lua_pushinteger(L, KEY_THREE);
    lua_setfield(L, -2, "KEY_THREE");
    lua_pushinteger(L, KEY_FOUR);
    lua_setfield(L, -2, "KEY_FOUR");
    lua_pushinteger(L, KEY_FIVE);
    lua_setfield(L, -2, "KEY_FIVE");
    lua_pushinteger(L, KEY_SIX);
    lua_setfield(L, -2, "KEY_SIX");
    lua_pushinteger(L, KEY_SEVEN);
    lua_setfield(L, -2, "KEY_SEVEN");
    lua_pushinteger(L, KEY_EIGHT);
    lua_setfield(L, -2, "KEY_EIGHT");
    lua_pushinteger(L, KEY_NINE);
    lua_setfield(L, -2, "KEY_NINE");
    lua_pushinteger(L, KEY_SEMICOLON);
    lua_setfield(L, -2, "KEY_SEMICOLON");
    lua_pushinteger(L, KEY_EQUAL);
    lua_setfield(L, -2, "KEY_EQUAL");
    lua_pushinteger(L, KEY_A);
    lua_setfield(L, -2, "KEY_A");
    lua_pushinteger(L, KEY_B);
    lua_setfield(L, -2, "KEY_B");
    lua_pushinteger(L, KEY_C);
    lua_setfield(L, -2, "KEY_C");
    lua_pushinteger(L, KEY_D);
    lua_setfield(L, -2, "KEY_D");
    lua_pushinteger(L, KEY_E);
    lua_setfield(L, -2, "KEY_E");
    lua_pushinteger(L, KEY_F);
    lua_setfield(L, -2, "KEY_F");
    lua_pushinteger(L, KEY_G);
    lua_setfield(L, -2, "KEY_G");
    lua_pushinteger(L, KEY_H);
    lua_setfield(L, -2, "KEY_H");
    lua_pushinteger(L, KEY_I);
    lua_setfield(L, -2, "KEY_I");
    lua_pushinteger(L, KEY_J);
    lua_setfield(L, -2, "KEY_J");
    lua_pushinteger(L, KEY_K);
    lua_setfield(L, -2, "KEY_K");
    lua_pushinteger(L, KEY_L);
    lua_setfield(L, -2, "KEY_L");
    lua_pushinteger(L, KEY_M);
    lua_setfield(L, -2, "KEY_M");
    lua_pushinteger(L, KEY_N);
    lua_setfield(L, -2, "KEY_N");
    lua_pushinteger(L, KEY_O);
    lua_setfield(L, -2, "KEY_O");
    lua_pushinteger(L, KEY_P);
    lua_setfield(L, -2, "KEY_P");
    lua_pushinteger(L, KEY_Q);
    lua_setfield(L, -2, "KEY_Q");
    lua_pushinteger(L, KEY_R);
    lua_setfield(L, -2, "KEY_R");
    lua_pushinteger(L, KEY_S);
    lua_setfield(L, -2, "KEY_S");
    lua_pushinteger(L, KEY_T);
    lua_setfield(L, -2, "KEY_T");
    lua_pushinteger(L, KEY_U);
    lua_setfield(L, -2, "KEY_U");
    lua_pushinteger(L, KEY_V);
    lua_setfield(L, -2, "KEY_V");
    lua_pushinteger(L, KEY_W);
    lua_setfield(L, -2, "KEY_W");
    lua_pushinteger(L, KEY_X);
    lua_setfield(L, -2, "KEY_X");
    lua_pushinteger(L, KEY_Y);
    lua_setfield(L, -2, "KEY_Y");
    lua_pushinteger(L, KEY_Z);
    lua_setfield(L, -2, "KEY_Z");
    lua_pushinteger(L, KEY_LEFT_BRACKET);
    lua_setfield(L, -2, "KEY_LEFT_BRACKET");
    lua_pushinteger(L, KEY_BACKSLASH);
    lua_setfield(L, -2, "KEY_BACKSLASH");
    lua_pushinteger(L, KEY_RIGHT_BRACKET);
    lua_setfield(L, -2, "KEY_RIGHT_BRACKET");
    lua_pushinteger(L, KEY_GRAVE);
    lua_setfield(L, -2, "KEY_GRAVE");
    lua_pushinteger(L, KEY_SPACE);
    lua_setfield(L, -2, "KEY_SPACE");
    lua_pushinteger(L, KEY_ESCAPE);
    lua_setfield(L, -2, "KEY_ESCAPE");
    lua_pushinteger(L, KEY_ENTER);
    lua_setfield(L, -2, "KEY_ENTER");
    lua_pushinteger(L, KEY_TAB);
    lua_setfield(L, -2, "KEY_TAB");
    lua_pushinteger(L, KEY_BACKSPACE);
    lua_setfield(L, -2, "KEY_BACKSPACE");
    lua_pushinteger(L, KEY_INSERT);
    lua_setfield(L, -2, "KEY_INSERT");
    lua_pushinteger(L, KEY_DELETE);
    lua_setfield(L, -2, "KEY_DELETE");
    lua_pushinteger(L, KEY_RIGHT);
    lua_setfield(L, -2, "KEY_RIGHT");
    lua_pushinteger(L, KEY_LEFT);
    lua_setfield(L, -2, "KEY_LEFT");
    lua_pushinteger(L, KEY_DOWN);
    lua_setfield(L, -2, "KEY_DOWN");
    lua_pushinteger(L, KEY_UP);
    lua_setfield(L, -2, "KEY_UP");
    lua_pushinteger(L, KEY_PAGE_UP);
    lua_setfield(L, -2, "KEY_PAGE_UP");
    lua_pushinteger(L, KEY_PAGE_DOWN);
    lua_setfield(L, -2, "KEY_PAGE_DOWN");
    lua_pushinteger(L, KEY_HOME);
    lua_setfield(L, -2, "KEY_HOME");
    lua_pushinteger(L, KEY_END);
    lua_setfield(L, -2, "KEY_END");
    lua_pushinteger(L, KEY_CAPS_LOCK);
    lua_setfield(L, -2, "KEY_CAPS_LOCK");
    lua_pushinteger(L, KEY_SCROLL_LOCK);
    lua_setfield(L, -2, "KEY_SCROLL_LOCK");
    lua_pushinteger(L, KEY_NUM_LOCK);
    lua_setfield(L, -2, "KEY_NUM_LOCK");
    lua_pushinteger(L, KEY_PRINT_SCREEN);
    lua_setfield(L, -2, "KEY_PRINT_SCREEN");
    lua_pushinteger(L, KEY_PAUSE);
    lua_setfield(L, -2, "KEY_PAUSE");
    lua_pushinteger(L, KEY_F1);
    lua_setfield(L, -2, "KEY_F1");
    lua_pushinteger(L, KEY_F2);
    lua_setfield(L, -2, "KEY_F2");
    lua_pushinteger(L, KEY_F3);
    lua_setfield(L, -2, "KEY_F3");
    lua_pushinteger(L, KEY_F4);
    lua_setfield(L, -2, "KEY_F4");
    lua_pushinteger(L, KEY_F5);
    lua_setfield(L, -2, "KEY_F5");
    lua_pushinteger(L, KEY_F6);
    lua_setfield(L, -2, "KEY_F6");
    lua_pushinteger(L, KEY_F7);
    lua_setfield(L, -2, "KEY_F7");
    lua_pushinteger(L, KEY_F8);
    lua_setfield(L, -2, "KEY_F8");
    lua_pushinteger(L, KEY_F9);
    lua_setfield(L, -2, "KEY_F9");
    lua_pushinteger(L, KEY_F10);
    lua_setfield(L, -2, "KEY_F10");
    lua_pushinteger(L, KEY_F11);
    lua_setfield(L, -2, "KEY_F11");
    lua_pushinteger(L, KEY_F12);
    lua_setfield(L, -2, "KEY_F12");
    lua_pushinteger(L, KEY_LEFT_SHIFT);
    lua_setfield(L, -2, "KEY_LEFT_SHIFT");
    lua_pushinteger(L, KEY_LEFT_CONTROL);
    lua_setfield(L, -2, "KEY_LEFT_CONTROL");
    lua_pushinteger(L, KEY_LEFT_ALT);
    lua_setfield(L, -2, "KEY_LEFT_ALT");
    lua_pushinteger(L, KEY_LEFT_SUPER);
    lua_setfield(L, -2, "KEY_LEFT_SUPER");
    lua_pushinteger(L, KEY_RIGHT_SHIFT);
    lua_setfield(L, -2, "KEY_RIGHT_SHIFT");
    lua_pushinteger(L, KEY_RIGHT_CONTROL);
    lua_setfield(L, -2, "KEY_RIGHT_CONTROL");
    lua_pushinteger(L, KEY_RIGHT_ALT);
    lua_setfield(L, -2, "KEY_RIGHT_ALT");
    lua_pushinteger(L, KEY_RIGHT_SUPER);
    lua_setfield(L, -2, "KEY_RIGHT_SUPER");
    lua_pushinteger(L, KEY_KB_MENU);
    lua_setfield(L, -2, "KEY_KB_MENU");
    lua_pushinteger(L, KEY_KP_0);
    lua_setfield(L, -2, "KEY_KP_0");
    lua_pushinteger(L, KEY_KP_1);
    lua_setfield(L, -2, "KEY_KP_1");
    lua_pushinteger(L, KEY_KP_2);
    lua_setfield(L, -2, "KEY_KP_2");
    lua_pushinteger(L, KEY_KP_3);
    lua_setfield(L, -2, "KEY_KP_3");
    lua_pushinteger(L, KEY_KP_4);
    lua_setfield(L, -2, "KEY_KP_4");
    lua_pushinteger(L, KEY_KP_5);
    lua_setfield(L, -2, "KEY_KP_5");
    lua_pushinteger(L, KEY_KP_6);
    lua_setfield(L, -2, "KEY_KP_6");
    lua_pushinteger(L, KEY_KP_7);
    lua_setfield(L, -2, "KEY_KP_7");
    lua_pushinteger(L, KEY_KP_8);
    lua_setfield(L, -2, "KEY_KP_8");
    lua_pushinteger(L, KEY_KP_9);
    lua_setfield(L, -2, "KEY_KP_9");
    lua_pushinteger(L, KEY_KP_DECIMAL);
    lua_setfield(L, -2, "KEY_KP_DECIMAL");
    lua_pushinteger(L, KEY_KP_DIVIDE);
    lua_setfield(L, -2, "KEY_KP_DIVIDE");
    lua_pushinteger(L, KEY_KP_MULTIPLY);
    lua_setfield(L, -2, "KEY_KP_MULTIPLY");
    lua_pushinteger(L, KEY_KP_SUBTRACT);
    lua_setfield(L, -2, "KEY_KP_SUBTRACT");
    lua_pushinteger(L, KEY_KP_ADD);
    lua_setfield(L, -2, "KEY_KP_ADD");
    lua_pushinteger(L, KEY_KP_ENTER);
    lua_setfield(L, -2, "KEY_KP_ENTER");
    lua_pushinteger(L, KEY_KP_EQUAL);
    lua_setfield(L, -2, "KEY_KP_EQUAL");
    lua_pushinteger(L, KEY_BACK);
    lua_setfield(L, -2, "KEY_BACK");
    lua_pushinteger(L, KEY_MENU);
    lua_setfield(L, -2, "KEY_MENU");
    lua_pushinteger(L, KEY_VOLUME_UP);
    lua_setfield(L, -2, "KEY_VOLUME_UP");
    lua_pushinteger(L, KEY_VOLUME_DOWN);
    lua_setfield(L, -2, "KEY_VOLUME_DOWN");
    lua_pushinteger(L, MOUSE_BUTTON_LEFT);
    lua_setfield(L, -2, "MOUSE_BUTTON_LEFT");
    lua_pushinteger(L, MOUSE_BUTTON_RIGHT);
    lua_setfield(L, -2, "MOUSE_BUTTON_RIGHT");
    lua_pushinteger(L, MOUSE_BUTTON_MIDDLE);
    lua_setfield(L, -2, "MOUSE_BUTTON_MIDDLE");
    lua_pushinteger(L, MOUSE_BUTTON_SIDE);
    lua_setfield(L, -2, "MOUSE_BUTTON_SIDE");
    lua_pushinteger(L, MOUSE_BUTTON_EXTRA);
    lua_setfield(L, -2, "MOUSE_BUTTON_EXTRA");
    lua_pushinteger(L, MOUSE_BUTTON_FORWARD);
    lua_setfield(L, -2, "MOUSE_BUTTON_FORWARD");
    lua_pushinteger(L, MOUSE_BUTTON_BACK);
    lua_setfield(L, -2, "MOUSE_BUTTON_BACK");
    lua_pushinteger(L, GAMEPAD_BUTTON_UNKNOWN);
    lua_setfield(L, -2, "GAMEPAD_BUTTON_UNKNOWN");
    lua_pushinteger(L, GAMEPAD_BUTTON_LEFT_FACE_UP);
    lua_setfield(L, -2, "GAMEPAD_BUTTON_LEFT_FACE_UP");
    lua_pushinteger(L, GAMEPAD_BUTTON_LEFT_FACE_RIGHT);
    lua_setfield(L, -2, "GAMEPAD_BUTTON_LEFT_FACE_RIGHT");
    lua_pushinteger(L, GAMEPAD_BUTTON_LEFT_FACE_DOWN);
    lua_setfield(L, -2, "GAMEPAD_BUTTON_LEFT_FACE_DOWN");
    lua_pushinteger(L, GAMEPAD_BUTTON_LEFT_FACE_LEFT);
    lua_setfield(L, -2, "GAMEPAD_BUTTON_LEFT_FACE_LEFT");
    lua_pushinteger(L, GAMEPAD_BUTTON_RIGHT_FACE_UP);
    lua_setfield(L, -2, "GAMEPAD_BUTTON_RIGHT_FACE_UP");
    lua_pushinteger(L, GAMEPAD_BUTTON_RIGHT_FACE_RIGHT);
    lua_setfield(L, -2, "GAMEPAD_BUTTON_RIGHT_FACE_RIGHT");
    lua_pushinteger(L, GAMEPAD_BUTTON_RIGHT_FACE_DOWN);
    lua_setfield(L, -2, "GAMEPAD_BUTTON_RIGHT_FACE_DOWN");
    lua_pushinteger(L, GAMEPAD_BUTTON_RIGHT_FACE_LEFT);
    lua_setfield(L, -2, "GAMEPAD_BUTTON_RIGHT_FACE_LEFT");
    lua_pushinteger(L, GAMEPAD_BUTTON_LEFT_TRIGGER_1);
    lua_setfield(L, -2, "GAMEPAD_BUTTON_LEFT_TRIGGER_1");
    lua_pushinteger(L, GAMEPAD_BUTTON_LEFT_TRIGGER_2);
    lua_setfield(L, -2, "GAMEPAD_BUTTON_LEFT_TRIGGER_2");
    lua_pushinteger(L, GAMEPAD_BUTTON_RIGHT_TRIGGER_1);
    lua_setfield(L, -2, "GAMEPAD_BUTTON_RIGHT_TRIGGER_1");
    lua_pushinteger(L, GAMEPAD_BUTTON_RIGHT_TRIGGER_2);
    lua_setfield(L, -2, "GAMEPAD_BUTTON_RIGHT_TRIGGER_2");
    lua_pushinteger(L, GAMEPAD_BUTTON_MIDDLE_LEFT);
    lua_setfield(L, -2, "GAMEPAD_BUTTON_MIDDLE_LEFT");
    lua_pushinteger(L, GAMEPAD_BUTTON_MIDDLE);
    lua_setfield(L, -2, "GAMEPAD_BUTTON_MIDDLE");
    lua_pushinteger(L, GAMEPAD_BUTTON_MIDDLE_RIGHT);
    lua_setfield(L, -2, "GAMEPAD_BUTTON_MIDDLE_RIGHT");
    lua_pushinteger(L, GAMEPAD_BUTTON_LEFT_THUMB);
    lua_setfield(L, -2, "GAMEPAD_BUTTON_LEFT_THUMB");
    lua_pushinteger(L, GAMEPAD_BUTTON_RIGHT_THUMB);
    lua_setfield(L, -2, "GAMEPAD_BUTTON_RIGHT_THUMB");
    lua_pushinteger(L, GAMEPAD_AXIS_LEFT_X);
    lua_setfield(L, -2, "GAMEPAD_AXIS_LEFT_X");
    lua_pushinteger(L, GAMEPAD_AXIS_LEFT_Y);
    lua_setfield(L, -2, "GAMEPAD_AXIS_LEFT_Y");
    lua_pushinteger(L, GAMEPAD_AXIS_RIGHT_X);
    lua_setfield(L, -2, "GAMEPAD_AXIS_RIGHT_X");
    lua_pushinteger(L, GAMEPAD_AXIS_RIGHT_Y);
    lua_setfield(L, -2, "GAMEPAD_AXIS_RIGHT_Y");
    lua_pushinteger(L, GAMEPAD_AXIS_LEFT_TRIGGER);
    lua_setfield(L, -2, "GAMEPAD_AXIS_LEFT_TRIGGER");
    lua_pushinteger(L, GAMEPAD_AXIS_RIGHT_TRIGGER);
    lua_setfield(L, -2, "GAMEPAD_AXIS_RIGHT_TRIGGER");

    lua_setglobal(L, "Input");
}

int LuaKeyDown(lua_State* L)
{
    int key = luaL_checkinteger(L, 1);
    lua_pushboolean(L, IsKeyDown(key));
    return 1;
}

int LuaKeyDownOnce(lua_State* L)
{
    int key = luaL_checkinteger(L, 1);
    lua_pushboolean(L, IsKeyPressed(key));
    return 1;
}

int LuaKeyUp(lua_State* L)
{
    int key = luaL_checkinteger(L, 1);
    lua_pushboolean(L, IsKeyUp(key));
    return 1;
}

int LuaKeyUpOnce(lua_State* L)
{
    int key = luaL_checkinteger(L, 1);
    lua_pushboolean(L, IsKeyReleased(key));
    return 1;
}

int LuaGetMousePosition(lua_State* L)
{
    Vector2 pos = GetMousePosition();

    lua_newtable(L);

    lua_pushstring(L, "x");
    lua_pushnumber(L, pos.x);
    lua_settable(L, -3);

    lua_pushstring(L, "y");
    lua_pushnumber(L, pos.y);
    lua_settable(L, -3);

    return 1;
}

int LuaGetMouseDelta(lua_State* L)
{
    Vector2 delta = GetMouseDelta();

    lua_newtable(L);

    lua_pushstring(L, "x");
    lua_pushnumber(L, delta.x);
    lua_settable(L, -3);

    lua_pushstring(L, "y");
    lua_pushnumber(L, delta.y);
    lua_settable(L, -3);

    return 1;
}

void LuaOnEndAnimation(LuaScript* script, ecs_entity_t entity, void* level, const char* animation)
{
    lua_State* L = script->L;

    lua_getglobal(L, "Entities");
    if (!lua_istable(L, -1)) {
        lua_pop(L, 1);
        return;
    }

    lua_pushinteger(L, entity);
    lua_gettable(L, -2);
    if (!lua_istable(L, -1)) {
        lua_pop(L, 2);
        return;
    }

    lua_setglobal(L, "self");

    lua_pushlightuserdata(L, level);
    lua_setglobal(L, "level");

    lua_getglobal(L, "OnEndAnimation");
    if (lua_isfunction(L, -1)) {
        lua_pushstring(L, animation);

        if (lua_pcall(L, 1, 0, 0) != LUA_OK) {
            printf("Error al llamar a <OnEndAnimation> en el modulo <%s>\n", script->module);
            lua_pop(L, 1);
        }
    }
    else
    {
        lua_pop(L, 1);
    }

    lua_pop(L, 1);
}

void LuaUpdate(LuaScript* script, ecs_entity_t entity, void *level)
{
  lua_State* L = script->L;

  lua_getglobal(L, "Entities");
  if(!lua_istable(L, -1)){
    lua_pop(L, 1);
    return;
  }

  lua_pushinteger(L, entity);
  lua_gettable(L, -2);
  if(!lua_istable(L, -1)){
    lua_pop(L, 2);
    return;
  }

  lua_setglobal(L, "self");

  lua_pushlightuserdata(L, level);
  lua_setglobal(L, "level");

  lua_getglobal(L, "OnUpdate");
  if(lua_isfunction(L, -1)){

    float dt = GetFrameTime();
    lua_pushnumber(L, dt);

    if(lua_pcall(L, 1, 0, 0) != LUA_OK){
      printf("Error en <OnUpdate> del modulo <%s>: %s\n", script->module, lua_tostring(L, -1));
      lua_pop(L, 1);
    }
  }else{
    lua_pop(L, 1);
  }

  lua_pop(L, 1);
}

void LuaOnCollision(LuaScript* script, ecs_entity_t entity, ecs_entity_t other, void* level)
{
    lua_State* L = script->L;

    lua_getglobal(L, "Entities");
    if (!lua_istable(L, -1)) {
        lua_pop(L, 1);
        return;
    }

    lua_pushinteger(L, entity);
    lua_gettable(L, -2);
    if (!lua_istable(L, -1)) {
        lua_pop(L, 2);
        return;
    }

    lua_setglobal(L, "self");
    lua_pushlightuserdata(L, level);
    lua_setglobal(L, "level");

    lua_getglobal(L, "OnCollision");
    if (lua_isfunction(L, -1)) {
        lua_pushnumber(L, other);
        if (lua_pcall(L, 1, 0, 0) != LUA_OK) {
            lua_pop(L, 1);
        }
    }
    else {
        lua_pop(L, 1);
    }

    lua_pop(L, 1);
}

void LuaOnDestroy(LuaScript* script, ecs_entity_t entity, void* level)
{
    lua_State* L = script->L;

    lua_getglobal(L, "Entities");
    if (!lua_istable(L, -1)) {
        lua_pop(L, 1);
        return;
    }

    lua_pushinteger(L, entity);
    lua_gettable(L, -2);
    if (!lua_istable(L, -1)) {
        lua_pop(L, 2);
        return;
    }

    lua_setglobal(L, "self");
    lua_pushlightuserdata(L, level);
    lua_setglobal(L, "level");

    lua_getglobal(L, "OnDestroy");
    if (lua_isfunction(L, -1)) {
        if (lua_pcall(L, 0, 0, 0) != LUA_OK) {
            lua_pop(L, 1);
        }
    }
    else {
        lua_pop(L, 1);
    }

    lua_pop(L, 1);
}
