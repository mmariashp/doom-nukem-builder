
#include "builder.h"

void					fill_grid_walls(int n_walls, t_wall *walls, int n_vecs, t_vec2d *vecs, t_grid *grid)
{
	int                 wall_i;
	int                 v1;
	int                 v2;

	if (!walls || !vecs || !grid || n_walls < 1 || n_vecs < 2 || n_walls >= MAX_N_WALLS)
		return ;
	wall_i = 0;
	while (wall_i < n_walls)
	{
		v1 = walls[wall_i].v1;
		v2 = walls[wall_i].v2;
		if (v1 >= 0 && v1 < n_vecs && v2 >= 0 && v2 < n_vecs)
		{
			draw_line_grid((t_line){ vecs[v1], vecs[v2] }, (signed char)wall_i, grid->nodes);
		}
		wall_i++;
	}
}

void					fill_grid(int n_vecs, t_vec2d *vecs, t_grid *grid)
{
	int 				i;

	if (!vecs || !grid || n_vecs < 1)
		return ;
	i = 0;
	while (i < n_vecs)
	{
		grid->nodes[vecs[i].x][vecs[i].y] = NODE_FULL;
		i++;
	}
}

void					fill_grid_items(t_sec *sector, t_grid *grid)
{
	int 				i;
	t_vec2d				p;

	if (!grid || !sector)
		return ;
	i = 0;
	while (i < sector->n_items)
	{
		p = sector->items[i].p;
		if (within(p.x, -1, GRID_SIZE) && within(p.y, -1, GRID_SIZE) && grid->nodes[p.x][p.y] == NODE_EMPTY)
		{
			grid->nodes[p.x][p.y] = (signed char)(-10 - i);
//			printf("i is %d, making node = %d\n", i, grid->nodes[p.x][p.y]);
		}
		else
			delete_item(sector, i--);
		i++;
	}
}