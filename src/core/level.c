#include "level.h"

ecs_entity_t ecs_id(C_Info);

GameLevel* CreateLevel(const char* name, unsigned int keepInMemory,
	unsigned int renderInStack,
	unsigned int updateInStack,
	void(*Run)(struct Game* game, struct GameLevel* level)
)
{
	GameLevel* level = (GameLevel*)malloc(sizeof(GameLevel));
	if (level == NULL)
	{
		printf("Error al crear el nivel: %s\n", name);
		return NULL;
	}

	level->world = ecs_init();
	if (level->world == NULL)
	{
		printf("Error al crear el mundo para las entidades en el nivel %s\n", name);
		free(level);
		return NULL;
	}

	level->name = _strdup(name);
	if (level->name == NULL)
	{
		printf("Error al asignar memoria para el nombre del nivel %s.\n", name);
		free(level);
		return NULL;
	}

	level->keepInMemory = keepInMemory;
	level->renderInStack = renderInStack;
	level->updateInStack = updateInStack;
	level->isComponentsAdded = 0;
	level->Run = Run;

	return level;
}

void UnloadLevel(GameLevel* level)
{
	if (level == NULL) return;

	free(level->name);
	level->name = NULL;
	ecs_fini(level->world);
	free(level);
}

ecs_entity_t GetEntityByTag(GameLevel* level, const char* tag)
{
	return 0;
}

void ForEntityWithTag(GameLevel* level, const char* tag, void(*func)(GameLevel* level, ecs_entity_t ent))
{
	/*ecs_query_t* query = ecs_query_init(level->world, &level->desc);
	ecs_iter_t it = ecs_query_iter(level->world, query);
	//ECS_COMPONENT(level->world, C_Info);

	while (ecs_iter_next(&it))
	{
		for (int i = 0; i < it.count; i++)
		{
			ecs_entity_t ent = it.entities[i];
			
			ecs_entity_t info_e = ecs_id(C_Info);
			C_Info* info = ecs_get(level->world, ent, C_Info);

			if (info[i].tag && strcmp(info[i].tag, tag) == 0)
			{
				func(level, it.entities[i]);
			}
		}
	}

	ecs_query_fini(query);*/
}
