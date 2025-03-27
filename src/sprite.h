#pragma once
#ifndef SPRITE_H
#define SPRITE_H


#include <stdio.h>
#include <stdlib.h>
#include <raylib.h>
#define _GNU_SOURCE
#include <string.h>

/** 
 * @brief Maneja los datos de animación para un sprite.
*/
typedef struct 
{
  char* name; /**< Nombre de la animación. */
  int fIndex; /**< Indice del frame de inicial de la animación. */
  int tIndex; /**< Indice del frame final del animación. */
  bool loop; /**< Indica si la animación se repite infinas veces. */
  int speed; /**< Indica la velocidad de la animación. */
} ResAnimation;

typedef struct
{
  int x;
  int y;
  int width;
  int height;
} ResFrame;

typedef struct
{
  char* name;
  int textureIndex;
  int x;
  int y;
  int width;
  int height;

  Vector2 origin;

  int framesCount;
  int nextFrame;
  ResFrame** frames;

  int animCount;
  ResAnimation** animations;
} ResSprite;

typedef struct SpriteNode
{
  ResSprite* sprite;
  struct SpriteNode* left;
  struct SpriteNode* right;
} SpriteNode;

/**
 * @brief Crea un nuevo sprite y lo almacena en memoria.
 *
 * @param name Nombre del sprite.
 * @param textureIndex Indice de la textura a la que esta enlazada el sprite.
 * @param x Posicion en x del sprite en la textura.
 * @param y Posicion en y del sprite en la textura.
 * @param width Ancho del sprite en la textura.
 * @param height Alto del sprite en la textura.
 * @param origin Punto de pivote del sprite.
 * @param framesCount Cantidad de frames que posee el sprite.
 *
 * @return Retorna el nuevo sprite creado <ResSprite>, caso contrario, retorna NULL.
*/
ResSprite* CreateSprite(const char* name, int textureIndex, int x, int y, int width, int height, Vector2 origin, int framesCount);

/**
 * @brief Agrega un nuevo frame al sprite.
 *
 * @param sprite Sprite al que se le agregará el frame.
 * @param x Posicion en el eje x del frame respecto a la textura.
 * @param y Posicion en el eje y del frame respecto a la textura.
 * @param width Ancho del frame.
 * @param height Alto del frame.
 *
 * @warning Si ya se alcanzó el tamaño máximo de frames, no se podrán agregar más.
 *
 * @return Retorna true si el frame se agrega correctamente, caso contrario, retorna false.
*/
bool AddFrameToSprite(ResSprite* sprite, int x, int y, int width, int height);

/**
 * @brief Agrega una animacion al sprite.
 *
 * @param sprite Sprite al que se le agregará la animación.
 * @param name Nombre de la animación.
 * @param fIndex Índice inicial de la animación.
 * @param tIndex Índice final de la animación.
 * @param loop Indica si la animación se repite.
 * @param speed Velocidad de la animación.
 *
 * @note fIndex no puede ser menor que cero y tIndex no puede ser mayor o igual a la cantidad de frames cargados.
 *
 * @return Retorna true si la animación se agrega correctamente, caso contrario, retorna false.
*/
bool AddAnimationToSprite(ResSprite* sprite, const char* name, int fIndex, int tIndex, bool loop, int speed);

/**
 * @brief Agrega un nuevo nodo al árbol de sprites.
 *
 * @param node Nodo al que se enlazará el nuevo nodo creado.
 * @param sprite Referencia al sprite que contendrá el nuevo nodo.
 *
 * @return Retorna <SpriteNode*> si se logra agregar el nodo, caso contrario, retorna NULL.
*/
SpriteNode* InsertSpriteNode(SpriteNode* node, ResSprite* sprite);

/**
 * @brief Busca un sprite en el árbol de sprites.
 *
 * @param node Nodo desde el cuál se iniciará la búsqueda.
 * @param name Nombre del sprite que se quiere obtener.
 *
 * @return Retorna <ResSprite> si se encuentra en el árbol, caso contrario, retorna NULL.
*/
ResSprite* SearchSpriteInTree(SpriteNode* node, const char* name);

/**
 * @brief Elimina de memoria un nodo del árbol de sprites.
 *
 * @param node Nodo a liberar de memoria.
*/
void UnloadSpriteNode(SpriteNode* node);

#endif //!SPRITE_H
