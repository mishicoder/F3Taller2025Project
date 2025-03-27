#include <raylib.h>
#include "game.h"

void LoadResources();
void LoadTestLevel(Game* game, Level* level);

int main()
{
    GameConfig config = {
        .targetFPS = 60,
        .windowTitle = "My Game Window",
        .windowWidth = 800,
        .windowHeight = 600,
        .windowColor = (Color){33, 33, 33, 255},
        .activeDebug = false,
        .globalScale = 1.0f,
        .useEscapeToExit = true,
        .windowFullscreen = false
    };

    InitGame(config, LoadResources);

    PushLevel("MyLevel", false, false, false, LoadTestLevel);
  
    RunGame();
    GameDestroy();

    return 0;
}

void LoadTestLevel(Game* game, Level* level)
{
    ecs_entity_t camera = Create2DEntity(level, "camera", "camera");
    AddComponent(level, camera, CAMERA_2D_ID, "main=true");
    AddComponent(level, camera, LUA_SCRIPT_ID, "module=scripts/camera.lua");


    ecs_entity_t mySprite = Create2DEntity(level, "player", "player");
    AddComponent(level, mySprite, SPRITE_RENDERER_ID, "sprite=player,opacity=1.0");
    AddComponent(level, mySprite, ANIMATION_CONTROLLER_ID, "sprite=player");
    AddComponent(level, mySprite, DYEING_ID, "red=1.0");
    AddComponent(level, mySprite, LUA_SCRIPT_ID, "module=scripts/player.lua");

    ecs_entity_t hair = AddEntity2DChild(level, mySprite, "hair", "hair");
    AddComponent(level, hair, SPRITE_RENDERER_ID, "sprite=long_hair");
    AddComponent(level, hair, ANIMATION_CONTROLLER_ID, "sprite=long_hair");

    ecs_entity_t tools = AddEntity2DChild(level, mySprite, "tools", "tools");
    AddComponent(level, tools, SPRITE_RENDERER_ID, "sprite=tools");
    AddComponent(level, tools, ANIMATION_CONTROLLER_ID, "sprite=tools");
}

void LoadResources()
{
    LoadCustomCursor("default", "assets/ui/default.png", 2.0f, 2.0f);
    SetCustomCursor("default");

    LoadSprite("assets/industrial_assets.png", "test");
    LoadSpriteWithOptions("assets/sprites/player/player.png", "assets/sprites/player/player.sprite");
    LoadSpriteWithOptions("assets/sprites/player/longhair.png", "assets/sprites/player/longhair.sprite");
    LoadSpriteWithOptions("assets/sprites/player/tools.png", "assets/sprites/player/tools.sprite");
    LoadSpriteAtlas("assets/sprites/ui/expresions.png", "assets/sprites/ui/expresion.atlas");
}
