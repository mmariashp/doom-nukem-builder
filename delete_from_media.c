
#include "builder.h"

void					delete_vector(int id, t_world *world);
void					delete_sector(int id, t_world *world);

void					delete_vector(int id, t_world *world)
{
	t_vec2d				*new;
	int 				i;
	int 				j;

	if (!world || !world->vecs || id < 0 || id >= world->n_vecs ||
	!(new = (t_vec2d *)ft_memalloc(sizeof(t_vec2d) * (world->n_vecs - 1))))
		return ;
	i = 0;
	j = 0;
	while (j < world->n_vecs)
	{
		if (j == id)
			j++;
		if (j < world->n_vecs)
			new[i++] = world->vecs[j++];
	}
	world->n_vecs--;
	free(world->vecs);
	world->vecs = new;
	i = -1;
	while (++i < world->n_walls)
	{
		if (world->walls[i].v1 > id)
			world->walls[i].v1--;
		if (world->walls[i].v2 > id)
			world->walls[i].v2--;
	}
	i = 0;
	while (i < world->n_sec)
	{
		j = -1;
		while (++j < world->sec[i].n_walls)
		{
			if (world->sec[i].v[j] > id)
				world->sec[i].v[j]--;
		}
		i++;
	}
}

void					copy_sector(t_sec *new, t_sec *old)
{
	int 				i;

	if (!old || !new)
		return ;
	new->sec_walls = ft_memalloc(sizeof(int) *  old->n_walls);
	new->v = ft_memalloc(sizeof(int) *  old->n_v);
	new->items = ft_memalloc(sizeof(t_item) *  old->n_items);
	i = -1;
	while (new->sec_walls && ++i < old->n_walls)
		new->sec_walls[i] = old->sec_walls[i];
	i = -1;
	while (new->v && ++i < old->n_v)
		new->v[i] = old->v[i];
	i = -1;
	while (new->items && ++i < old->n_items)
		new->items[i] = old->items[i];
	new->floor      = old->floor;
	new->ceiling    = old->ceiling;
	new->floor_txtr = old->floor_txtr;
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