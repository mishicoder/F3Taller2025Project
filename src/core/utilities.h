#pragma once
#ifndef UTILITIES_H
#define UTILITIES_H

#include <stdio.h>
#include <math.h>
#include <raylib.h>
#include "gametypes.h"

/**
* Funci�n para obtener limitar un valor entre un m�nimo y un m�ximo.
*
* @param[in] value Valor a limitar.
* @param[in] min Valor m�nimo del l�mite.
* @param[in] max Valor m�ximo del l�mite.
*
* @return Retorna un float que es valor v�lido en el rango dado.
*/
float GClamp(float value, float min, float max);

/**
* Resuelve la colisi�n entre un rect�ngulo y un c�rculo.
*
* @param[in] rect Rect�ngulo al cu�l se le corregir� la posici�n.
* @param[in] circle C�rculo con el que se resolver� la colisi�n.
*/
int ResolveRectangleCircleCollision(Rectangle* rect, Circle circle, unsigned int isSolid);

/**
* Obtiene el punto m�s cercano del rect�ngulo al c�rculo.
*/
void GetClosestPoint(Rectangle rect, Circle circle, float* closestX, float* closestY);

/**
* Determina la intersecci�n del objeto para evitar el tunelado (intersecci�n).
* 
* @param[in] rect Rect�ngulo al cu�l se le corregir� la posici�n.
* @param[in] circle C�rculo con el que se resolver� la colisi�n.
* 
* @return Retorna 1 si la colisi�n se da y se resuelve, caso contrario, retorna 0.
*/
int IntersectionCircleRectCollisionImplementation(Rectangle* rect, Circle circle);

/**
* Funci�n para obtener el rect�ngulo de intersecci�n entre dos rect�ngulos.
*
* @param[in]  a Rect�ngulo de colisi�n 1.
* @param[in]  b Rect�ngulo de colisi�n 2.
* @param[out] result Rect�ngulo resultante.
*
* @return Retorna 1 existe una intersecci�n, caso contrario, 0.
*/
int GetCollisionRectangleRectangle(Rectangle a, Rectangle b, Rectangle* result);

/**
* Funci�n para resolver la colisi�n entre dos rect�ngulos.
*
* @param[in] a Rect�ngulo al cu�l se le corregir� la posici�n.
* @param[in] b Rect�ngulo con el que se comprueba la colisi�n.
*/
int ResolveRectRectCollision(Rectangle* a, Rectangle b, unsigned int isSolid);

/**
* Determina la intersecci�n de un rect�ngulo en un rect�ngulo para evitar el tunelado.
* 
* @param[in] a Rect�ngulo al que se le corrige el tunelado.
* @param[in] b Rect�ngulo que simula el obst�culo que el objeto no puede atravesar.
* 
* @return Retorna 1 si se da la colisi�n, caso contrario, retorna 0.
*/
int IntersectionRectangleRectangleCollisionImplementation(Rectangle* a, Rectangle b);

#endif // !UTILITIES_H
