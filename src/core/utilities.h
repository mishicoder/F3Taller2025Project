#pragma once
#ifndef UTILITIES_H
#define UTILITIES_H

#include <stdio.h>
#include <math.h>
#include <raylib.h>
#include "components.h"
#include "gametypes.h"

#define MAX_NODES 1000
#define MAX_ITERATIONS 2000

/**
* Path Finding
* 
*/

typedef struct PFNode {
    int x, y;
    int gcost, hcost, fcost;
    struct PFNode* parent;
}PFNode;

int Heuristic(int x1, int y1, int x2, int y2);
PFNode* FindPath(Vector2 start, Vector2 end, C_MapController* controller);
void FreePath(PFNode* node);

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
* 
*/
int IntersectionCircleRectTransformImplementation(C_Transform* transform, Rectangle rect, Circle circle, int isSolid);

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

// 9-slice
/*
#include "raylib.h"

int main(void)
{
    // Inicializar ventana
    const int screenWidth = 800;
    const int screenHeight = 600;
    InitWindow(screenWidth, screenHeight, "9-Slice con NPatch");

    // Cargar imagen
    Texture2D texture = LoadTexture("panel.png");

    // Definir el N-Patch (bordes que no se estiran)
    int border = 16;  // Ajusta seg�n la imagen
    NPatchInfo patch = {
        (Rectangle){ 0, 0, texture.width, texture.height },  // Regi�n de la imagen completa
        border, border, border, border,  // M�rgenes (left, top, right, bottom)
        NPATCH_NINE_PATCH  // Tipo de parche (9-Slice)
    };

    // �rea donde se dibujar� la imagen
    Rectangle dest = { 200, 150, 400, 300 };

    // Bucle principal
    while (!WindowShouldClose())
    {
        BeginDrawing();
        ClearBackground(RAYWHITE);

        // Dibujar el N-Patch
        DrawTextureNPatch(texture, patch, dest, (Vector2){ 0, 0 }, 0.0f, WHITE);

        DrawText("9-Slice con NPatch", 10, 10, 20, DARKGRAY);
        EndDrawing();
    }

    // Liberar recursos
    UnloadTexture(texture);
    CloseWindow();

    return 0;
}
*/

#endif // !UTILITIES_H
