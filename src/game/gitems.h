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

/*
Para conseguir una oveja se necesita de un granero de nivel 2
El granero nivel 1 solo puede tener vacas
Las vacas producen leche por día
La leche tiene la siguiente tabla de precios:
	- 120 -> pequeña
		- calidad base -> 120
		- calidad 1 -> 150
		- calidad 2 -> 180
		- calidad 3 -> 275 (más difícil de conseguir)
	- 180 -> grande
		- calidad base -> 180
		- calidad 1 -> 210
		- calidad 2 -> 240
		- calidad 3 -> 380
El granero para ser construido necesita:
	- 400 madera
	- 250 piedra
	- 6500 monedas
La vaca tuene un valor de 1750
*/