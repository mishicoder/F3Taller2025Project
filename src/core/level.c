#include "level.h"

ecs_entity_t ecs_id(C_Info);

GameLevel* CreateLevel(const char* name, unsigned int keepInMemory,
	unsigned int renderInStack,
	unsigned int updateInStack,
	void(*Load)(struct Game* game, struct GameLevel* level)
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
	level->Load = Load;

	return level;
}

void UnloadLevel(struct Game* game, GameLevel* level)
{
	if (level == NULL) return;
	ECS_COMPONENT(level->world, C_Behaviour);

	ecs_query_t* query = ecs_query_init(level->world, &(ecs_query_desc_t){
		.terms = { {.id = ecs_id(C_Behaviour)}}
	});
	ecs_iter_t it = ecs_query_iter(level->world, query);
	while (ecs_iter_next(&it))
	{
		for (int i = 0; i < it.count; i++)
		{
			ecs_entity_t ent = it.entities[i];
			C_Behaviour* comp = ecs_get(level->world, ent, C_Behaviour);
			if (comp != NULL)
			{
				if (comp->OnUnloadDataHandler != NULL)
				{
					comp->OnUnloadDataHandler(game, level, ent);
				}
			}
		}
	}

	//free(level->name);
	//level->name = NULL;
	ecs_fini(level->world);
	level->world = NULL;
	free(level);
}

ecs_entity_t GetEntityByTag(GameLevel* level, const char* tag)
{
	ecs_query_t* query = ecs_query_init(level->world, &(ecs_query_desc_t){
		.terms = { {.id = tag } }
	});

	ecs_iter_t it = ecs_query_iter(level->world, query);
	while (ecs_query_next(&it))
	{
		for (int i = 0; i < it.count; i++)
		{
			return it.entities[i];
		}
	}

	ecs_query_fini(query);
}

void ForEntityWithTag(struct Game* game, GameLevel* level, const char* tag, void(*func)(struct Game* game, GameLevel* level, ecs_entity_t ent))
{
	ecs_query_t* query = ecs_query_init(level->world, &(ecs_query_desc_t){
		.terms = {{ .id = tag }}
	});

	ecs_iter_t it = ecs_query_iter(level->world, query);
	while (ecs_query_next(&it))
	{
		for (int i = 0; i < it.count; i++)
		{
			func(game, level, it.entities[i]);
		}
	}

	ecs_query_fini(query);
}
