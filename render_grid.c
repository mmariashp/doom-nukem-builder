
#include "builder.h"

t_vec2d                  make_iso(int x, int y, int z)
{
    t_vec2d             res;

    res.x = (x - y) * cos(0.523599);
    res.y = -z + (x + y) * sin(0.523599);
    return (res);
}

void					draw_node(t_vec2d c, int r, int color, int **screen)
{
	draw_circle_fill2(c, r, color, screen);
}

void					draw_player(t_vec2d c, int r, int color, int **screen)
{
	draw_circle_fill2(c, r * 1.5, DARK_GRAY, screen);
	draw_circle_fill2(c, r, color, screen);
	draw_circle_fill2(c, r * 0.5, WHITE, screen);
}

void					render_grid_nodes(int **screen, t_grid *grid)
{
	int 				x;
	int 				y;
	t_vec2d				node;
	int 				radius1;
	int 				radius2;

	grid->box.w = GRID_SIZE * grid->scale;
	grid->box.h = GRID_SIZE * grid->scale;
	radius1 = grid->box.w * 0.001;
	radius2 = grid->box.w * 0.002;
	y = 0;
	while (y < GRID_SIZE)
	{
		x = 0;
		node.y = (int)(grid->box.y + (y * grid->scale));

		while (x < GRID_SIZE)
		{
			node.x = (int)(grid->box.x + (x * grid->scale));
			if (within(node.x, -1, WIN_W) && within(node.y, -1, WIN_H))
			{
				if (grid->nodes[x][y] == NODE_FULL)
					draw_node(node, radius2, BROWN, screen);
				else if (grid->nodes[x][y] == NODE_SECTOR)
					draw_node(node, radius2, YELLOW, screen);
				else if (grid->nodes[x][y] == NODE_EMPTY)
				{
					if (screen[node.x][node.y] == 0)
						draw_node(node, radius1, DARK_GRAY, screen);
				}
//				else
//				{
//					draw_node(node, radius2, RED, screen);
//				}
			}
			x++;
		}
		y++;
	}
}




void					place_player(t_world world, t_grid *grid, int **screen, int radius)
{
	t_vec2d				node;

	node.x = (int)(grid->box.x + world.p_start.x * grid->scale);
	node.y = (int)(grid->box.y + world.p_start.y * grid->scale);
	draw_player(node, radius * 1.5, RED, screen);

	node.x = (int)(grid->box.x + world.p_end.x * grid->scale);
	node.y = (int)(grid->box.y + world.p_end.y * grid->scale);
	draw_player(node, radius * 1.5, GREEN, screen);
}



void					draw_walls(t_world world, t_grid *grid, int **screen, int wall)
{
	int					i = 0;
	t_vec2d				v1;
	t_vec2d				v2;

	while (i < world.n_walls)
	{
		v1 = world.vecs[world.walls[i].v1];
		v2 = world.vecs[world.walls[i].v2];

		v1.x = (int)(grid->box.x + v1.x * grid->scale);
		v1.y = (int)(grid->box.y + v1.y * grid->scale);
		v2.x = (int)(grid->box.x + v2.x * grid->scale);
		v2.y = (int)(grid->box.y + v2.y * grid->scale);

		if (i == wall)
			draw_thick_line((t_line){ v1, v2 }, NAVY, LIT_WALL_RADIUS, screen);
		if (world.walls[i].type == WALL_EMPTY)
			draw_thick_line((t_line){ v1, v2 }, GREEN, WALL_RADIUS, screen);
		else if (world.walls[i].type == WALL_FILLED)
			draw_thick_line((t_line){ v1, v2 }, WHITE, WALL_RADIUS, screen);
		else if (world.walls[i].type == WALL_DOOR)
			draw_thick_line((t_line){ v1, v2 }, SOFT_ORANGE, WALL_RADIUS, screen);
		i++;
	}
}

