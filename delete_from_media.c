
#include "builder.h"



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

