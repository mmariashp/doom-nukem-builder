/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   drawing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mshpakov <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/09/28 18:54:43 by mshpakov          #+#    #+#             */
/*   Updated: 2019/09/28 18:54:45 by mshpakov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "builder.h"

//j = -1;
//while (++j < w->sec[i].n_w)
//{
//if (w->sec[i].s_walls[j] != wall)
//continue ;
//add_secwall(&w->sec[i].s_walls, w->sec[i].n_w++, new_id);
//if ((prv_nxt[0] = j - 1) < 0)
//prv_nxt[0] = w->sec[i].n_w - 1;
//if ((prv_nxt[1] = j + 1) >= w->sec[i].n_w)
//prv_nxt[1] = 0;
//if (w->walls[w->sec[i].s_walls[prv_nxt[0]]].v1 != w->walls[w->sec[
//			i].s_walls[j]].v1 &&
//w->walls[w->sec[i].s_walls[prv_nxt[0]]].v2 !=
//			w->walls[w->sec[i].s_walls[j]].v1)
//swap_ints(&w->sec[i].s_walls[prv_nxt[0]],
//				&w->sec[i].s_walls[w->sec[i].n_w - 1]);
//else
//swap_ints(&w->sec[i].s_walls[prv_nxt[1]],
//				&w->sec[i].s_walls[w->sec[i].n_w - 1]);
//get_sec_v(&w->sec[i], w->walls);
//break ;
//}

void					add_broken_secwall(t_world *w, int wall, int new_id)
{
	int 				i;
	int 				j;
	int 				prv_nxt[2];

	if (!w)
		return ;
	i = -1;
	while (++i < w->n_s)
	{
		j = -1;
		while (++j < w->sec[i].n_w)
		{
			if (w->sec[i].s_walls[j] != wall)
				continue ;
			add_secwall(&w->sec[i].s_walls, w->sec[i].n_w++, new_id);
			if ((prv_nxt[0] = j - 1) < 0)
				prv_nxt[0] = w->sec[i].n_w - 1;
			if ((prv_nxt[1] = j + 1) >= w->sec[i].n_w)
				prv_nxt[1] = 0;
			if (w->walls[w->sec[i].s_walls[prv_nxt[0]]].v1 != w->walls[w->sec[
					i].s_walls[j]].v1 &&
				w->walls[w->sec[i].s_walls[prv_nxt[0]]].v2 !=
				w->walls[w->sec[i].s_walls[j]].v1)
				swap_ints(&w->sec[i].s_walls[prv_nxt[0]],
						  &w->sec[i].s_walls[w->sec[i].n_w - 1]);
			else
				swap_ints(&w->sec[i].s_walls[prv_nxt[1]],
						  &w->sec[i].s_walls[w->sec[i].n_w - 1]);
			if (w->sec[i].v)
				free(w->sec[i].v);
			get_sec_v(&w->sec[i], w->walls);
			make_continuous(&w->sec[i], w, i);
			validate_clockwise(w, i);
			break ;
		}
	}
}

unsigned short			break_wall(t_world *world, int vec_id, int wall)
{
	int					two;
	int 				new_id;

	if (!world || !within(wall, -1, world->n_w) ||
	!within(vec_id, -1, world->n_v))
		return (FAIL);
	two = world->walls[wall].v2;
	if (add_wall(&world->walls, world->n_w, vec_id, two) == FAIL)
		return (FAIL);
	new_id = world->n_w;
	world->walls[wall].v2 = vec_id;
	world->n_w++;
	add_broken_secwall(world, wall, new_id);
	return (SUCCESS);
}

int						add_start_vec(t_grid *g, t_world *w, int *sector, \
int wall)
{
	int					id;

	id = -1;
	if (!can_place_vec(g->p[0], *w, g, wall))
	{
		g->p[0] = (t_vec){ -1, -1 };
		return (-1);
	}
	if (g->nod[g->p[0].x][g->p[0].y] == -1 || (g->nod[g->p[0].x][g->p[0].y] == \
	-2 && (id = find_vec(w->vecs, g->p[0], w->n_v)) == -1))
	{
		if (add_vec(&w->vecs, (id = w->n_v++), g, 0) == FAIL)
			return (-1);
		if (wall != -1)
			break_wall(w, id, wall);
	}
	if (id == -1)
	{
		g->p[0] = (t_vec){ -1, -1 };
		return (-1);
	}
	if (add_sector(&w->sec, (*sector = w->n_s++)) == FAIL)
		return (-1);
	if (add_sector_v(&w->sec[*sector].v, w->sec[*sector].n_v++, id) == FAIL)
		return (-1);
	return (id);
}

unsigned				get_walliddone(short *wall_id_done)
{
	wall_id_done[1] = -1;
	wall_id_done[2] = FALSE;
	wall_id_done[0] = -1;
	return (TRUE);
}

unsigned				place_start(int *f_s_l, t_grid *grid, t_world *world, \
int wall)
{
	f_s_l[2] = add_start_vec(grid, world, &f_s_l[1], wall);
	f_s_l[0] = f_s_l[2];
	return (TRUE);
}

void					add_to_media(t_grid *grid, t_world *world, int wall)
{
	static int			f_s_l[3] = { -1, -1, -1 };
	short				wall_id_done[3];

	if (!world || !grid || !get_walliddone(wall_id_done) || (grid->p[1].x != -1\
	&& !(within(f_s_l[1], -1, world->n_s) && f_s_l[0] != -1)))
		return ;
	if (grid->p[1].x == -1 && place_start(f_s_l, grid, world, wall))
		return ;
	if (grid->nod[grid->p[1].x][grid->p[1].y] == NODE_FULL)
		wall_id_done[1] = find_vec(world->vecs, grid->p[1], world->n_v);
	if (wall_id_done[1] != f_s_l[0] && !can_place_vec(grid->p[1], *world, grid,\
	wall))
	{
		grid->p[1] = (t_vec){ -1, -1 };
		return ;
	}
	if (!(wall_id_done[1] == -1 && grid->nod[grid->p[1].x][grid->p[1].y] == -1))
		existing_vec(wall_id_done, f_s_l, world, grid);
	else
	{
		if (add_vec(&world->vecs, (wall_id_done[1] = world->n_v++), grid, 1)\
		== FAIL)
			return ;
		if (wall != -1)
			break_wall(world, wall_id_done[1], wall);
	}
	if (add_media_elements(world, grid, wall_id_done, f_s_l) == FAIL)
		return ;
	done_condition(grid, f_s_l, wall_id_done[2]);
}

void					drawing(t_world *world, t_prog *prog, t_grid *grid, \
t_vec mouse)
{
	int					i;
	unsigned short		vis;
	int 				wall;

	wall = -1;
	if (!world || !prog || !prog->modes || !grid)
		return ;
	if ((prog->click.x || prog->click.y) && mouse_over(grid->box, mouse))
	{
		if (grid->p[0].x == -1)
		{
			if (prog->screen[mouse.x][mouse.y].is == SCR_WALL)
				wall = prog->screen[mouse.x][mouse.y].n;
			grid->p[0] = find_node(mouse.x, mouse.y, grid);
		}
		else if (grid->p[1].x == -1)
		{
			if (prog->screen[mouse.x][mouse.y].is == SCR_WALL)
				wall = prog->screen[mouse.x][mouse.y].n;
			grid->p[1] = find_node(mouse.x, mouse.y, grid);
		}
		if (grid->p[0].x != -1)
			add_to_media(grid, world, wall);
		vis = grid->p[0].x != -1 ? FALSE : TRUE;
		i = 0;
		while (i < TOT_EDIT_BTNS)
			prog->modes[prog->m_id].btn[i++].vis_lit_on[0] = vis;
		prog->click = (t_vec){ 0, 0 };
	}
	prog->redraw = 1;
}
