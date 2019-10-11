/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   drawing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mshpakov <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/09/28 18:54:43 by mshpakov          #+#    #+#             */
/*   Updated: 2019/10/11 13:46:18 by mshpakov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "builder.h"

void					break_in_prev(int *s_walls, int n, int prev)
{
	int 				new;
	int 				tmp[n];
	int					k;

	new = n - 1;
	int i = 0;
	while (i < n)
	{
		tmp[i] = s_walls[i];
		i++;
	}
	i = 0;
	while (i < prev)
		i++;
	s_walls[i++] = tmp[new];
	while (i < n)
	{
		k = i - 1;
		if (k < 0)
			k = n - 1;
		s_walls[i++] = tmp[k];
	}
}

void					add_broken_secwall(t_world *w, int wall, int new_id, \
int s)
{
	int 				i;
	int 				j;
	int 				prv_nxt[2];

	if (!w)
		return ;
	i = -1;
	while (++i < w->n_s)
	{
//		printf("checking sector %d\n", i);
		if (i == s)
		{
//			printf("SKIPPING\n");
			continue ;
		}
		j = -1;
		while (++j < w->sec[i].n_w)
		{
			if (w->sec[i].s_walls[j] != wall)
				continue ;
//			printf("secwall %d = %d; (WALL is %d)\nsector walls were:\n", j, w->sec[i].s_walls[j], wall);
//			int k = 0;
//			while (k < w->sec[i].n_w)
//			{
//				printf("%d, ", w->sec[i].s_walls[k]);
//				k++;
//			}
//			printf("\n");
//			printf("sector vecs were:\n");
//			k = 0;
//			while (k < w->sec[i].n_v)
//			{
//				printf("%d, ", w->sec[i].v[k]);
//				k++;
//			}
//			printf("\n");

			add_secwall(&w->sec[i].s_walls, w->sec[i].n_w++, new_id);

//			printf("AFTER ADD SECWALL walls:\n");
//			k = 0;
//			while (k < w->sec[i].n_w)
//			{
//				printf("%d, ", w->sec[i].s_walls[k]);
//				k++;
//			}
//			printf("\n");

			if ((prv_nxt[0] = j - 1) < 0)
				prv_nxt[0] = w->sec[i].n_w - 2;
			if ((prv_nxt[1] = j + 1) >= w->sec[i].n_w)
				prv_nxt[1] = 0;

//			printf("prvnxt %d: %d-%d ; %d: %d-%d\n", prv_nxt[0], w->walls[w->sec[i].s_walls[prv_nxt[0]]].v1, w->walls[w->sec[i].s_walls[prv_nxt[0]]].v2,
//					prv_nxt[1] , w->walls[w->sec[i].s_walls[prv_nxt[1]]].v1, w->walls[w->sec[i].s_walls[prv_nxt[1]]].v2);
//			printf("points to compare: %d,%d\n", w->walls[w->sec[i].s_walls[j]].v1, w->walls[w->sec[i].s_walls[j]].v2);

			if (w->walls[w->sec[i].s_walls[prv_nxt[0]]].v1 != w->walls[w->sec[
					i].s_walls[j]].v1 &&
				w->walls[w->sec[i].s_walls[prv_nxt[0]]].v2 !=
				w->walls[w->sec[i].s_walls[j]].v1)
			{
//				printf("will put new wall before, prev= %d\n", w->sec[i].s_walls[prv_nxt[0]]);
				break_in_prev(w->sec[i].s_walls, w->sec[i].n_w, prv_nxt[0]);
			}
			else
			{
//				printf("will put new wall after, prev= %d\n", w->sec[i].s_walls[prv_nxt[1]]);
				break_in_prev(w->sec[i].s_walls, w->sec[i].n_w, prv_nxt[1]);
			}
//			printf("AFTER BREAK IN walls:\n");
//			k = 0;
//			while (k < w->sec[i].n_w)
//			{
//				printf("%d, ", w->sec[i].s_walls[k]);
//				k++;
//			}
//			printf("\n");
			if (w->sec[i].v)
				free(w->sec[i].v);
			get_sec_v(&w->sec[i], w->walls);

//			printf("sector vecs are now:\n");
//			k = 0;
//			while (k < w->sec[i].n_v)
//			{
//				printf("%d, ", w->sec[i].v[k]);
//				k++;
//			}
//			printf("\n");

			make_continuous(&w->sec[i], w, i);
			validate_clockwise(w, i);
			break ;
		}
	}
}

unsigned short			break_wall(t_world *world, int vec, int wall, int s)
{
	int					two;
	int 				new_id;

	if (!world || !within(wall, -1, world->n_w) || !within(vec, -1, world->n_v))
		return (FAIL);
	printf("wall points %d-%d\n", world->walls[wall].v1, world->walls[wall].v2);
	two = world->walls[wall].v2;
	if (add_wall(&world->walls, world->n_w, vec, two) == FAIL)
		return (FAIL);
	printf("new wall points %d-%d\n", world->walls[world->n_w].v1, world->walls[world->n_w].v2);
	new_id = world->n_w;
	world->walls[wall].v2 = vec;
	printf("updated old wall points %d-%d\n", world->walls[wall].v1, world->walls[wall].v2);
	world->n_w++;
	add_broken_secwall(world, wall, new_id, s);
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
		{
			printf("breaking wall %d, sector = %d\n", wall, -1);
			break_wall(w, id, wall, -1);
		}
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
		{
			printf("breaking wall %d, sector = %d\n", wall, f_s_l[1]);
			break_wall(world, wall_id_done[1], wall, f_s_l[1]);
		}
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
