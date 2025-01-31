#include "test_level.h"

float rot = 0.0f;
int scaleDir = 1;

void TL_Run(Game* game, GameLevel* level)
{
	ECS_COMPONENT(level->world, C_Camera2D);
	ECS_COMPONENT(level->world, C_Info);
	ECS_COMPONENT(level->world, C_RenderLayer);
	ECS_COMPONENT(level->world, C_Transfom);
	ECS_COMPONENT(level->world, C_SpriteRender);
	ECS_COMPONENT(level->world, C_Color);
	ECS_COMPONENT(level->world, C_MapRender);
	ECS_COMPONENT(level->world, C_RectCollider);
	ECS_COMPONENT(level->world, C_DayCicle);
	ECS_COMPONENT(level->world, C_Collector);
	ECS_COMPONENT(level->world, C_Dialog);
	ECS_COMPONENT(level->world, C_Inventory);
	ECS_COMPONENT(level->world, C_HotBar);
	ECS_COMPONENT(level->world, C_Movement);
	ECS_COMPONENT(level->world, C_Action);
	ECS_COMPONENT(level->world, C_PlayerStats);
	ECS_COMPONENT(level->world, C_WorldItem);
	ECS_COMPONENT(level->world, C_Build);
	ECS_COMPONENT(level->world, C_Builder);
	ECS_COMPONENT(level->world, C_Trader);
	ECS_COMPONENT(level->world, C_DropTable);
	ECS_COMPONENT(level->world, C_FarmLand);
	ECS_COMPONENT(level->world, C_Crop);
	ECS_COMPONENT(level->world, C_Tree);

	if (level->isComponentsAdded == 0)
	{	
		LoadSprite(game, "assets/window_icon.png", "test_sprite");
		printf("Assets: %d\n", game->resourcesManager.textureCount);

		ecs_entity_t ent = ecs_entity(level->world, { .name = "player"});
		ecs_add(level->world, ent, C_Info);
		ecs_add(level->world, ent, C_SpriteRender);
		ecs_add(level->world, ent, C_Transfom);
		ecs_set(level->world, ent, C_Transfom, {20.0f, 20.0f, 1.0f, 1.0f, 0.0f});
		ecs_set(level->world, ent, C_Info, { "ent", "test" });
		ecs_set(level->world, ent, C_SpriteRender, {"test_sprite", 1, 1.0f});

		ecs_entity_t ent2 = ecs_entity(level->world, { .name = "change"});
		ecs_add(level->world, ent2, C_Info);
		ecs_add(level->world, ent2, C_SpriteRender);
		ecs_add(level->world, ent2, C_Transfom);
		ecs_add(level->world, ent2, C_Color);
		ecs_set(level->world, ent2, C_Transfom, { 220.0f, 120.0f, 3.0f, 3.0f, 0.0f });
		ecs_set(level->world, ent2, C_Info, { "ent2", "test" });
		ecs_set(level->world, ent2, C_SpriteRender, { "test_sprite", 1, 1.0f});
		ecs_set(level->world, ent2, C_Color, {45, 255, 25});

		level->isComponentsAdded = 1;
	}

	if (IsKeyPressed(KEY_SPACE))
	{
		ecs_entity_t ent = ecs_lookup(level->world, "change");
		if (ent != 0)
		{
			if(ecs_has(level->world, ent, C_Color))
				ecs_remove(level->world, ent, C_Color);
		}
	}

	if (IsKeyPressed(KEY_A))
	{
		ecs_entity_t ent = ecs_lookup(level->world, "change");
		if (ent != 0)
		{
			if (!ecs_has(level->world, ent, C_Color))
			{
				ecs_add(level->world, ent, C_Color);
				ecs_set(level->world, ent, C_Color, {25, 68, 201});
			}
		}
	}

	ecs_query_t* query = ecs_query_init(level->world, &(ecs_query_desc_t){
		.terms = {{ .id = ecs_id(C_SpriteRender)}}
	});

	rot += 120.0f * GetFrameTime();

	ecs_iter_t it = ecs_query_iter(level->world, query);
	while (ecs_iter_next(&it))
	{
		for (int i = 0; i < it.count; i++)
		{
			ecs_entity_t entity = it.entities[i];

			C_SpriteRender* spriteRender = ecs_get(level->world, entity, C_SpriteRender);
			C_Transfom* transform = ecs_get(level->world, entity, C_Transfom);
			C_Color* color = ecs_get(level->world, entity, C_Color);

			if (spriteRender && transform)
			{
				//transform->rotation = rot;
				//transform->positionX += 50.0f * GetFrameTime();

				// scale
				//transform->scaleX += (1.0f * scaleDir) * GetFrameTime();
				//transform->scaleY += (1.0f * scaleDir) * GetFrameTime();
				if (transform->scaleX >= 3.0f)
					scaleDir = -1;
				else if (transform->scaleX <= 1.0f)
					scaleDir = 1;

				Sprite* sprite = GetSprite(&game->resourcesManager, spriteRender->spriteName);
				DrawTexturePro(
					game->resourcesManager.textures[sprite->textureIndex],
					(Rectangle){ sprite->x, sprite->y, sprite->width, sprite->height },
					(Rectangle){ 
						transform->positionX + (sprite->origin.x),
						transform->positionY + (sprite->origin.y),
						sprite->width * transform->scaleX, 
						sprite->height * transform->scaleY
					},
					(Vector2){
						sprite->origin.x * transform->scaleX,
						sprite->origin.y * transform->scaleY
					},
					transform->rotation,
					color ? (Color){color->r, color->g, color->b, 255} : WHITE
				);
				DrawCircle(transform->positionX + sprite->origin.x, transform->positionY + sprite->origin.y, 4, WHITE);
			}
		}
	}

	ecs_query_fini(query);
}