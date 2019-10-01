/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   render_grid.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mshpakov <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/09/28 20:10:32 by mshpakov          #+#    #+#             */
/*   Updated: 2019/09/29 16:40:47 by mshpakov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "builder.h"

void					render_grid_nod(int **screen, t_grid *g)
{
	int					x;
	int					y;
	t_vec				n;
	int					radius;

	radius = g->box.w * 0.001;
	y = -1;
	while (++y < GRID_SIZE)
	{
		n.y = (int)(g->box.y + (y * g->scl));
		x = -1;
		while (++x < GRID_SIZE)
		{
			if (!within((n.x = (int)(g->box.x + (x * g->scl))), -1, W_W) ||
			!within(n.y, -1, W_H))
				continue ;
			if (g->nod[x][y] == NODE_FULL)
				draw_node(n, radius, FULL_COLOR, screen);
			else if (g->nod[x][y] == NODE_SEC)
				draw_node(n, radius, SEC_NOD_COLOR, screen);
			else if (!screen[n.x][n.y])
				draw_node(n, radius, EMPTY_COLOR, screen);
		}
	}
}

void					draw_walls(t_world world, t_grid *grid, int **screen, \
int wall)
{
	int					i;
	t_vec				v1;
	t_vec				v2;

	i = 0;
	while (i < world.n_w)
	{
		v1 = world.vecs[world.walls[i].v1];
		v2 = world.vecs[world.walls[i].v2];
		v1.x = (int)(grid->box.x + v1.x * grid->scl);
		v1.y = (int)(grid->box.y + v1.y * grid->scl);
		v2.x = (int)(grid->box.x + v2.x * grid->scl);
		v2.y = (int)(grid->box.y + v2.y * grid->scl);
		if (i == wall)
			draw_thick_line((t_line){ v1, v2 }, NAVY, LIT_WALL_R, screen);
		else if (world.walls[i].type == WALL_EMPTY)
			draw_thick_line((t_line){ v1, v2 }, GREEN, WALL_R, screen);
		else if (world.walls[i].type == WALL_FILLED)
			draw_thick_line((t_line){ v1, v2 }, WHITE, WALL_R, screen);
		i++;
	}
}

unsigned short			fill_sector(t_world world, t_grid *grid, int **screen, \
int sec)
{
	int					i;
	int					color;
	int					st;
	t_vec				p[MAX_SEC_WALLS];

	st = select_it(1, ST_SEL, -1);
	if (world.sec[sec].n_v < 3)
		return (FALSE);
	i = -1;
	while (++i < world.sec[sec].n_v)
		p[i] = transform_to_screen(world.vecs[world.sec[sec].v[i]], grid);
	color = world.sec[sec].status == SEC_CONVEX_CLOSED ? CONVEX_COLOR : \
	CONCAVE_COLOR;
	if ((st == SEC_SEARCH || st == SEC_EDIT) && sec == lit_it(1, S_SELECT, 0))
		color = ACTIVE_SEC_COLOR;
	if (world.sec[sec].is_door == TRUE)
		color = DOOR_COLOR;
	return (fill_polygon(p, world.sec[sec].n_v, screen, color));
}

void					drawing_nodes(t_grid *grid, int **screen, t_vec mouse)
{
	int					radius;
	t_vec				node;
	t_vec				node2;

	if (!grid || !screen)
		return ;
	radius = grid->box.w * 0.001;
	if (grid->p[0].x != -1 && grid->p[0].y != -1)
	{
		node = transform_to_screen(grid->p[0], grid);
		draw_node(node, radius, BABY_PINK, screen);
		if (grid->p[1].x != -1 && grid->p[1].y != -1)
		{
			node2 = transform_to_screen(grid->p[1], grid);
			draw_node(node2, radius, BABY_PINK, screen);
			draw_line2((t_line){ node, node2 }, BABY_PINK, screen);
		}
		else
			draw_line2((t_line){ node, mouse }, BABY_PINK, screen);
	}
}

void					render_grid(t_world world, t_grid *grid, t_prog *prog,\
t_vec mouse)
{
	int					radius;
	int					wall;
	int					k;
	unsigned short		overlay;

	if (!grid || !prog)
		return ;
	grid->box.w = GRID_SIZE * grid->scl;
	grid->box.h = GRID_SIZE * grid->scl;
	clean_screen(prog->screen);
	radius = grid->box.w * 0.001;
	if ((wall = select_it(1, W_SELECT, -1)) == -1)
		wall = lit_it(1, W_SELECT, -1);
	draw_walls(world, grid, prog->screen, wall);
	render_grid_nod(prog->screen, grid);
	place_player(world, grid, prog->screen, radius);
	if (select_it(1, ST_SEL, -1) == NORMAL && prog->btn_on == DRAW_BTN)
		drawing_nodes(grid, prog->screen, mouse);
	k = -1;
	overlay = FALSE;
	while (++k < world.n_s)
	{
		if (world.sec[k].status != SEC_OPEN)
			overlay = fill_sector(world, grid, prog->screen, k) ? 1 : overlay;
	}
}
