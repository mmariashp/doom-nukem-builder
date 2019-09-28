
#include "builder.h"

void					draw_item(int type, t_rec box, SDL_Renderer *rend, SDL_Texture **t)
{
	int 				txtr;

	if (!rend || !t)
		return ;
	txtr = type + TXTR_COIN;
	if (within(type, -1, TOTAL_TYPES) && within(txtr, -1, TOTAL_TXTRS) && t[txtr])
	{
		if (type == KEY || type == HEALTH)
		{
			box.x += box.w * 0.2;
			box.w *= 0.8;
		}
		render_box(box, t[txtr], rend);
	}
}

void					render_items(SDL_Renderer *rend, t_world *world, t_itemfull *itemfull, int n, t_grid *grid, SDL_Texture **t)
{
	int 				sector;
	int 				i;
	int 				id;
	t_vec2d				p;
	t_rec				box;

	if (!rend || !world || !itemfull || !grid)
		return ;
	sector = select_it(1, S_SELECT, -1);
	if (!within(sector, -1, world->n_s) || world->sec[sector].n_items < 1 || !world->sec[sector].items)
		return ;
	i = 0;
	box = (t_rec){ 0, 0, grid->box.w * 0.009, grid->box.w * 0.009 };
	while (i < world->sec[sector].n_items)
	{
		id = world->sec[sector].items[i].id;
		p = world->sec[sector].items[i].p;
		if (within(p.x, -1, GRID_SIZE) && within(p.y, -1, GRID_SIZE) &&
		node_in_sec(p, world) == sector && grid->nodes[p.x][p.y] == (signed char)(-10 - i))
		{
			p.x = (int)(grid->box.x + (world->sec[sector].items[i].p.x * grid->scale));
			p.y = (int)(grid->box.y + (world->sec[sector].items[i].p.y * grid->scale));
			if (within(id, -1, n))
			{
				box.x = p.x - box.w / 2;
				box.y = p.y - box.h / 2;
				draw_item(itemfull[id].type, box, rend, t);
			}
		}
		else
			delete_item(&world->sec[sector], i--);
		i++;
	}
}

int 					find_def_item(int type, t_itemfull *itemfull, int n)
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

void					add_item(int default_id, t_vec2d mouse, t_grid *grid, t_sec *sector)
{
	t_vec2d				node;
	int 				i;
	t_item				*new;

	if (!grid || !sector || sector->n_items >= MAX_SEC_ITEMS || default_id == -1)
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

void					move_item(t_prog *prog, t_vec2d mouse, t_grid *grid, t_sec *sector)
{
	static int			id = -1;
	static t_vec2d		to_erase = { -1, -1 };


	if (!prog || !grid || !sector)
		return ;
	if (prog->click.x || prog->click.y)
	{
		if (mouse_over(grid->box, mouse))
		{
			if (grid->p[0].x == -1)
			{
				grid->p[0] = find_node(mouse.x, mouse.y, grid);
				if (grid->p[0].x >= 0 && grid->p[0].y >= 0 &&
					grid->nodes[grid->p[0].x][grid->p[0].y] < -9)
					id = (grid->nodes[grid->p[0].x][grid->p[0].y] + 10) * (-1);
				if (!within(id, -1, sector->n_items))
				{
					grid->p[0] = (t_vec2d){ -1, -1 };
					move_grid_drag(prog, mouse, grid);
					id = -1;
				}
				select_it(0, I_SELECT, id);
				to_erase = grid->p[0];
			}
			else if (id >= 0)
			{
				grid->p[1] = find_node(mouse.x, mouse.y, grid);
				if (grid->nodes[grid->p[1].x][grid->p[1].y] == NODE_EMPTY)
				{
					if (to_erase.x != -1)
						grid->nodes[to_erase.x][to_erase.y] = NODE_EMPTY;
					sector->items[id].p = grid->p[1];
					grid->nodes[grid->p[1].x][grid->p[1].y] = (signed char)(-10 - id);
					to_erase = grid->p[1];
				}
			}
			prog->click = mouse;
			prog->redraw = 1;
		}
		else
			prog->click = (t_vec2d){ 0, 0 };
	}
	else
	{
		grid->p[0] = (t_vec2d){ -1, -1 };
		id = -1;
	}
}