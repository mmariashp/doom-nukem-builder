
#include "builder.h"

void					draw_node(t_vec2d c, int r, int color, int **screen)
{
	draw_circle_fill2(c, r, color, screen);
}

void					draw_player(t_vec2d c, int r, int color, int **screen)
{
	draw_circle_fill2(c, r, color, screen);
	draw_circle_fill2(c, r * 0.5, WHITE, screen);
}

void					render_grid_nodes(t_sdl *sdl, t_t *t)
{
	int 				x;
	int 				y;
	t_vec2d				node;
	int 				radius1;
	int 				radius2;

	t->grid.box.w = GRID_SIZE * t->grid.scale;
	t->grid.box.h = GRID_SIZE * t->grid.scale;
//	render_frame(t->grid.box, PURPLE, sdl->rend);
	radius1 = t->grid.box.w * 0.001;
	radius2 = t->grid.box.w * 0.002;
	y = 0;
	while (y < GRID_SIZE)
	{
		x = 0;
		node.y = (int)(t->grid.box.y + (y * t->grid.scale));

		while (x < GRID_SIZE)
		{
			node.x = (int)(t->grid.box.x + (x * t->grid.scale));
			if (t->grid.nodes[x][y] == NODE_FULL)
				draw_node(node, radius2, BROWN, sdl->screen);
			else
				draw_node(node, radius1, DARK_GRAY, sdl->screen);
			x++;
		}
		y++;
	}
}

void					place_player(t_world world, t_grid grid, int **screen, int radius)
{
	t_vec2d				node;

	node.x = (int)(grid.box.x + world.p_start.x * grid.scale);
	node.y = (int)(grid.box.y + world.p_start.y * grid.scale);
	draw_player(node, radius * 1.5, PURPLE, screen);

	node.x = (int)(grid.box.x + world.p_end.x * grid.scale);
	node.y = (int)(grid.box.y + world.p_end.y * grid.scale);
	draw_player(node, radius * 1.5, YELLOW, screen);
}

void					render_walls(t_world world, t_grid grid, int **screen)
{
	int					i = 0;
	t_vec2d				v1;
	t_vec2d				v2;

	while (i < world.n_walls)
	{
		v1 = world.vertices[world.walls[i].v1];
		v2 = world.vertices[world.walls[i].v2];

		v1.x = (int)(grid.box.x + v1.x * grid.scale);
		v1.y = (int)(grid.box.y + v1.y * grid.scale);
		v2.x = (int)(grid.box.x + v2.x * grid.scale);
		v2.y = (int)(grid.box.y + v2.y * grid.scale);

		if (world.walls[i].type == WALL_EMPTY)
			draw_line2((t_line){ v1, v2 }, YELLOW, screen);
		else if (world.walls[i].type == WALL_FILLED)
			draw_line2((t_line){ v1, v2 }, WHITE, screen);
		else if (world.walls[i].type == WALL_DOOR)
			draw_line2((t_line){ v1, v2 }, BABY_PINK, screen);
		i++;
	}
}

unsigned short			dot_inside_sector(int x, int y, t_vec2d *p, int n)
{
	int 				i;
	int 				j;
	unsigned short		odd;

	i = 0;
	j = n - 1;
	odd = FALSE;
	while(i < n)
	{
		if ((p[i].y < y && p[j].y >= y) || (p[j].y < y && p[i].y >= y) )
		{
			if (p[i].x + (y - p[i].y) / (p[j].y - p[i].y) * (p[j].x - p[i].x) < x)
				odd = odd == FALSE ? TRUE : FALSE;
		}
		j = i;
		i++;
	}
	return (odd);
}

