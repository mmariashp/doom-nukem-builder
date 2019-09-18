
#include "builder.h"

void					draw_items_or_free(char draw_free, int type, t_rec box, SDL_Renderer *rend)
{
	int 				i;
	static t_texture	*items = NULL;
	static char 		path[TOTAL_TYPES][25] = {	"./items/coin.png", \
													"./items/key.png", \
													"./items/object.png", \
													"./items/enemy.png", \
													"./items/super_bonus.png", \
													"./items/health.png", \
													"./items/ammo.png" };

	if (!rend)
		return ;
	if (!items)
	{
		if (!(items = (t_texture *)ft_memalloc(sizeof(t_texture) * TOTAL_TYPES)))
			return ;
		ft_bzero(items, sizeof(t_texture) * TOTAL_TYPES);
		i = 0;
		while (i < TOTAL_TYPES)
		{
			items[i].name = NULL;
			items[i].full_path = NULL;
			items[i].sdl_t = load_texture(path[i], rend, 0);
			i++;
		}
	}
	if (draw_free == 0 && within(type, -1, TOTAL_TYPES) && items[type].sdl_t)
	{
		if (type == AMMO)
		{
			box.y += box.h / 4;
			box.h /= 2;
		}
//		if (type == ENEMY)
//		{
//			box.y -= box.h * 0.5;
//			box.h *= 1.6;
//		}
		render_box(box, items[type].sdl_t, rend);
	}
	else if (draw_free == 1 && items)
	{
		i = 0;
		while (i < TOTAL_TYPES)
		{
			if (items[i].sdl_t)
				SDL_DestroyTexture(items[i].sdl_t);
			items[i].sdl_t = NULL;
			i++;
		}
		free(items);
		items = NULL;
	}
}

int 					node_in_sector(t_vec2d grid_p, t_world *world, t_grid *grid)
{
	int 				id;
	t_vec2d				*v;
	int 				i;
	int 				j;

	if (!world || !grid)
		return (-1);
	id = -1;
	i = 0;
	while (i < world->n_sec)
	{
		if ( world->sec[i].status != SEC_OPEN)
		{
			if (!(v = ft_memalloc(sizeof(t_vec2d) * world->sec[i].n_v)))
				return (-1);
			j = 0;
			while (j < world->sec[i].n_v)
			{
				if (world->sec[i].v[j] < 0 || world->sec[i].v[j] >= world->n_vectors)
					return (-1);
				v[j] = world->vertices[world->sec[i].v[j]];
				j++;
			}
			if (dot_inside_sector(grid_p.x, grid_p.y, v, world->sec[i].n_v) == TRUE)
				id = i;
			free(v);
		}
		i++;
	}
	return (id);
}

void					render_items(SDL_Renderer *rend, t_world *world, t_itemfull *itemfull, int n, t_grid *grid)
{
	int 				sector;
	int 				i;
	int 				id;
	t_vec2d				p;
	t_rec				box;

	if (!rend || !world || !itemfull)
		return ;
	sector = selected_item(1, S_SELECT, -1);
	if (!within(sector, -1, world->n_sec) || world->sec[sector].n_items < 1 || !world->sec[sector].items)
		return ;
	i = 0;
	box = (t_rec){ 0, 0, grid->box.w * 0.008, grid->box.w * 0.008 };
	while (i < world->sec[sector].n_items)
	{
		id = world->sec[sector].items[i].id;
		p = world->sec[sector].items[i].p;
		if (within(p.x, -1, GRID_SIZE) && within(p.y, -1, GRID_SIZE) &&
		node_in_sector(p, world, grid) == sector && grid->nodes[p.x][p.y] == (signed char)(-10 - i))
		{

//			grid->nodes[p.x][p.y] = (signed char)(-10 - i);
//			printf("i is %d, making node = %d\n", i, grid->nodes[p.x][p.y]);
			p.x = (int)(grid->box.x + (world->sec[sector].items[i].p.x * grid->scale));
			p.y = (int)(grid->box.y + (world->sec[sector].items[i].p.y * grid->scale));
//			printf("%d, %d ; %d, %d\n", world->sec[sector].items[i].p.x, world->sec[sector].items[i].p.y, p.x, p.y);
			if (within(id, -1, n))
			{
				box.x = p.x - box.w / 2;
				box.y = p.y - box.h / 2;
				draw_items_or_free(0, itemfull[id].type, box, rend);
			}
		}
		else
			delete_item(&world->sec[sector], i--);

		i++;
	}
}