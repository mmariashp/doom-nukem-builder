
#include "builder.h"

void					draw_items_or_free(char draw_free, int type, t_rec box, SDL_Renderer *rend)
{
	int 				i;
	static t_texture	*items = NULL;
	static char 		path[10] = "./items3/";
	static char 		ext[5] = ".png";
	static char 		name[TOTAL_TYPES][25] = {	"coin", \
													"key", \
													"object", \
													"enemy", \
													"super_bonus", \
													"health", \
													"ammo", \
													"light" };
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
			items[i].sdl_t = load_texture(get_full_path(name[i], ext, path), rend, 0);
			i++;
		}
	}
	if (draw_free == 0 && within(type, -1, TOTAL_TYPES) && items[type].sdl_t)
	{
		if (type == KEY || type == LIGHT || type == HEALTH)
		{
			box.x += box.w * 0.2;
			box.w *= 0.8;
		}
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
	box = (t_rec){ 0, 0, grid->box.w * 0.009, grid->box.w * 0.009 };
	while (i < world->sec[sector].n_items)
	{
		id = world->sec[sector].items[i].id;
		p = world->sec[sector].items[i].p;
		if (within(p.x, -1, GRID_SIZE) && within(p.y, -1, GRID_SIZE) &&
		node_in_sector(p, world) == sector && grid->nodes[p.x][p.y] == (signed char)(-10 - i))
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

int 					find_default_item(int type, t_itemfull *itemfull, int n)
{
	int 				i;

	i = 0;
	if (!itemfull)
		return (-1);
	while (i < n)
	{
		if (itemfull[i].type == type)
			return (i);
		i++;
	}
	return (-1);
}

void					add_item(int default_id, t_vec2d mouse, t_grid *grid, t_sector *sector)
{
	t_vec2d				node;
	int 				i;
	t_item				*new;

	if (!grid || !sector || sector->n_items >= MAX_SECTOR_ITEMS || default_id == -1)
		return ;
	node = find_node(mouse.x, mouse.y, grid);
	if (within(node.x, -1, GRID_SIZE) && within(node.y, -1, GRID_SIZE))
	{
		if (grid->nodes[node.x][node.y] == NODE_EMPTY)
		{
			new = (t_item *)ft_memalloc(sizeof(t_item) * (sector->n_items + 1));
			if (!new)
				return ;
			i = 0;
			while (i < sector->n_items)
			{
				new[i] = sector->items[i];
				i++;
			}
			new[sector->n_items] = (t_item){ node, default_id };
			if (sector->items)
				free(sector->items);
			sector->items = new;
			grid->nodes[node.x][node.y] = (signed char)(-10 - sector->n_items);
			sector->n_items++;
		}
	}
}

void					move_item(t_prog *prog, t_vec2d mouse, t_grid *grid, t_sector *sector)
{
	static int			id = -1;
	static t_vec2d		to_erase = { -1, -1 };


	if (!prog || !grid || !sector)
		return ;
	if (prog->click.x || prog->click.y)
	{
		if (mouse_over(grid->box, mouse))
		{
			if (grid->active[0].x == -1)
			{
				grid->active[0] = find_node(mouse.x, mouse.y, grid);
				if (grid->active[0].x >= 0 && grid->active[0].y >= 0 &&
					grid->nodes[grid->active[0].x][grid->active[0].y] < -9)
					id = (grid->nodes[grid->active[0].x][grid->active[0].y] + 10) * (-1);
				if (!within(id, -1, sector->n_items))
				{
					grid->active[0] = (t_vec2d){ -1, -1 };
					move_grid_drag(prog, mouse, grid);
					id = -1;
				}
				selected_item(0, I_SELECT, id);
				to_erase = grid->active[0];
			}
			else if (id >= 0)
			{
				grid->active[1] = find_node(mouse.x, mouse.y, grid);
				if (grid->nodes[grid->active[1].x][grid->active[1].y] == NODE_EMPTY)
				{
					if (to_erase.x != -1)
						grid->nodes[to_erase.x][to_erase.y] = NODE_EMPTY;
					sector->items[id].p = grid->active[1];
					grid->nodes[grid->active[1].x][grid->active[1].y] = (signed char)(-10 - id);
					to_erase = grid->active[1];
				}
			}
			prog->click = mouse;
			prog->features[F_REDRAW] = 1;
		}
		else
			prog->click = (t_vec2d){ 0, 0 };
	}
	else
	{
		grid->active[0] = (t_vec2d){ -1, -1 };
		id = -1;
	}
}