//void					flood_fill(int **screen, t_vec2d *p, int n, int x, int y, int new, int *count)
//{
//	if (!screen || x < 0 || y < 0 || x >= WIN_W || y >= WIN_H || *count > 30000)
//		return ;
//	if (screen[x][y] != 0 && screen[x][y] != DARK_GRAY)
//		return ;
//	screen[x][y] = new;
//
//	if (x + 1 < WIN_W && (screen[x + 1][y] == 0 || screen[x + 1][y] == DARK_GRAY) && dot_inside_sector(x + 1, y, p, n))
//	{
//		(*count)++;
//		flood_fill(screen, p, n, x + 1, y, new, count);
//	}
//	if (y + 1 < WIN_H && (screen[x][y + 1] == 0 || screen[x][y + 1] == DARK_GRAY) && dot_inside_sector(x, y + 1, p, n))
//	{
//		(*count)++;
//		flood_fill(screen, p, n, x, y + 1, new, count);
//	}
//	if (x - 1 >= 0 && (screen[x - 1][y] == 0 || screen[x - 1][y] == DARK_GRAY) && dot_inside_sector(x - 1, y, p, n))
//	{
//		(*count)++;
//		flood_fill(screen, p, n, x - 1, y, new, count);
//	}
//	if (y - 1 >= 0 && (screen[x][y - 1] == 0 || screen[x][y - 1] == DARK_GRAY) && dot_inside_sector(x, y - 1, p, n))
//	{
//		(*count)++;
//		flood_fill(screen, p, n, x, y - 1, new, count);
//	}
//}


//int			ff_start(t_vec2d d1, t_vec2d d2, int *x, int *y)
//{
//	float 	slope;
//	float dx;
//	float dy;
//	float b;
//
//	dx = d2.x - d1.x;
//	dy = d2.y - d1.y;
//	if (dx == 0)
//		return (-1);
//	slope = dy / dx;
//	b = d1.y - (slope * d1.x);
//	if (d2.x > d1.x)
//		*x = (int)(d1.x + (15 * fabs(dx)) / 100);
//	else
//		*x = (int)(d1.x - (15 * fabs(dx)) / 100);
//	*y = (int)(slope * *x + b);
////	else
////	{
////		if (d2.y > d1.y)
////			*y = d1.y + 1; //(10 * abs(dx)) / 100;
////		else
////			*y = d1.y - 1; //(10 * abs(dx)) / 100;
////
////		*x = (int)((*y - b) / slope);
////	}
//	return (1);
//}

//void				color_surface(t_poly *poly, int **screen, int color)
//{
//	int x;
//	int y;
//	int count= 0;
//
//	poly->area = find_area(*poly->a, *poly->b, *poly->c, *poly->d);
//	poly->color = color;
//	if (poly->area < 50)
//		return ;
//	if (ff_start(*poly->a, *poly->c, &x, &y) == 1 && is_in_poly(poly, x, y))
//		flood_fill(screen, x, y, RED, &count);
//	if (ff_start(*poly->c, *poly->a, &x, &y) == 1 && is_in_poly(poly, x, y))
//		flood_fill(screen, x, y, RED, &count);
//	if (ff_start(*poly->d, *poly->b, &x, &y) == 1 && is_in_poly(poly, x, y))
//		flood_fill(screen, x, y, RED, &count);
//	if (ff_start(*poly->b, *poly->d, &x, &y) == 1 && is_in_poly(poly, x, y))
//		flood_fill(screen, x, y, RED, &count);
//}

