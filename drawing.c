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

int						add_start_vec(t_grid *grid, t_world *world, int *sector)
{
	int 				id;

	id = -1;
	if (!can_place_vec(grid->p[0], *world, grid))
	{
		grid->p[0] = (t_vec2d){ -1, -1 };
		return (-1);
	}
	if (grid->nodes[grid->p[0].x][grid->p[0].y] == NODE_EMPTY ||
		(grid->nodes[grid->p[0].x][grid->p[0].y] == NODE_FULL &&
		 (id = find_vec(world->vecs, grid->p[0], world->n_v)) == -1))
	{
		if (add_vec(&world->vecs, world->n_v, grid, 0) == FAIL)
			return (-1);
		id = world->n_v++;
	}
	if (id == -1)
	{
		grid->p[0] = (t_vec2d){ -1, -1 };
		return (-1);
	}
	if (add_sector(&world->sec, world->n_s) == FAIL)
		return (-1);
	*sector = world->n_s++;
	if (add_sector_v(&world->sec[*sector].v, world->sec[*sector].n_v, id) == FAIL)
		return (-1);
	world->sec[*sector].n_v++;
	return (id);
}




void					add_to_media(t_grid *grid, t_world *world)
{
	static int	 		first_vector = -1;
	static int	 		sector = -1;
	static int	 		last_id = -1;
	short 				wall;
	short 				id;
	short 				done;

	if (!world || !grid)
		return ;
	id = -1;
	done = FALSE;
	wall = -1;
	if (grid->p[1].x == -1)
	{
		last_id = add_start_vec(grid, world, &sector);
		first_vector = last_id;
	}
	else if (within(sector, -1, world->n_s) && first_vector != -1)
	{
//		add_second_vec(grid, world, &sector);
		if (grid->nodes[grid->p[1].x][grid->p[1].y] == NODE_FULL)
			id = find_vec(world->vecs, grid->p[1], world->n_v);
		if (id != first_vector && !can_place_vec(grid->p[1], *world, grid))
		{
			grid->p[1] = (t_vec2d){ -1, -1 };
			return ;
		}
		if (id == -1 && grid->nodes[grid->p[1].x][grid->p[1].y] == NODE_EMPTY)
		{
			if (add_vec(&world->vecs, world->n_v, grid, 1) == FAIL)
				return ;
			id = world->n_v++;
		}
		else
		{
			if (id == first_vector)
				done = TRUE;
			else if (id == -1 || already_in_sector(id, world->sec[sector].v, world->sec[sector].n_v))
			{
				grid->p[1] = (t_vec2d){ -1, -1 };
				return ;
			}
			else if (grid->nodes[world->vecs[last_id].x][world->vecs[last_id].y] == NODE_FULL &&
					 grid->nodes[world->vecs[id].x][world->vecs[id].y] == NODE_FULL)
				wall = find_wall(last_id, id, world->walls, world->n_w);
		}
		if (wall == -1)
		{
			if (add_wall(&world->walls, world->n_w, last_id, id) == FAIL)
				return ;
			wall = world->n_w++;
		}
		if (add_secwall(&world->sec[sector].s_walls, world->sec[sector].n_w, wall) == FAIL)
			return ;
		world->sec[sector].n_w++;
		if (add_sector_v(&world->sec[sector].v, world->sec[sector].n_v, id) == FAIL)
			return ;
		world->sec[sector].n_v++;
		grid->p[0] = grid->p[1];
		grid->p[1] = (t_vec2d){ -1, -1 };
		last_id = id;
	}
	else
		return ;
	if (done == TRUE)
	{
		vec_set(grid->p, -1, -1, 2);
		last_id = -1;
		sector = -1;
		first_vector = -1;
	}
}

void 					drawing(t_world *world, t_prog *prog, t_grid *grid,
		t_vec2d mouse)
{
	int 				i;
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
		prog->click = (t_vec2d){ 0, 0 };
	}
	prog->redraw = 1;
}