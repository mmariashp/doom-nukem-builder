/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   render_grid_iso.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mshpakov <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/09/29 15:56:13 by mshpakov          #+#    #+#             */
/*   Updated: 2019/09/29 15:56:14 by mshpakov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "builder.h"

t_vec_d					make_iso(t_vec v, int z)
{
	t_vec_d				res;
	static double 		cos_iso = 0;
	static double 		sin_iso = 0;

	if (!cos_iso)
	{
		cos_iso = cos(0.523599);
		sin_iso = cos(0.523599);
	}
	res.x = (v.x - v.y) * cos_iso;
	res.y = -z + (v.x + v.y) * sin_iso;
	return (res);
}

unsigned short			fill_sector_iso(t_world world, t_grid *grid, \
int **screen, int sec)
{
	int					i;
	int					j;
	int					color;
	t_vec_d				tmp;
	t_vec				p[MAX_SEC_WALLS];

	i = 0;
	j = sec;
	if (world.sec[j].n_v < 3)
		return (FALSE);
	while (i < world.sec[j].n_v)
	{
		tmp = make_iso(world.vecs[world.sec[j].v[i]], world.sec[j].fl);
		p[i].x = (int)(grid->box.x + tmp.x * grid->scl);
		p[i].y = (int)(grid->box.y + tmp.y * grid->scl);
		i++;
	}
	color = world.sec[j].status == SEC_CONVEX_CLOSED ? \
	CONVEX_COLOR : CONCAVE_COLOR;
	return (fill_polygon(p, world.sec[j].n_v, screen, color));
}

t_vec					tmp_op(int onex, int oney, t_vec_d two, float three)
{
	int					x;
	int					y;

	x = (int)(onex + two.x * three);
	y = (int)(oney + two.y * three);
	return ((t_vec){ x, y });
}

void					draw_walls_iso(t_world w, t_grid *g, int **screen, \
t_sec *s)
{
	int					i;
	t_vec				v[4];
	t_vec_d				vd[4];
	int					color;

	i = -1;
	while (++i < s->n_w)
	{
		vd[0] = make_iso(w.vecs[w.walls[s->s_walls[i]].v1], s->fl);
		vd[1] = make_iso(w.vecs[w.walls[s->s_walls[i]].v2], s->fl);
		vd[2] = make_iso(w.vecs[w.walls[s->s_walls[i]].v1], s->ceil);
		vd[3] = make_iso(w.vecs[w.walls[s->s_walls[i]].v2], s->ceil);
		v[0] = tmp_op(g->box.x, g->box.y, vd[0], g->scl);
		v[1] = tmp_op(g->box.x, g->box.y, vd[1], g->scl);
		v[2] = tmp_op(g->box.x, g->box.y, vd[2], g->scl);
		v[3] = tmp_op(g->box.x, g->box.y, vd[3], g->scl);
		color = w.walls[s->s_walls[i]].type == WALL_EMPTY ? EMPTY_WALL_CLR : FULL_WALL_CLR;
		draw_thick_line((t_line){ v[0], v[1] }, color, WALL_R, screen);
		draw_thick_line((t_line){ v[2], v[3] }, color, WALL_R, screen);
		draw_thick_line((t_line){ v[0], v[2] }, color, WALL_R, screen);
		draw_thick_line((t_line){ v[1], v[3] }, color, WALL_R, screen);
		draw_thick_line((t_line){ v[0], v[3] }, DARK_GRAY, WALL_R / 2, screen);
		draw_thick_line((t_line){ v[1], v[2] }, DARK_GRAY, WALL_R / 2, screen);
	}
}

void					render_grid_iso(t_world world, t_grid *grid, \
t_prog *prog)
{
	t_vec				tab[world.n_s];
	int					k;
	unsigned short		overlay;

	if (!grid || !prog)
		return ;
	clean_screen(prog->screen);
	overlay = FALSE;
	k = -1;
	while (++k < world.n_s)
	{
		tab[k].x = k;
		tab[k].y = world.sec[k].fl;
	}
	k = -1;
	while (++k < world.n_s)
	{
		if (world.sec[tab[k].x].status != SEC_OPEN)
		{
			draw_walls_iso(world, grid, prog->screen, &world.sec[tab[k].x]);
			overlay = fill_sector_iso(world, grid, prog->screen, tab[k].x) \
			== TRUE ? TRUE : overlay;
		}
	}
}
