/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   add_door.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mshpakov <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/09/28 16:07:04 by mshpakov          #+#    #+#             */
/*   Updated: 2019/09/28 16:07:06 by mshpakov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "builder.h"

void					get_new_p(t_vec *new, t_grid *grid, t_world *world)
{
	if (!world || !grid || !new || !within(new[0].x, -1, GRID_SIZE) ||
		!within(new[0].y, -1, GRID_SIZE) || !within(new[1].x, -1, GRID_SIZE) ||
		!within(new[1].y, -1, GRID_SIZE))
		return ;
	if (nod_in_sec(new[0], world) != -1 ||
		grid->nod[new[0].x][new[0].y] != NODE_EMPTY ||
		find_wall_inter(new[0], *world) != -1)
		new[0] = (t_vec){ -1, -1 };
	if (nod_in_sec(new[1], world) != -1 ||
		grid->nod[new[1].x][new[1].y] != NODE_EMPTY ||
		find_wall_inter(new[1], *world) != -1)
		new[1] = (t_vec){ -1, -1 };
}

unsigned short			will_inter(t_world *world, t_vec one, t_vec two)
{
	unsigned short		i;
	int					j;
	t_vec				v1;
	t_vec				v2;

	i = FALSE;
	j = -1;
	if (!world || !world->walls || !world->vecs)
		return (i);
	while (++j < world->n_w && i == FALSE)
	{
		if (within(world->walls[j].v1, -1, world->n_v) &&
			within(world->walls[j].v2, -1, world->n_v))
		{
			v1 = world->vecs[world->walls[j].v1];
			v2 = world->vecs[world->walls[j].v2];
			lines_intersect((t_line){ one, two }, (t_line){ v1, v2 }, &i);
		}
	}
	return (i);
}

void					get_door_points(t_vec *p, t_grid *grid, \
													t_world *world, float slope)
{
	t_vec				*q[2];

	if (fabs(slope) != 1 && p[0].x != p[1].x && p[0].y != p[1].y)
		return ;
	if (!(q[0] = get_point_at_d(p[0], 1, (int)slope)))
		return ;
	get_new_p(q[0], grid, world);
	if (!(q[1] = get_point_at_d(p[1], 1, (int)slope)))
	{
		free(q[0]);
		return ;
	}
	get_new_p(q[1], grid, world);
	if (q[0][0].x > 0 && q[1][0].x > 0 && !will_inter(world, q[0][0], q[1][0]))
	{
		p[2] = q[0][0];
		p[3] = q[1][0];
	}
	if (q[0][1].x > 0 && q[1][1].x > 0 && !will_inter(world, q[0][1], q[1][1]))
	{
		p[2] = q[0][1];
		p[3] = q[1][1];
	}
	free(q[1]);
	free(q[0]);
}

unsigned short			add_dsec(t_sec **sec, short n_s, int wall)
{
	*sec = (t_sec *)realloc_tab(*sec, sizeof(t_sec) * (n_s + 1), \
	sizeof(t_sec) * n_s);
	if (!*sec)
		return (FAIL);
	(*sec)[n_s].s_walls = NULL;
	(*sec)[n_s].items = NULL;
	(*sec)[n_s].fl = default_heights(1, 0, -1);
	(*sec)[n_s].ceil = default_heights(1, 1, -1);;
	(*sec)[n_s].fl_t = (short)default_texture(1, DEFAULT_FLR_T, -1);
	(*sec)[n_s].ceil_t = (short)default_texture(1, DEFAULT_DOOR_T, -1);
	(*sec)[n_s].n_w = 4;
	(*sec)[n_s].n_it = 0;
	(*sec)[n_s].n_v = 4;
	(*sec)[n_s].is_door = TRUE;
	(*sec)[n_s].status = SEC_CONVEX_CLOSED;
	if (!((*sec)[n_s].s_walls = ft_memalloc(sizeof(int) * (*sec)[n_s].n_w)))
		return (FAIL);
	(*sec)[n_s].s_walls[0] = wall;
	return (SUCCESS);
}

void					add_door(t_world *w, int wall, t_grid *g)
{
	t_vec				p[4];
	int					v[4];

	if (!w || !w->walls || !w->vecs || !g || !within(wall, -1, w->n_w) ||
		!within(w->walls[wall].v1, -1, w->n_v) ||
		!within(w->walls[wall].v2, -1, w->n_v))
		return ;
	vec_set(p, -1, -1, 4);
	p[0] = w->vecs[w->walls[wall].v1];
	p[1] = w->vecs[w->walls[wall].v2];
	get_door_points((t_vec *)p, g, w, get_perp_slope(p[0], p[1]));
	if (p[2].x == -1 || p[3].x == -1 || add_dsec(&w->sec, w->n_s, wall) == FAIL)
		return ;
	g->p[0] = p[2];
	g->p[1] = p[3];
	v[1] = w->walls[wall].v2;
	w->walls[wall].txtr = default_texture(1, DEFAULT_DOOR_T, -1);
	if (add_vec(&w->vecs, (v[2] = w->n_v++), g, 0) == SUCCESS && add_vec(\
	&w->vecs, (v[3] = w->n_v++), g, 1) == SUCCESS && add_wall(&w->walls, (\
	w->sec[w->n_s].s_walls[1] = w->n_w++), v[1], v[3]) \
	== SUCCESS && add_wall_door(&w->walls, (w->sec[w->n_s].s_walls[2] = \
	w->n_w++), v[3], v[2]) == SUCCESS && add_wall(&w->walls, \
	(w->sec[w->n_s].s_walls[3] = w->n_w++), v[2], (v[0] = w->walls[wall].v1)) \
	== SUCCESS)
		get_sec_v(&w->sec[w->n_s++], w->walls);
	else
		return ;
}
