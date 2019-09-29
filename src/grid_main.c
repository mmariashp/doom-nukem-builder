/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   grid_main.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mshpakov <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/09/28 16:45:37 by mshpakov          #+#    #+#             */
/*   Updated: 2019/09/28 16:45:38 by mshpakov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "builder.h"

void					clean_grid(t_grid *grid)
{
	int					i;
	int					j;

	if (!grid)
		return ;
	i = 0;
	while (i < GRID_SIZE)
	{
		j = 0;
		while (j < GRID_SIZE)
			grid->nod[i][j++] = NODE_EMPTY;
		i++;
	}
}

t_grid					*get_grid(void)
{
	t_grid					*grid;

	if (!(grid = (t_grid *)ft_memalloc(sizeof(t_grid))))
		return (NULL);
	grid->scl = (float)W_H / GRID_SIZE;
	grid->box.w = GRID_SIZE * grid->scl;
	grid->box.h = GRID_SIZE * grid->scl;
	grid->box.x = (W_W - grid->box.w) / 2;
	grid->box.y = (W_H - grid->box.h) / 2;
	grid->p[0] = (t_vec){ -1, -1 };
	grid->p[1] = (t_vec){ -1, -1 };
	clean_grid(grid);
	return (grid);
}

void					grid_re(t_grid *grid, t_media *media, int state,
																	int sector)
{
	if (!grid || !media || !media->worlds || !media->worlds[media->w].sec)
		return ;
	clean_grid(grid);
	fill_grid_walls(&media->worlds[media->w], grid);
	fill_grid(media->worlds[media->w].n_v,\
	media->worlds[media->w].vecs, grid);
	if (state == SEC_EDIT && within(sector, -1, media->worlds[media->w].n_s))
		fill_grid_items(&media->worlds[media->w].sec[sector], grid);
}
