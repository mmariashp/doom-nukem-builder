
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
			new[++i] = to_add;
		i++;
		j++;
	}
	free(old_array);
	*size = new_size;
	return (new);
}

unsigned short			remove_wall_in_secs(t_world *world, int to_remove)
{
	int 				i;
	int 				tmp;

	if (!world)
		return (FAIL);
	i = 0;
	tmp = 0;
	while (i < world->n_sec)
	{
		world->sec[i].sec_walls = remove_from_array(world->sec[i].sec_walls,\
		world->sec[i].n_walls, &tmp, to_remove);
		if (!world->sec[i].sec_walls)
			return (FAIL);
		world->sec[i].n_walls = tmp;
		i++;
	}
	return (SUCCESS);
}

unsigned short			add_wall_in_secs(t_world *world, int to_add, int find)
{
	int 				i;

	if (!world)
		return (FAIL);
	i = 0;
	while (i < world->n_sec)
	{
		world->sec[i].sec_walls = add_to_array(world->sec[i].sec_walls,\
		&world->sec[i].n_walls, to_add, find);
		if (!world->sec[i].sec_walls)
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

	if (!world || !within(id, -1, world->n_walls))
		return ;
	if (!(new = (t_wall *)ft_memalloc(sizeof(t_wall) * (world->n_walls - 1))))
		return ;
	i = 0;
	j = 0;
	while (j < world->n_walls && i < world->n_walls - 1)
	{
		if (j == id)
			j++;
		new[i++] = world->walls[j++];
	}
	world->n_walls--;
	free(world->walls);
	world->walls = new;
	remove_wall_in_secs(world, id);
	i = 0;
	while (i < world->n_sec)
	{
		j = 0;
		while (j < world->sec[i].n_walls)
		{
			if (world->sec[i].sec_walls[j] > id)
				world->sec[i].sec_walls[j]--;
			j++;
		}
		i++;
	}
}

