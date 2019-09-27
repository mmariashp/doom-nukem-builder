
#include "builder.h"

t_vec2d					*get_point_at_d(t_vec2d origin, int dist, int slope)
{
	t_vec2d				*res;

	res = ft_memalloc(sizeof(t_vec2d) * 2);
	if (!res)
		return (NULL);
	if (!slope)
	{
		res[0] = (t_vec2d){ origin.x + dist, origin.y };
		res[1] = (t_vec2d){ origin.x - dist, origin.y };
	}
	else if (slope > 10000)
	{
		res[0] = (t_vec2d){ origin.x, origin.y + dist };
		res[1] = (t_vec2d){ origin.x, origin.y - dist };
	}
	else
	{
		res[0] = (t_vec2d){ origin.x + 1, origin.y + slope };
		res[1] = (t_vec2d){ origin.x - 1, origin.y - slope };
	}
	return ((t_vec2d *)res);
}

void					add_door(t_world *world, int wall, t_grid *grid)
{
	float				slope;
	t_vec2d				p[4];
	int 				v[4];
	t_vec2d				*new[2];

	if (!within(wall, -1, world->n_walls))
		return ;
	vec_set(p, -1, -1, 4);
	p[0] = world->vecs[world->walls[wall].v1];
	p[1] = world->vecs[world->walls[wall].v2];
	slope = get_perp_slope(p[0], p[1]);
	if (p[0].x == p[1].x || p[0].y == p[1].y || slope == 1 || slope == -1)
	{
		if ((new[0] = get_point_at_d(p[0], 1, (int)slope)))
		{
			if (node_in_sector(new[0][0], world) != -1 ||
				grid->nodes[new[0][0].x][new[0][0].y] != NODE_EMPTY || find_wall_inter(new[0][0], *world) != -1)
				new[0][0] = (t_vec2d){ -1, -1 };
			if (node_in_sector(new[0][1], world) != -1 ||
				grid->nodes[new[0][1].x][new[0][1].y] != NODE_EMPTY || find_wall_inter(new[0][1], *world) != -1)
				new[0][1] = (t_vec2d){ -1, -1 };
		}
		else
			return ;
		if ((new[1] = get_point_at_d(p[1], 1, (int)slope)))
		{
			if (node_in_sector(new[1][0], world) != -1 ||
				grid->nodes[new[1][0].x][new[1][0].y] != NODE_EMPTY || find_wall_inter(new[1][0], *world) != -1)
				new[1][0] = (t_vec2d){ -1, -1 };
			if (node_in_sector(new[1][1], world) != -1 ||
				grid->nodes[new[1][1].x][new[1][1].y] != NODE_EMPTY || find_wall_inter(new[1][1], *world) != -1)
				new[1][1] = (t_vec2d){ -1, -1 };

		}
		if (new[1])
		{
			unsigned short i = FALSE;
			int j;
			if (new[0][0].x != -1 && new[1][0].x != -1)
			{
				j = 0;
				while (j < world->n_walls)
				{
					t_vec2d v1 = world->vecs[world->walls[j].v1];
					t_vec2d v2 = world->vecs[world->walls[j].v2];
					lines_intersect((t_line){ new[0][0], new[1][0] }, (t_line){ v1, v2 }, &i);
					if (i == TRUE)
					{
						printf("opt 0 intersects!\n");
						break ;
					}
					j++;
				}
				if (i == FALSE)
				{
					p[2] = new[0][0];
					p[3] = new[1][0];
				}

			}
			i = FALSE;
			if (new[0][1].x != -1 && new[1][1].x != -1)
			{
				j = 0;
				while (j < world->n_walls)
				{
					t_vec2d v1 = world->vecs[world->walls[j].v1];
					t_vec2d v2 = world->vecs[world->walls[j].v2];
					lines_intersect((t_line){ new[0][1], new[1][1] }, (t_line){ v1, v2 }, &i);
					if (i == TRUE)
					{
						printf("opt 1 intersects!\n");
						break ;
					}
					j++;
				}
				if (i == FALSE)
				{
					p[2] = new[0][1];
					p[3] = new[1][1];
				}
			}
			free(new[1]);
		}
		free(new[0]);
		if (p[2].x == -1 || p[3].x == -1)
			return ;
		if (add_sector(&world->sec, world->n_sec) == FAIL)
			return ;
		world->sec[world->n_sec].is_door = TRUE;
		world->sec[world->n_sec].n_walls = 4;
		world->sec[world->n_sec].n_v = 4;
		if (!(world->sec[world->n_sec].s_walls = ft_memalloc(sizeof(int) * 4)))
			return ;
		world->sec[world->n_sec].s_walls[0] = wall;
		grid->p[0] = p[2];
		grid->p[1] = p[3];
		if (add_vec(&world->vecs, (v[2] = world->n_vecs++), grid, 0) == SUCCESS
		&& add_vec(&world->vecs, (v[3] = world->n_vecs++), grid, 1) == SUCCESS
		&& add_wall(&world->walls, (world->sec[world->n_sec].s_walls[1] = \
		world->n_walls++), (v[1] = world->walls[wall].v2), v[3]) == SUCCESS &&
		add_wall(&world->walls, (world->sec[world->n_sec].s_walls[2] = \
		world->n_walls++), v[3], v[2]) == SUCCESS && add_wall(&world->walls, \
		(world->sec[world->n_sec].s_walls[3] = world->n_walls++), v[2], \
		(v[0] = world->walls[wall].v1)) == SUCCESS)
			get_sec_v(&world->sec[world->n_sec++], world->walls);

	}
}