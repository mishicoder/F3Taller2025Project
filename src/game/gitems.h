#pragma once
#ifndef GITEMS_H
#define GITEMS_H

#define MAX_ITEMS 44

/*
typedef struct ShopAnimal
{
	// nombre del animal
	const char* name;
	// 
	int cost;
	int buildRequired;
	int buildLevelRequired;
} ShopAnimal;
*/

typedef struct GItem
{
	const char* sprite;
	int maxStack;
	int minQuality;
	int maxQuality;
	int canBeSold;
	int buyFor;
	int sellFor;
	int retailSellFor;
} GItem;

extern const GItem items[MAX_ITEMS];

#endif // !GITEMS_H
