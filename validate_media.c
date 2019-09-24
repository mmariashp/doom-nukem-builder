/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   validate_media.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mshpakov <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/09/23 16:21:32 by mshpakov          #+#    #+#             */
/*   Updated: 2019/09/23 16:21:34 by mshpakov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "builder.h"

void 					validate_textures(t_world *world, int n)
{
	int 				i;

	if (!world)
		return ;
	i = 0;
	while (i < world->n_walls)
	{
		if (within(world->walls[i].txtr, -1, n) == FALSE)
			world->walls[i].txtr = 0;
		i++;
	}
	i = 0;
	while (i < world->n_sec)
	{
		if (within(world->sec[i].floor_txtr, -1, n) == FALSE)
			world->sec[i].floor_txtr = 0;
		if (within(world->sec[i].ceil_txtr, -1, n) == FALSE)
			world->sec[i].ceil_txtr = 0;
		i++;
	}
}

void					delete_item(t_sec *sector, int id)
{
	t_item				*items;
	int 				i;
	int 				j;

	if (!sector || !within(id, -1, sector->n_items) || !sector->items)
		return ;
	if (!(items = ft_memalloc(sizeof(t_item) * (sector->n_items - 1))))
		return ;
	i = 0;
	j = 0;
	while (i < sector->n_items)
	{
		if (i == id)
			i++;
		if (j < sector->n_items - 1 && i < sector->n_items)
			items[j] = sector->items[i];
		i++;
		j++;
	}
	free(sector->items);
	sector->items = items;
	sector->n_items--;
}

void 					validate_items(t_world *world, int n)
{
	int 				i;
	int 				j;
	int 				k;

	if (!world)
		return ;
	i = 0;
	while (i < world->n_sec)
	{
		j = 0;
		while (j < world->sec[i].n_items)
		{
			if (within(world->sec[i].items[j].id, -1, n) == FALSE)
				world->sec[i].items[j].id = 0;
			k = 0;
			while (k < j)
			{
				if (vec_same(world->sec[i].items[j].p, world->sec[i].items[k].p))
				{
					delete_item(&world->sec[i], j);
					j--;
					break ;
				}
				k++;
			}
			j++;
		}
		i++;
	}
}

void					replace_vector(int to_replace, int new, t_world *world)
{
	int 				i;
	int 				j;

	if (!world || !within(to_replace, -1, world->n_vecs) || !within(new, -1, world->n_vecs))
		return ;
	i = 0;
	while (i < world->n_walls)
	{
		if (world->walls[i].v1 == to_replace)
			world->walls[i].v1 = new;
		i++;
	}
	i = 0;
	while (i < world->n_sec)
	{
		j = 0;
		while (j < world->sec[i].n_walls)
		{
			if (world->sec[i].v[j] == to_replace)
				world->sec[i].v[j] = new;
			j++;
		}
		i++;
	}
}

void					replace_wall(int to_replace, int new, t_world *world)
{
	int 				i;
	int 				j;

	if (!world || !within(to_replace, -1, world->n_walls) || !within(new, -1, world->n_walls))
		return ;
	i = 0;
	while (i < world->n_sec)
	{
		j = 0;
		while (j < world->sec[i].n_walls)
		{
			if (world->sec[i].sec_walls[j] == to_replace)
				world->sec[i].sec_walls[j] = new;
			j++;
		}
		i++;
	}
}

void					delete_double_v(t_world *world)
{
	int					i;
	int 				j;

	i = 0;
	while (i < world->n_vecs)
	{
		j = 0;
		while (j < i)
		{
			if (vec_same(world->vecs[i], world->vecs[j]))
			{
				replace_vector(i, j, world);
				delete_vector(i, world);
				i--;
				break ;
			}
			j++;
		}
		i++;
	}
}

void					delete_unused_v(t_world *world)
{
	int					i;
	int 				j;
	unsigned short		used;

	i = 0;
	while (i < world->n_vecs)
	{
		j = 0;
		used = FALSE;
		while (j < world->n_walls)
		{
			if (world->walls[j].v1 == i || world->walls[j].v2 == i)
			{
				used = TRUE;
				break ;
			}
			j++;
		}
		if (used == FALSE)
			delete_vector(i, world);
		else
			i++;
	}
}

void					delete_unused_walls(t_world *world)
{
	int					i;
	int 				j;
	int 				k;
	unsigned short		used;

	i = 0;
	while (i < world->n_walls)
	{
		j = 0;
		used = FALSE;
		while (j < world->n_sec && used == FALSE)
		{
			k = 0;
			while (k < world->sec[j].n_walls && used == FALSE)
			{
				if (world->sec[j].sec_walls[k] == i)
					used = TRUE;
				k++;
			}
			j++;
		}
		if (used == FALSE)
			delete_wall(i, world);
		else
			i++;
	}
}

void					delete_double_walls(t_world *world)
{
	int					i;
	int 				j;

	i = 0;
	while (i < world->n_walls)
	{
		j = 0;
		while (j < i)
		{
			if ((world->walls[i].v1 == world->walls[j].v2 && world->walls[i].v2 == world->walls[j].v1) ||
				(world->walls[i].v1 == world->walls[j].v1 && world->walls[i].v2 == world->walls[j].v2))
			{
				if (!((world->walls[i].type == WALL_EMPTY || world->walls[j].type == WALL_EMPTY) &&
					  (world->walls[i].type == WALL_DOOR || world->walls[j].type == WALL_DOOR)))
				{
					replace_wall(i, j, world);
					delete_wall(i, world);
					i--;
					break ;
				}
			}
			j++;
		}
		i++;
	}
}

void					validate_sectors(t_world *world)
{
	int 				i;
	int 				control;

	if (!world)
		return ;
	update_sector_status(world->sec, world->walls, world->vecs, world->n_sec);
	delete_double_v(world);
	delete_double_walls(world);
	delete_unused_walls(world);
	delete_unused_v(world);
	i = 0;
	control = 0;
	while (i < world->n_sec && control < MAX_N_SECTORS)
	{
		if (world->sec[i].status == SEC_OPEN)
			delete_sector(i, world);
		else
			i++;
		control++;
	}
}

void					validate_media(t_media *media)
{
	int 				i;

	if (!media)
		return ;
	i = 0;
	while (i < media->n_worlds)
	{
		validate_textures(&media->worlds[i], media->n_txtrs);
		validate_items(&media->worlds[i], media->n_itemfull);
		validate_sectors(&media->worlds[i]);
		i++;
	}
}