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
	while (i < world->n_w)
	{
		if (within(world->walls[i].txtr, -1, n) == FALSE)
			world->walls[i].txtr = 0;
		i++;
	}
	i = 0;
	while (i < world->n_s)
	{
		if (within(world->sec[i].fl_txtr, -1, n) == FALSE)
			world->sec[i].fl_txtr = 0;
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
	while (i < world->n_s)
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

	if (!world || !within(to_replace, -1, world->n_v) || !within(new, -1, world->n_v))
		return ;
	i = 0;
	while (i < world->n_w)
	{
		if (world->walls[i].v1 == to_replace)
			world->walls[i].v1 = new;
		i++;
	}
	i = 0;
	while (i < world->n_s)
	{
		j = 0;
		while (j < world->sec[i].n_w)
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

	if (!world || !within(to_replace, -1, world->n_w) || !within(new, -1, world->n_w))
		return ;
	i = 0;
	while (i < world->n_s)
	{
		j = 0;
		while (j < world->sec[i].n_w)
		{
			if (world->sec[i].s_walls[j] == to_replace)
				world->sec[i].s_walls[j] = new;
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
	while (i < world->n_v)
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
	while (i < world->n_v)
	{
		j = 0;
		used = FALSE;
		while (j < world->n_w)
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
	while (i < world->n_w)
	{
		j = 0;
		used = FALSE;
		while (j < world->n_s && used == FALSE)
		{
			k = 0;
			while (k < world->sec[j].n_w && used == FALSE)
			{
				if (world->sec[j].s_walls[k] == i)
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
	while (i < world->n_w)
	{
		j = 0;
		while (j < i)
		{
			if ((world->walls[i].v1 == world->walls[j].v2 && world->walls[i].v2 == world->walls[j].v1) ||
				(world->walls[i].v1 == world->walls[j].v1 && world->walls[i].v2 == world->walls[j].v2))
			{
				replace_wall(i, j, world);
				delete_wall(i, world);
				i--;
				break ;
			}
			j++;
		}
		i++;
	}
}

int						*reverse_order(int *p, int n_p)
{
	int 				*tab;
	int 				i;
	int 				j;

	if (p)
	{
		if (!(tab = (int *)ft_memalloc(sizeof(int) * n_p)))
			return (NULL);
		i = 0;
		j = n_p - 1;
		while (i < n_p)
			tab[j--] = p[i++];
		free(p);
		return (tab);
	}
	return (NULL);
}

//int						*reverse_order_walls(int *w, int n_w, t_wall *walls)
//{
//	int 				*tab;
//	int 				i;
//	int 				j;
//
//	if (w && walls)
//	{
//		if (!(tab = (int *)ft_memalloc(sizeof(int) * n_w)))
//			return (NULL);
//		i = 0;
//		j = n_w - 1;
//		while (i < n_w)
//			tab[j--] = w[i++];
//		free(w);
//		return (tab);
//	}
//	return (NULL);
//}

void					validate_clockwise(t_world *world, int sec)
{
	int 				i;
	int 				sum;
	t_vec2d				one;
	t_vec2d				two;

	if (!world)
		return ;
	i = 0;
	sum = 0;
	while (i < world->sec[sec].n_v)
	{
		one = world->vecs[world->sec[sec].v[i]];
		two = world->vecs[world->sec[sec].v[(i + 1) % world->sec[sec].n_v]];
		sum += (two.x - one.x) * (one.y + two.y);
		i++;
	}
	if (sum > 0)
	{
		ft_putstr("\033[1;31m");
		printf("SEC %d counter clockwise\n", sec);
		ft_putstr("\x1b[0m");
		world->sec[sec].s_walls = reverse_order(world->sec[sec].s_walls, world->sec[sec].n_w);
		free(world->sec[sec].v);
		get_sec_v(&world->sec[sec], world->walls);
	}
}

void					validate_sectors(t_world *world)
{
	int 				i;
	int 				control;

	if (!world)
		return ;
	upd_sec(world->sec, world->walls, world->vecs, world->n_s);
	delete_double_v(world);
	delete_double_walls(world);
	delete_unused_walls(world);
	delete_unused_v(world);
	i = 0;
	control = 0;
	while (i < world->n_s && control < MAX_N_SECS)
	{
		if (world->sec[i].status == SEC_OPEN)
		{
			ft_putstr("\033[1;31m");
			printf("deleting SEC %d bec open\n", i);
			ft_putstr("\x1b[0m");
			delete_sector(i, world);
		}
		else
		{
			validate_clockwise(world, i);
			i++;
		}
		control++;
		i++;
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
		ft_putstr("\033[1;35m");
		printf("level %d validation \n", i);
		ft_putstr("\x1b[0m");
		validate_textures(&media->worlds[i], media->n_txtrs);
		validate_items(&media->worlds[i], media->n_itemfull);
		validate_sectors(&media->worlds[i]);
		i++;
	}
}