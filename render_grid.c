
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
			if (grid->nodes[x][y] == NODE_FULL)
				draw_node(node, radius2, BROWN, screen);
            else if (grid->nodes[x][y] == NODE_SECTOR)
                draw_node(node, radius2, YELLOW, screen);
			else
				draw_node(node, radius1, DARK_GRAY, screen);
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
	draw_player(node, radius * 1.5, PURPLE, screen);

	node.x = (int)(grid->box.x + world.p_end.x * grid->scale);
	node.y = (int)(grid->box.y + world.p_end.y * grid->scale);
	draw_player(node, radius * 1.5, YELLOW, screen);
}

void					draw_walls(t_world world, t_grid *grid, int **screen)
{
	int					i = 0;
	t_vec2d				v1;
	t_vec2d				v2;

	while (i < world.n_walls)
	{
		v1 = world.vertices[world.walls[i].v1];
		v2 = world.vertices[world.walls[i].v2];

		v1.x = (int)(grid->box.x + v1.x * grid->scale);
		v1.y = (int)(grid->box.y + v1.y * grid->scale);
		v2.x = (int)(grid->box.x + v2.x * grid->scale);
		v2.y = (int)(grid->box.y + v2.y * grid->scale);

//		if (world.walls[i].type == WALL_EMPTY)
//			draw_line2((t_line){ v1, v2 }, YELLOW, screen);
//		else if (world.walls[i].type == WALL_FILLED)
//			draw_line2((t_line){ v1, v2 }, WHITE, screen);
//		else if (world.walls[i].type == WALL_DOOR)
//			draw_line2((t_line){ v1, v2 }, BABY_PINK, screen);

        if (world.walls[i].type == WALL_EMPTY)
            draw_line2((t_line){ v1, v2 }, YELLOW, screen);
        else if (world.walls[i].type == WALL_FILLED)
            draw_line2((t_line){ v1, v2 }, WHITE, screen);
        else if (world.walls[i].type == WALL_DOOR)
            draw_line2((t_line){ v1, v2 }, BABY_PINK, screen);
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

void					fill_sector(t_world world, t_grid *grid, int **screen, int sec, int button)
{
	int					i = 0;
	int					j = sec;
	int					color;
	t_vec2d				p[world.sectors[j].n_v];

	if (world.sectors[j].n_v < 3)
		return ;
	while (i < world.sectors[j].n_v)
	{
		p[i] = world.vertices[world.sectors[j].v[i]];
		p[i].x = (int)(grid->box.x + p[i].x * grid->scale);
		p[i].y = (int)(grid->box.y + p[i].y * grid->scale);
		i++;
	}
	if (button == SECTOR_BUTTON || button == DESELECT_SEC_BUTTON)
	{
		if (sec == grid->active[0].x)
			color = ACTIVE_SECTOR_COLOR;
		else
			color = world.sectors[j].status == SEC_CONVEX_CLOSED ? CONVEX_COLOR : CONCAVE_COLOR;
	}
	else
		color = world.sectors[j].status == SEC_CONVEX_CLOSED ? CONVEX_COLOR : CONCAVE_COLOR;
	fillpoly(p, world.sectors[j].n_v, screen, color);
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

	if (!grid || !prog || world.n_sectors == 100)
		return;
	clean_screen(prog->screen);
	radius1 = grid->box.w * 0.001;
	radius2 = grid->box.w * 0.002;
	render_grid_nodes(prog->screen, grid);
	draw_walls(world, grid, prog->screen);
	place_player(world, grid, prog->screen, radius2);
	if (prog->button_on == DRAW_BUTTON) // draw mode
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
	if (prog->mode_id == MODE_EDITOR)
	{
		while (k < world.n_sectors)
		{
			if (world.sectors[k].status != SEC_OPEN)
				fill_sector(world, grid, prog->screen, k, prog->button_on);
			k++;
		}
	}
}