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

int						add_start_vec(t_grid *g, t_world *w, int *sector)
{
	int					id;

	id = -1;
	if (!can_place_vec(g->p[0], *w, g))
	{
		g->p[0] = (t_vec){ -1, -1 };
		return (-1);
	}
	if (g->nod[g->p[0].x][g->p[0].y] == -1 || (g->nod[g->p[0].x][g->p[0].y] == \
	-2 && (id = find_vec(w->vecs, g->p[0], w->n_v)) == -1))
	{
		if (add_vec(&w->vecs, (id = w->n_v++), g, 0) == FAIL)
			return (-1);
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

unsigned				tmp_fun(int *f_s_l, t_grid *grid, t_world *world)
{
	f_s_l[2] = add_start_vec(grid, world, &f_s_l[1]);
	f_s_l[0] = f_s_l[2];
	return (TRUE);
}

void					add_to_media(t_grid *grid, t_world *world)
{
	static int			f_s_l[3] = { -1, -1, -1 };
	short				wall_id_done[3];

	if (!world || !grid || !get_walliddone(wall_id_done) || (grid->p[1].x != -1\
	&& !(within(f_s_l[1], -1, world->n_s) && f_s_l[0] != -1)))
		return ;
	if (grid->p[1].x == -1 && tmp_fun(f_s_l, grid, world))
		return ;
	if (grid->nod[grid->p[1].x][grid->p[1].y] == NODE_FULL)
		wall_id_done[1] = find_vec(world->vecs, grid->p[1], world->n_v);
	if (wall_id_done[1] != f_s_l[0] && !can_place_vec(grid->p[1], *world, grid))
	{
		grid->p[1] = (t_vec){ -1, -1 };
		return ;
	}
	if (!(wall_id_done[1] == -1 && grid->nod[grid->p[1].x][grid->p[1].y] == -1))
		fun_fun(wall_id_done, f_s_l, world, grid);
	else if (add_vec(&world->vecs, (wall_id_done[1] = world->n_v++), grid, 1)\
	== FAIL)
		return ;
	if (fun2(world, grid, wall_id_done, f_s_l) == FAIL)
		return ;
	done_condition(grid, f_s_l, wall_id_done[2]);
}

void					drawing(t_world *world, t_prog *prog, t_grid *grid, \
t_vec mouse)
{
	int					i;
	unsigned short		vis;

	if (!world || !prog || !prog->modes || !grid)
		return ;
	if ((prog->click.x || prog->click.y) && mouse_over(grid->box, mouse))
	{
		if (grid->p[0].x == -1)
			grid->p[0] = find_node(mouse.x, mouse.y, grid);
		else if (grid->p[1].x == -1)
			grid->p[1] = find_node(mouse.x, mouse.y, grid);
		if (grid->p[0].x != -1)
			add_to_media(grid, world);
		vis = grid->p[0].x != -1 ? FALSE : TRUE;
		i = 0;
		while (i < prog->modes[prog->m_id].n_btn)
			prog->modes[prog->m_id].btn[i++].vis_lit_on[0] = vis;
		prog->click = (t_vec){ 0, 0 };
	}
	prog->redraw = 1;
}