//unsigned short 			is_convex(t_vec2d *p, int n)
//{
//	short 			i;
//	short 			j;
//	short 			k;
//	char 			pos;
//	char 			neg;
//	int 			cross;
//	t_vec2d			pi;
//	t_vec2d			pj;
//	t_vec2d			pk;
//
//	i = 0;
//	pos = 0;
//	neg = 0;
//	while (i < n)
//	{
//		j = (i + 1) % n;
//		k = (j + 1) % n;
//		pi = p[i];
//		pj = p[j];
//		pk = p[k];
//		cross = (pi.x - pj.x) * (pk.x - pj.x) - (pi.y - pj.y) * (pk.y - pj.y);
//		if (cross < 0)
//			neg = 1;
//		else if (cross > 0)
//			pos = 1;
//		if (pos != 0 && neg != 0)
//			return (FALSE);
//		i++;
//	}
//	return (TRUE);
//}

void					fill_sector(t_world world, t_grid *grid, int **screen, int sec, int state)
{
	int					i = 0;
	int					j = sec;
	int					color;
	t_vec2d				p[world.sec[j].n_v];

	if (world.sec[j].n_v < 3)
		return ;
	while (i < world.sec[j].n_v)
	{
		p[i] = world.vecs[world.sec[j].v[i]];
		p[i].x = (int)(grid->box.x + p[i].x * grid->scale);
		p[i].y = (int)(grid->box.y + p[i].y * grid->scale);
		i++;
	}
	if (state == SECTOR_SEARCH || state == SECTOR_EDIT)
	{
		if (sec == lit_item(1, S_SELECT, 0))
			color = ACTIVE_SECTOR_COLOR;
		else
			color = world.sec[j].status == SEC_CONVEX_CLOSED ? CONVEX_COLOR : CONCAVE_COLOR;
	}
	else
		color = world.sec[j].status == SEC_CONVEX_CLOSED ? CONVEX_COLOR : CONCAVE_COLOR;
	fillpoly(p, world.sec[j].n_v, screen, color);
}

void					clean_screen(int **screen)
{
	int					i;

	if (!screen)
		return ;
	i = 0;
	while (i < WIN_W)
	{
		ft_memset(screen[i], 0, sizeof(int) * WIN_H);
		i++;
	}
}

void					render_grid(t_world world, t_grid *grid, t_prog *prog, t_vec2d mouse)
{
	t_vec2d				node;
	t_vec2d				node2;
	int 				radius1;
	int 				radius2;
	int					wall;


	if (!grid || !prog || world.n_sec == 100)
		return;
	clean_screen(prog->screen);
	radius1 = grid->box.w * 0.001;
	radius2 = grid->box.w * 0.002;
	wall = selected_item(1, W_SELECT, -1);
	if (wall == -1)
		wall = lit_item(1, W_SELECT, -1);
	draw_walls(world, grid, prog->screen, wall);
	render_grid_nodes(prog->screen, grid);
	place_player(world, grid, prog->screen, radius2);
	if (selected_item(1, STATE_SELECT, -1) == NORMAL && prog->button_on == DRAW_BTN) // draw mode
	{
		if (grid->active[0].x != -1 && grid->active[0].y != -1)
		{
			node.x = (int)(grid->box.x + grid->active[0].x * grid->scale);
			node.y = (int)(grid->box.y + grid->active[0].y * grid->scale);
			draw_node(node, radius2, BABY_PINK, prog->screen);
			if (grid->active[1].x != -1 && grid->active[1].y != -1)
			{
				node2.x = (int)(grid->box.x + grid->active[1].x * grid->scale);
				node2.y = (int)(grid->box.y + grid->active[1].y * grid->scale);
				draw_node(node2, radius1, BABY_PINK, prog->screen);
				draw_line2((t_line){ node, node2 }, BABY_PINK, prog->screen);
			}
			else
				draw_line2((t_line){ node, mouse }, BABY_PINK, prog->screen);
		}
	}
	int k = 0;
	while (k < world.n_sec)
	{
		if (world.sec[k].status != SEC_OPEN)
			fill_sector(world, grid, prog->screen, k, selected_item(1, STATE_SELECT, 0));
		k++;
	}
}