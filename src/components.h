#pragma once
#ifndef COMPONENTS_H

#include <stdio.h>
#include <stdbool.h>
#include <lua.h>
#include <lualib.h>
#include <luaconf.h>

#define INFO_ID "Info"
#define TRANSFORM_2D_ID "Transform2D"
#define CAMERA_2D_ID "Camera2D"
#define SPRITE_RENDERER_ID "SpriteRender"
#define ANIMATION_CONTROLLER_ID "Animation"
#define RENDER_LAYER_ID "RenderLayer"
#define MAP_RENDER_ID "MapRender"
#define DYEING_ID "Dyeing"
#define LUA_SCRIPT_ID "LScript"
#define PYTHON_SCRIPT_ID "PScript" // futura implementacion
#define RECT_COLLIDER_ID "RectCollider"
#define CIRCLE_COLLIDER_ID "CircleCollider"

/* Brinda informacion de la entidad. */
typedef struct
{
  // Nombre de la entidad.
  char* name;
  // Etiqueta de la entidad.
  char* tag;
} Info;

/* Permite que la entidad sea persistente en 1 o varios niveles */
typedef struct
{
  // Indica si es persistente en todos los niveles
  bool forAll;
  // Indica los nombres de los niveles en los que es persistente (si forAll es true, este campo debe ser NULL)
  char** levels;
  // Contador de los niveles agregados
  int listCount;
} Persistent;

/* Contiene los datos de translación, escalado y posición de una entidad 2D */
typedef struct
{
  // Posicion en x
  float posx;
  // Posicion en y
  float posy;
  // Escale en x
  float scalex;
  // Escala en y
  float scaley;
  // Angulo de rotacion
  float rotation;

  // Posicion x relativa al padre (si no tiene, es 0.0)
  float relx;
  // Posicion y relativa al padre (si no tiene, es 0.0)
  float rely;
  // Escala x relativa al padre (si no tiene, es 0.0)
  float relsx;
  // Escala x relativa al padre (si no tiene, es 0.0)
  float relsy;
} Transform2D;

typedef struct
{
	float posx;
	float posy;
	float posz;

	float scalex;
	float scaley;
	float scalez;

	float rotx;
	float roty;
	float rotz;

	bool isParent;

	float realx;
	float realy;
	float realz;

	float realsx;
	float realsy;
	float realsz;

	float realrotx;
	float realroty;
	float realrotz;
} Transform3D;

/* Permite renderizar un sprite */
typedef struct
{
  char* sprite;
  bool visible;
  float opacity;
  bool flipx;
  bool flipy;
} SpriteRender;

/* Permite gestionar animaciones */
typedef struct
{
  char* sprite;
  char* currentAnimation;
  // callback para la finalización de las animaciones (Son funciones pre-declaras desde lua y enlazadas en la configuración)
  //char* endCallback;
  int currentFrame;
  int frameCounter;
  int fIndex;
  int tIndex;
  bool loop;
  int speed;
} AnimationController;

/* Gestiona la capa de renderizado de la entidad */
typedef struct
{
  int index;
} RenderLayer;

/* Manejar una cámara 2D */
typedef struct
{
  bool isMain;
  float offsetx;
  float offsety;
  float zoom;
} Camera2d;

/* Permite el cambio del color de un elemento gráfico de una entidad */
typedef struct
{
  float r;
  float g;
  float b;
} Dyeing;

/* Permite renderizar un mapa */
typedef struct
{
  char* map;
} MapRender;

/* Control de scripts de la entidad con {Lua} */
typedef struct
{
	char* module;
	lua_State* L;
	bool isValid;
} LuaScript;

/* Control de scripts de la entidad con {Python} */
typedef struct
{
  char* module;
} PythonScript;

/* Colision rectángular */
typedef struct
{
  float posx;
  float posy;
  float offsetx;
  float offsety;
  int width;
  int height;
  bool isSolid;
  bool isStatic;
} RectCollider;

/* Colision circular. */
typedef struct
{
  float posx;
  float posy;
  float offsetx;
  float offsety;
  float radius;
  bool isSolid;
  bool isStatic;
} CircleCollider;

// Physics
typedef struct
{
	unsigned int id;
}RigidBody;

// RigidBody Required
typedef struct PhysicMaterial
{
	unsigned int id;
};

#endif // !COMPONENTS_H
