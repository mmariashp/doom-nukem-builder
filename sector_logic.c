
#include "builder.h"

unsigned short			dot_inside_sector(int x, int y, t_vec2d *p, int n)
{
	int 				i;
	int 				j;
	unsigned short		odd;

	i = -1;
	j = n - 1;
	odd = FALSE;
	while(++i < n)
	{
		if ((p[i].y < y && p[j].y >= y) || (p[j].y < y && p[i].y >= y) )
		{
			if (p[i].x + (float)(y - p[i].y) / (p[j].y - p[i].y) * (p[j].x - p[i].x) < x)
				odd = odd == FALSE ? TRUE : FALSE;
		}
		j = i;
	}
	return (odd);
}

int 					node_in_sector(t_vec2d grid_p, t_world *world)
{
	int 				id;
	t_vec2d				*v;
	int 				i;
	int 				j;

	if (!world)
		return (-1);
	id = -1;
	i = -1;
	while (++i < world->n_sec)
	{
		if (world->sec[i].status != SEC_OPEN)
		{
			if (!(v = ft_memalloc(sizeof(t_vec2d) * world->sec[i].n_v)))
				return (-1);
			j = -1;
			while (++j < world->sec[i].n_v)
			{
				if (!within(world->sec[i].v[j], -1, world->n_vecs))
				{
					free(v);
					return (-1);
				}
				v[j] = world->vecs[world->sec[i].v[j]];
			}
			if (dot_inside_sector(grid_p.x, grid_p.y, v, world->sec[i].n_v))
				id = i;
			free(v);
		}
	}
	return (id);
}

int 					mouse_in_sector(t_vec2d p, t_world *world, t_grid *grid)
{
	t_vec2d				map_p;

	if (!world || !grid || mouse_over(grid->box, p) == FALSE)
		return (-1);
	map_p = find_node(p.x, p.y, grid);
	return (node_in_sector(map_p, world));
}

unsigned short 			sec_is_convex(t_vec2d *vec, int *v, int n)
{
	char				pos_neg[2];
	t_vec2d_d			d[2];
	int					ijk[3];

	if (n < 4 || !vec || !v)
		return (TRUE);
	pos_neg[0] = FALSE;
	pos_neg[1] = FALSE;
	ijk[0] = -1;
	while (++ijk[0] < n)
	{
		if (v[ijk[0]] != -2)
		{
			d[1].x = vec[v[ijk[0]]].x - vec[v[(ijk[1] = ijk[0] + 1) % n]].x;
			d[1].y = vec[v[ijk[0]]].y - vec[v[(ijk[1]) % n]].y;
			d[0].x = vec[v[(ijk[2] = ijk[1] + 1) % n]].x - vec[v[ijk[1] % n]].x;
			d[0].y = vec[v[(ijk[2]) % n]].y - vec[v[ijk[1] % n]].y;
			d[0].x = d[0].x * d[1].y - d[0].y * d[1].x;
			pos_neg[0] = d[0].x > 0 ? TRUE : pos_neg[0];
			pos_neg[1] = d[0].x < 0 ? TRUE : pos_neg[1];
			if (pos_neg[0] == TRUE && pos_neg[1] == TRUE)
				return (FALSE);
		}
	}
	return (TRUE);
}
