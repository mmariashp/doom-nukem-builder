
#include "builder.h"

short 					find_vector(t_vec2d *vecs, t_vec2d p, int n)
{
	short 				i;

	if (!vecs)
		return (-1);
	i = 0;
	while (i < n)
	{
		if (vecs[i].x == p.x && vecs[i].y == p.y)
			return (i);
		i++;
	}
	return (-1);
}

short 					find_wall(short one, short two, t_wall *walls, short n_walls)
{
	short 				i;

	i = 0;
	if (!walls)
		return (-1);
	while (i < n_walls)
	{
		if (one == walls[i].v1 && two == walls[i].v2)
			return (i);
		if (two == walls[i].v1 && one == walls[i].v2)
			return (i);
		i++;
	}
	return (-1);
}