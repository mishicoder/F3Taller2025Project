#include "utilities.h"

void GetClosestPoint(Rectangle rect, Circle circle, float *closestX, float *closestY)
{
  *closestX = fmaxf(rect.x, fminf(circle.centerx, rect.x + rect.width));
  *closestY = fmaxf(rect.y, fminf(circle.centery, rect.y + rect.height));
}

bool IntersectionCircleRectTransformImplementation(Transform2D *transform, Rectangle rect, Circle circle, bool isSolid)
{
  float closestX, closestY;
  GetClosestPoint(rect, circle, &closestX, &closestY);

  float toCircleX = closestX - circle.centerx;
  float toCircleY = closestY - circle.centery;
  float distSq = toCircleX * toCircleX + toCircleY * toCircleY;
  float radiusSq = circle.radius * circle.radius;

  if(distSq < radiusSq)
  {
    if(isSolid) return true;

    float dist = sqrtf(distSq);
    if(dist == 0) return false;

    float normalX = toCircleX / dist;
    float normalY = toCircleY / dist;
    float overlap = circle.radius - dist;

    transform->posx += normalX * overlap;
    transform->posy += normalY * overlap;

    return true;
  }

  return false;
}

bool IntersectionRectRectTransformImplementation(Transform2D *transform, Rectangle a, Rectangle b, bool isSolid, bool isStatic)
{
  if(a.x < b.x + b.width && 
    a.x + a.width > b.x &&
    a.y < b.y + b.height &&
    a.y + a.height > b.y
  )
  {
    if(isSolid) return true;

    float overlapLeft = (a.x + a.width) - b.x;
    float overlapRight = (b.x + b.width) - a.x;
    float overlapTop = (a.x + a.height) - b.y;
    float overlapBottom = (b.y + b.height) - a.y;

    float minOverlap = fminf(fminf(overlapLeft, overlapRight), fminf(overlapTop, overlapBottom));

    if(minOverlap == overlapLeft)
    {  
      if(!isStatic) transform->posx -= overlapLeft;
    }
    else if(minOverlap == overlapRight)
    {
      if(!isStatic) transform->posx += overlapRight;
    }
    else if(minOverlap == overlapTop)
    {
      if(!isStatic) transform->posy -= overlapTop;
    }
    else if(minOverlap == overlapBottom)
    {
      if(!isStatic) transform->posy += overlapBottom;
    }

    return true;
  }

  return false;
}
