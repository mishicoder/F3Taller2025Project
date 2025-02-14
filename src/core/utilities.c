#include "utilities.h"

int Heuristic(int x1, int y1, int x2, int y2)
{
	return abs(x1 - x2) + abs(y1 - y2);
}

PFNode* FindPath(Vector2 start, Vector2 end, C_MapController* controller)
{
	PFNode* openList[MAX_NODES];
	PFNode* closedList[MAX_NODES];
	int openCount = 0, closedCount = 0;

	PFNode* startNode = malloc(sizeof(PFNode));
	if (startNode == NULL) return NULL;
	startNode->x = start.x;
	startNode->y = start.y;
	startNode->gcost = 0;
	startNode->hcost = Heuristic(start.x, start.y, end.x, end.y);
	startNode->fcost = startNode->hcost;
	startNode->parent = NULL;

	openList[openCount++] = startNode;

	int directions[4][2] = { {0, -1}, {0, 1}, {-1, 0}, {1.0} };

	int iterations = 0;
	while (openCount > 0)
	{
		if (iterations > MAX_ITERATIONS)
			return NULL;

		int lowestIndex = 0;
		for (int i = 1; i < openCount; i++)
		{
			if (openList[i]->fcost < openList[lowestIndex]->fcost) {
				lowestIndex = i;
			}
		}

		PFNode* current = openList[lowestIndex];

		if (current->x == end.x && current->y == end.y)
		{
			return current;
		}

		closedList[closedCount++] = current;
		openList[lowestIndex] = openList[--openCount];

		for (int i = 0; i < 4; i++)
		{
			int nx = current->x + directions[i][0];
			int ny = current->y + directions[i][1];

			if (nx < 0 || ny < 0 || nx >= controller->gridCols || ny >= controller->gridRows)
				continue;

			PFNode* neighbor = malloc(sizeof(PFNode));
			if (neighbor == NULL)
				continue;
			neighbor->x = nx;
			neighbor->y = ny;
			neighbor->gcost = current->gcost + 1;
			neighbor->hcost = Heuristic(nx, ny, end.x, end.y);
			neighbor->fcost = neighbor ->gcost + neighbor->hcost;
			neighbor->parent = current;

			openList[openCount++] = neighbor;
		}
	}

	return NULL;
}

void FreePath(PFNode* node)
{
	while (node)
	{
		PFNode* temp = node;
		node = node->parent;
		if(temp != NULL)
			free(temp);
	}
}

float GClamp(float value, float min, float max)
{
	return (value < min) ? min : (value > max) ? max : value;
}

int ResolveRectangleCircleCollision(Rectangle* rect, Circle circle, unsigned int isSolid)
{
	// 1. Encontrar el punto más cercano del rectángulo al círculo
	float closestX = GClamp(circle.centerX, rect->x, rect->x + rect->width);
	float closestY = GClamp(circle.centerY, rect->y, rect->y + rect->height);

	// 2. Calcular la distancia entre el centro del círculo y ese punto
	float deltaX = circle.centerX - closestX;
	float deltaY = circle.centerY - closestY;
	float distanceSquared = deltaX * deltaX + deltaY * deltaY;
	float radiusSquared = circle.radius * circle.radius;

	// 3. Si hay colisión (distancia menor que el radio del círculo)
	if (distanceSquared < radiusSquared) {
		if (isSolid == 0) return 1;

		float distance = sqrt(distanceSquared);

		// Si la distancia es 0 (el rectángulo está en el centro del círculo), moverlo arbitrariamente
		if (distance == 0) {
			deltaX = 1;
			deltaY = 0;
			distance = 1;
		}

		// 4. Calcular cuánto debe moverse el rectángulo
		float overlap = circle.radius - distance;
		float normX = deltaX / distance;
		float normY = deltaY / distance;

		// 5. Mover el rectángulo fuera del círculo en la dirección opuesta
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

	if (distSq < radiusSq) { // Hay colisión
		float dist = sqrtf(distSq);
		if (dist == 0) return 0; // Evita divisiones por cero

		float normalX = toCircleX / dist;
		float normalY = toCircleY / dist;
		float overlap = circle.radius - dist;

		// Mueve el rectángulo hacia afuera del círculo
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

	if (distSq < radiusSq) { // Hay colisión
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

	// Si hay intersección válida
	if (intersectWidth > 0 && intersectHeight > 0) {
		result->x = intersectX;
		result->y = intersectY;
		result->width = intersectWidth;
		result->height = intersectHeight;
		return 1; // Hay colisión
	}
	return 0; // No hay colisión
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

		// Determinar la menor superposición y mover `a` en consecuencia
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

int IntersectionRectangleRectangleCollisionImplementation(Rectangle* a, Rectangle b, C_Transform* transform, int isSolid, int isStatic)
{
	if (a->x < b.x + b.width &&
		a->x + a->width > b.x &&
		a->y < b.y + b.height &&
		a->y + a->height > b.y){

		if (isSolid == 0) return 1;

		float overlapLeft = (a->x + a->width) - b.x;
		float overlapRight = (b.x + b.width) - a->x;
		float overlapTop = (a->y + a->height) - b.y;
		float overlapBottom = (b.y + b.height) - a->y;

		float minOverlap = fminf(fminf(overlapLeft, overlapRight), fminf(overlapTop, overlapBottom));

		if (minOverlap == overlapLeft)
		{
			//a->x -= overlapLeft;
			if(isStatic == 0)
				transform->positionX -= overlapLeft;
		}
		else if (minOverlap == overlapRight)
		{
			//a->x += overlapRight;
			if (isStatic == 0)
				transform->positionX += overlapRight;
		}
		else if (minOverlap == overlapTop)
		{
			//a->y -= overlapTop;
			if(isStatic == 0)
				transform->positionY -= overlapTop;
		}
		else if (minOverlap == overlapBottom)
		{
			//a->y += overlapBottom;
			if (isStatic == 0)
				transform->positionY += overlapBottom;
		}

		return 1;
	}

	return 0;
}

int IntersectionRectRectTransformImplementation(C_Transform* transform, Rectangle a, Rectangle b, int isSolid)
{
	if (a.x < b.x + b.width &&
		a.x + a.width > b.x &&
		a.y < b.y + b.height &&
		a.y + a.height > b.y)
	{
		if (isSolid == 0)
			return 1;

		// Calculamos cuánto se está superponiendo en cada dirección
		float overlapLeft = (a.x + a.width) - b.x;
		float overlapRight = (b.x + b.width) - a.x;
		float overlapTop = (a.y + a.height) - b.y;
		float overlapBottom = (b.y + b.height) - a.y;

		// Determinamos la dirección de la mínima superposición
		float minOverlap = fminf(fminf(overlapLeft, overlapRight), fminf(overlapTop, overlapBottom));

		// Ajustamos la posición de `a` para que salga de la colisión
		if (minOverlap == overlapLeft)
			transform->positionX -= overlapLeft; // Mueve `a` hacia la izquierda
		else if (minOverlap == overlapRight)
			transform->positionX += overlapRight; // Mueve `a` hacia la derecha
		else if (minOverlap == overlapTop)
			transform->positionY -= overlapTop; // Mueve `a` hacia arriba
		else if (minOverlap == overlapBottom)
			transform->positionY += overlapBottom; // Mueve `a` hacia abajo

		return 1;
	}

	return 0;
}
