#pragma once
#ifndef UTILITIES_H
#define UTILITIES_H

#include <stdio.h>
#include <math.h>
#include <stdbool.h>
#include <raylib.h>
#include "components.h"

typedef struct
{
  float centerx;
  float centery;
  float radius;
}Circle;

void GetClosestPoint(Rectangle rect, Circle circle, float* closestX, float* closestY);
bool IntersectionCircleRectTransformImplementation(Transform2D* transform, Rectangle rect, Circle circle, bool isSolid);
bool IntersectionRectRectTransformImplementation(Transform2D* transform, Rectangle a, Rectangle b, bool isSolid, bool isStatic);

#endif // !UTILITIES_H
