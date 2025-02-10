#include "utilities.h"

float GClamp(float value, float min, float max)
{
	return (value < min) ? min : (value > max) ? max : value;
}

int ResolveRectangleCircleCollision(Rectangle* rect, Circle circle, unsigned int isSolid)
{
	// 1. Encontrar el punto m�s cercano del rect�ngulo al c�rculo
	float closestX = GClamp(circle.centerX, rect->x, rect->x + rect->width);
	float closestY = GClamp(circle.centerY, rect->y, rect->y + rect->height);

	// 2. Calcular la distancia entre el centro del c�rculo y ese punto
	float deltaX = circle.centerX - closestX;
	float deltaY = circle.centerY - closestY;
	float distanceSquared = deltaX * deltaX + deltaY * deltaY;
	float radiusSquared = circle.radius * circle.radius;

	// 3. Si hay colisi�n (distancia menor que el radio del c�rculo)
	if (distanceSquared < radiusSquared) {
		if (isSolid == 0) return 1;

		float distance = sqrt(distanceSquared);

		// Si la distancia es 0 (el rect�ngulo est� en el centro del c�rculo), moverlo arbitrariamente
		if (distance == 0) {
			deltaX = 1;
			deltaY = 0;
			distance = 1;
		}

		// 4. Calcular cu�nto debe moverse el rect�ngulo
		float overlap = circle.radius - distance;
		float normX = deltaX / distance;
		float normY = deltaY / distance;

		// 5. Mover el rect�ngulo fuera del c�rculo en la direcci�n opuesta
		rect->x -= normX * overlap;
		rect->y -= normY * overlap;

		return 1;
	}

	return 0;
}

void GetClosestPoint(Rectangle rect, Circle circle, float* closestX, float* closestY)
{
	*closestX = fmaxf(rect.x, fminf(circle.centerX, rect.x + rect.width));
	*closestY = fmaxf(rect.y, fminf(circle.centerY, rect.y + rect.height));
}

int IntersectionCircleRectCollisionImplementation(Rectangle* rect, Circle circle)
{
	float closestX, closestY;
	GetClosestPoint(*rect, circle, &closestX, &closestY);

	float toCircleX = closestX - circle.centerX;
	float toCircleY = closestY - circle.centerY;
	float distSq = toCircleX * toCircleX + toCircleY * toCircleY;
	float radiusSq = circle.radius * circle.radius;

	if (distSq < radiusSq) { // Hay colisi�n
		float dist = sqrtf(distSq);
		if (dist == 0) return 0; // Evita divisiones por cero

		float normalX = toCircleX / dist;
		float normalY = toCircleY / dist;
		float overlap = circle.radius - dist;

		// Mueve el rect�ngulo hacia afuera del c�rculo
		rect->x += normalX * overlap;
		rect->y += normalY * overlap;
	}

	return 1;
}

int IntersectionCircleRectTransformImplementation(C_Transform* transform, Rectangle rect, Circle circle, int isSolid)
{
	float closestX, closestY;
	GetClosestPoint(rect, circle, &closestX, &closestY);

	float toCircleX = closestX - circle.centerX;
	float toCircleY = closestY - circle.centerY;
	float distSq = toCircleX * toCircleX + toCircleY * toCircleY;
	float radiusSq = circle.radius * circle.radius;

	if (distSq < radiusSq) { // Hay colisi�n
		if (isSolid == 0) return 1;

		float dist = sqrtf(distSq);
		if (dist == 0) return 0; // Evita divisiones por cero

		float normalX = toCircleX / dist;
		float normalY = toCircleY / dist;
		float overlap = circle.radius - dist;
		
		transform->positionX += normalX * overlap;
		transform->positionY += normalY * overlap;

		return 1;
	}

	return 0;
}

int GetCollisionRectangleRectangle(Rectangle a, Rectangle b, Rectangle* result)
{
	float intersectX = (a.x > b.x) ? a.x : b.x;
	float intersectY = (a.y > b.y) ? a.y : b.y;
	float intersectWidth = ((a.x + a.width) < (b.x + b.width)) ? (a.x + a.width) - intersectX : (b.x + b.width) - intersectX;
	float intersectHeight = ((a.y + a.height) < (b.y + b.height)) ? (a.y + a.height) - intersectY : (b.y + b.height) - intersectY;

	// Si hay intersecci�n v�lida
	if (intersectWidth > 0 && intersectHeight > 0) {
		result->x = intersectX;
		result->y = intersectY;
		result->width = intersectWidth;
		result->height = intersectHeight;
		return 1; // Hay colisi�n
	}
	return 0; // No hay colisi�n
}

int ResolveRectRectCollision(Rectangle* a, Rectangle b, unsigned int isSolid)
{
	Rectangle collisionRect;

	if (GetCollisionRectangleRectangle(*a, b, &collisionRect)) {
		if (isSolid == 0) return 1;

		float overlapX = (a->x + a->width) - b.x;
		float overlapY = (a->y + a->height) - b.y;

		if (a->x > b.x) overlapX = (b.x + b.width) - a->x;
		if (a->y > b.y) overlapY = (b.y + b.height) - a->y;

		// Determinar la menor superposici�n y mover `a` en consecuencia
		if (overlapX < overlapY) {
			if (a->x < b.x) {
				a->x -= overlapX; // Mover a la izquierda
			}
			else {
				a->x += overlapX; // Mover a la derecha
			}
		}
		else {
			if (a->y < b.y) {
				a->y -= overlapY; // Mover arriba
			}
			else {
				a->y += overlapY; // Mover abajo
			}
		}

		return 1;
	}

	return 0;
}

int IntersectionRectangleRectangleCollisionImplementation(Rectangle* a, Rectangle b)
{
	if (a->x < b.x + b.width &&
		a->x + a->width > b.x &&
		a->y < b.y + b.height &&
		a->y + a->height > b.y)
	{
		float overlapLeft = (a->x + a->width) - b.x;
		float overlapRight = (b.x + b.width) - a->x;
		float overlapTop = (a->y + a->height) - b.y;
		float overlapBottom = (b.y + b.height) - a->y;

		float minOverlap = fminf(fminf(overlapLeft, overlapRight), fminf(overlapTop, overlapBottom));

		if (minOverlap == overlapLeft)
			a->x -= overlapLeft;
		else if (minOverlap == overlapRight)
			a->x += overlapRight;
		else if (minOverlap == overlapTop)
			a->y -= overlapTop;
		else if (minOverlap == overlapBottom)
			a->y += overlapBottom;

		return 1;
	}

	return 0;
}