//t_vec2d					inter(int ax1, int ax2, int ay1, int ay2, int bx1, int bx2, int by1, int by2)
//{
//	float m1, c1, m2, c2;
//
//	float x1, y1, x2, y2;
//
//	float dx, dy;
//
//	float intersection_X, intersection_Y;
//
//	x1 = ax1;
//	y1 = ay1;
//	x2 = ax2;
//	y2 = ay2;
//
//
//
//	dx = x2 - x1;
//
//	dy = y2 - y1;
//
//
//
//	m1 = dy / dx;
//
//	// y = mx + c
//
//	// intercept c = y - mx
//
//	c1 = y1 - m1 * x1; // which is same as y2 - slope * x2
//
//
//
//	x1 = bx1;
//	y1 = by1;
//	x2 = bx2;
//	y2 = by2;
//
//
//
//	dx = x2 - x1;
//
//	dy = y2 - y1;
//
//
//
//	m2 = dy / dx;
//
//	// y = mx + c
//
//	// intercept c = y - mx
//
//	c2 = y1 - m2 * x1; // which is same as y2 - slope * x2
//
//
//
//	if( (m1 - m2) == 0)
//		return ((t_vec2d){ -1, -1 });
//	else
//	{
//
//		intersection_X = (c2 - c1) / (m1 - m2);
//
//		intersection_Y = m1 * intersection_X + c1;
//
//
//		return ((t_vec2d){ intersection_X, intersection_Y });
//
//	}
//}
//
//void					render_line(int xl, int xr, int y, int **screen, t_vec2d *p, int n)
//{
//	int 				i;
////	t_vec2d		v;
//
////	i = 0;
////	while (i < n - 1)
////	{
////		v = inter(xl, xr, y, y, p[i].x, p[i+1].x, p[i].y, p[i+1].y );
////		if (v.x != -1)
////		{
////			draw_dot2(v.x, v.y, RED, screen);
////		}
////		i++;
////	}
//
//	if (!p || n == 0)
//		return ;
//i = 0;
//	while (xl < xr)
//	{
//		if (screen[xl][y] != 0 && screen[xl][y] != DARK_GRAY && i == 0)
//			i = 1;
//		else if (screen[xl][y] != 0 && screen[xl][y] != DARK_GRAY && i == 1)
//			i = 0;
//
//		if (screen[xl][y] == 0 && dot_inside_sector(xl, y, p, n) == TRUE)
//			screen[xl][y] = RED;
//		xl++;
//	}
//}

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

void					fill_sector(t_world world, t_t *t, int **screen, int sec)
{
	int					i = 0;
	int					j = sec;
	int					color;
	t_vec2d				p[world.sectors[j].n_walls];

	if (world.sectors[j].n_walls < 3)
		return ;
	while (i < world.sectors[j].n_walls)
	{
		p[i] = world.vertices[world.sectors[j].v[i]];
		p[i].x = (int)(t->grid.box.x + p[i].x * t->grid.scale);
		p[i].y = (int)(t->grid.box.y + p[i].y * t->grid.scale);
		i++;
	}
	color = world.sectors[j].status == SEC_CONVEX_CLOSED ? GREEN : RED;
	fillpoly(p, world.sectors[j].n_walls, screen, color);
}

void					render_grid(t_world world, t_t *t, t_sdl *sdl)
{
	t_vec2d				node;
	t_vec2d				node2;
	int 				radius1;
	int 				radius2;

	radius1 = t->grid.box.w * 0.001;
	radius2 = t->grid.box.w * 0.002;
	if (!t || !sdl || world.n_sectors == 100)
		return;
	int k = 0;
	while (k < WIN_W)
	{
		ft_memset(sdl->screen[k], 0, sizeof(int) * WIN_H);
		k++;
	}
	render_grid_nodes(sdl, t);
	render_walls(world, t->grid, sdl->screen);
	place_player(world, t->grid, sdl->screen, radius2);
	if (sdl->button_on == 1)
	{
		if (t->active[0].x != -1 && t->active[0].y != -1)
		{
			node.x = (int)(t->grid.box.x + t->active[0].x * t->grid.scale);
			node.y = (int)(t->grid.box.y + t->active[0].y * t->grid.scale);
			draw_node(node, radius2, BABY_PINK, sdl->screen);
			if (t->active[1].x != -1 && t->active[1].y != -1)
			{
				node2.x = (int)(t->grid.box.x + t->active[1].x * t->grid.scale);
				node2.y = (int)(t->grid.box.y + t->active[1].y * t->grid.scale);
				draw_node(node2, radius1, BABY_PINK, sdl->screen);
				draw_line((t_line){ node, node2 }, BABY_PINK, sdl->rend);
			}
			else
				draw_line((t_line){ node, sdl->mouse }, BABY_PINK, sdl->rend);
		}
	}
	k = 0;
	while (k < world.n_sectors)
	{
		if (world.sectors[k].status != SEC_OPEN)
			fill_sector(world, t, sdl->screen, k);
		k++;
	}

}