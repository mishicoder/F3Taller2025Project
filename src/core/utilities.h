#pragma once
#ifndef UTILITIES_H
#define UTILITIES_H

#include <stdio.h>
#include <math.h>
#include <raylib.h>
#include "gametypes.h"

/**
* Función para obtener limitar un valor entre un mínimo y un máximo.
*
* @param[in] value Valor a limitar.
* @param[in] min Valor mínimo del límite.
* @param[in] max Valor máximo del límite.
*
* @return Retorna un float que es valor válido en el rango dado.
*/
float GClamp(float value, float min, float max);

/**
* Resuelve la colisión entre un rectángulo y un círculo.
*
* @param[in] rect Rectángulo al cuál se le corregirá la posición.
* @param[in] circle Círculo con el que se resolverá la colisión.
*/
int ResolveRectangleCircleCollision(Rectangle* rect, Circle circle, unsigned int isSolid);

/**
* Obtiene el punto más cercano del rectángulo al círculo.
*/
void GetClosestPoint(Rectangle rect, Circle circle, float* closestX, float* closestY);

/**
* Determina la intersección del objeto para evitar el tunelado (intersección).
* 
* @param[in] rect Rectángulo al cuál se le corregirá la posición.
* @param[in] circle Círculo con el que se resolverá la colisión.
* 
* @return Retorna 1 si la colisión se da y se resuelve, caso contrario, retorna 0.
*/
int IntersectionCircleRectCollisionImplementation(Rectangle* rect, Circle circle);

/**
* Función para obtener el rectángulo de intersección entre dos rectángulos.
*
* @param[in]  a Rectángulo de colisión 1.
* @param[in]  b Rectángulo de colisión 2.
* @param[out] result Rectángulo resultante.
*
* @return Retorna 1 existe una intersección, caso contrario, 0.
*/
int GetCollisionRectangleRectangle(Rectangle a, Rectangle b, Rectangle* result);

/**
* Función para resolver la colisión entre dos rectángulos.
*
* @param[in] a Rectángulo al cuál se le corregirá la posición.
* @param[in] b Rectángulo con el que se comprueba la colisión.
*/
int ResolveRectRectCollision(Rectangle* a, Rectangle b, unsigned int isSolid);

/**
* Determina la intersección de un rectángulo en un rectángulo para evitar el tunelado.
* 
* @param[in] a Rectángulo al que se le corrige el tunelado.
* @param[in] b Rectángulo que simula el obstáculo que el objeto no puede atravesar.
* 
* @return Retorna 1 si se da la colisión, caso contrario, retorna 0.
*/
int IntersectionRectangleRectangleCollisionImplementation(Rectangle* a, Rectangle b);

#endif // !UTILITIES_H
