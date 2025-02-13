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
* 
*/
int IntersectionCircleRectTransformImplementation(C_Transform* transform, Rectangle rect, Circle circle, int isSolid);

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
    int border = 16;  // Ajusta según la imagen
    NPatchInfo patch = {
        (Rectangle){ 0, 0, texture.width, texture.height },  // Región de la imagen completa
        border, border, border, border,  // Márgenes (left, top, right, bottom)
        NPATCH_NINE_PATCH  // Tipo de parche (9-Slice)
    };

    // Área donde se dibujará la imagen
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
