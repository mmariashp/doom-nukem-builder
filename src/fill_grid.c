/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fill_grid.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mshpakov <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/09/28 16:18:24 by mshpakov          #+#    #+#             */
/*   Updated: 2019/09/28 16:18:25 by mshpakov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "builder.h"

void					fill_grid_walls(t_world *world, t_grid *grid)
{
	int					wall_i;
	int					v1;
	int					v2;

	if (!world || !world->walls || !world->vecs || !grid || world->n_w < 1 ||
	world->n_v < 2 || world->n_w >= MAX_N_WALLS)
		return ;
	wall_i = 0;
	while (wall_i < world->n_w)
	{
		v1 = world->walls[wall_i].v1;
		v2 = world->walls[wall_i].v2;
		if (v1 >= 0 && v1 < world->n_v && v2 >= 0 && v2 < world->n_v)
			draw_line_grid((t_line){ world->vecs[v1], world->vecs[v2] },
					(signed char)wall_i, grid->nod);
		wall_i++;
	}
}

void					fill_grid(int n_v, t_vec *vecs, t_grid *grid)
{
	int					i;

	if (!vecs || !grid || n_v < 1)
		return ;
	i = 0;
	while (i < n_v)
	{
		if (within(vecs[i].x, -1, GRID_SIZE) &&
		within(vecs[i].y, -1, GRID_SIZE))
			grid->nod[vecs[i].x][vecs[i].y] = NODE_FULL;
		i++;
	}
}

void					fill_grid_items(t_sec *sector, t_grid *grid)
{
	int					i;
	t_vec				p;

	if (!grid || !sector || !sector->items)
		return ;
	i = 0;
	while (i < sector->n_it)
	{
		p = sector->items[i].p;
		if (within(p.x, -1, GRID_SIZE) && within(p.y, -1, GRID_SIZE) &&
		grid->nod[p.x][p.y] == NODE_EMPTY)
			grid->nod[p.x][p.y] = (signed char)(-10 - i);
		else
			delete_item(sector, i--);
		i++;
	}
}
