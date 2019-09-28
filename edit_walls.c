/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   edit_walls.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mshpakov <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/09/28 20:12:22 by mshpakov          #+#    #+#             */
/*   Updated: 2019/09/28 20:12:23 by mshpakov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "builder.h"

int 					*remove_from_array(int *old_array, int size, int *new_size, int to_remove)
{
	int 				count;
	int 				i;
	int 				*new;

	count = 0;
	i = -1;
	if (!old_array || !new_size)
		return (NULL);
	while (++i < size)
	{
		if (old_array[i] != to_remove)
			count++;
	}
	if (!count)
		return (NULL);
	*new_size = count;
	if (!(new = (int *)malloc(sizeof(int) * (*new_size))))
		return (NULL);
	count = -1;
	i = -1;
	while (++i < *new_size && ++count < size)
	{
		while (count < size && old_array[count] == to_remove)
			count++;
		new[i] = old_array[count];
	}
	free(old_array);
	return (new);
}

int 					*add_to_array(int *old_array, unsigned short *size, int to_add, int find)
{
	int 				j;
	int 				i;
	int 				*new;
	int					new_size;

	if (!old_array || !size)
		return (NULL);
	i = 0;
	j = 0;
	while (i < *size)
	{
		if (old_array[i] == find)
			j++;
		i++;
	}
	if (j == 0)
		return (old_array);
	new_size = *size + 1;
	if (!(new = (int *)malloc(sizeof(int) * new_size)))
		return (NULL);
	i = 0;
	j = 0;
	while (i < new_size && j < *size)
	{
		new[i] = old_array[j];
		if (old_array[j] == find)
		{
			new[i] = to_add;
			new[++i] = find;
		}
		i++;
		j++;
	}
	free(old_array);
	*size = new_size;
	return (new);
}

unsigned short			remove_wall_in_ss(t_world *world, int to_remove)
{
	int 				i;
	int 				tmp;

	if (!world)
		return (FAIL);
	i = 0;
	tmp = 0;
	while (i < world->n_s)
	{
		world->sec[i].s_walls = remove_from_array(world->sec[i].s_walls,\
		world->sec[i].n_w, &tmp, to_remove);
		if (!world->sec[i].s_walls)
			return (FAIL);
		world->sec[i].n_w = tmp;
		i++;
	}
	return (SUCCESS);
}

unsigned short			add_wall_in_ss(t_world *world, int to_add, int find)
{
	int 				i;

	if (!world)
		return (FAIL);
	i = 0;
	while (i < world->n_s)
	{
		world->sec[i].s_walls = add_to_array(world->sec[i].s_walls,\
		&world->sec[i].n_w, to_add, find);
		if (!world->sec[i].s_walls)
			return (FAIL);
		i++;
	}
	return (SUCCESS);
}

void					delete_wall(int id, t_world *world)
{
	t_wall				*new;
	int 				i;
	int 				j;

	if (!world || !within(id, -1, world->n_w))
		return ;
	if (!(new = (t_wall *)ft_memalloc(sizeof(t_wall) * (world->n_w - 1))))
		return ;
	i = 0;
	j = 0;
	while (j < world->n_w && i < world->n_w - 1)
	{
		if (j == id)
			j++;
		new[i++] = world->walls[j++];
	}
	world->n_w--;
	free(world->walls);
	world->walls = new;
	remove_wall_in_ss(world, id);
	i = 0;
	while (i < world->n_s)
	{
		j = 0;
		while (j < world->sec[i].n_w)
		{
			if (world->sec[i].s_walls[j] > id)
				world->sec[i].s_walls[j]--;
			j++;
		}
		i++;
	}
}

