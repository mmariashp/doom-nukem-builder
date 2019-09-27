/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   delete_sector.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mshpakov <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/09/23 15:21:48 by mshpakov          #+#    #+#             */
/*   Updated: 2019/09/23 15:21:50 by mshpakov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "builder.h"

void					free_sector(t_sec *sector)
{
	if (!sector)
		return ;
	if (sector->s_walls)
		free(sector->s_walls);
	sector->s_walls = NULL;
	if (sector->v)
		free(sector->v);
	sector->v = NULL;
	if (sector->items)
		free(sector->items);
	sector->items = NULL;
}

void					copy_sector(t_sec *new, t_sec *old)
{
	int 				i;

	if (!old || !new)
		return ;
	new->s_walls = ft_memalloc(sizeof(int) *  old->n_walls);
	new->v = ft_memalloc(sizeof(int) *  old->n_v);
	new->items = ft_memalloc(sizeof(t_item) *  old->n_items);
	i = -1;
	while (new->s_walls && ++i < old->n_walls)
		new->s_walls[i] = old->s_walls[i];
	i = -1;
	while (new->v && ++i < old->n_v)
		new->v[i] = old->v[i];
	i = -1;
	while (new->items && ++i < old->n_items)
		new->items[i] = old->items[i];
	new->fl      = old->fl;
	new->ceiling    = old->ceiling;
	new->fl_txtr = old->fl_txtr;
	new->ceil_txtr  = old->ceil_txtr;
	new->n_items    = old->n_items;
	new->n_walls    = old->n_walls;
	new->n_v        = old->n_v;
	new->status     = old->status;
	free_sector(old);
}

void					delete_sector(int id, t_world *world)
{
	t_sec				*new;
	int 				i;
	int 				j;

	if (!world || !world->sec || id < 0 || id >= world->n_sec || !(new = \
	(t_sec *)ft_memalloc(sizeof(t_sec) * (world->n_sec - 1))))
		return ;
	i = 0;
	j = 0;
	while (j < world->n_sec)
	{
		if (j == id)
			free_sector(&world->sec[j++]);
		if (j < world->n_sec)
			copy_sector(&new[i++], &world->sec[j++]);
	}
	free(world->sec);
	world->sec = new;
	world->n_sec--;
}
