
#include "builder.h"

//int                     wall_door(int id, t_wall *walls, int n_walls) // finding the index of a corresponding door to a wall of index id, -1 if none
//{
//	int                 i;
//	t_wall              wall;
//
//	i = 0;
//	if (id >= 0 && id < n_walls)
//		wall = walls[id];
//	else
//		return (-1);
//	while (i < n_walls)
//	{
//		if (id != i && ((wall.v[0] == walls[i].v[0] && wall.v[1] == walls[i].v[1]) ||
//						(wall.v[0] == walls[i].v[1] && wall.v[1] == walls[i].v[0])))
//			return (i);
//		i++;
//	}
//	return (-1);
//}
//
//void                    delete_door(t_world *world, int id)
//{
//	int                 door;
//
//	if (id < 0 || id >= world->n_walls)
//		return ;
//	door = wall_door(id, world->walls, world->n_walls);
//	if (door == -1)
//		return ;
//	delete_wall(door, world);
//	world->walls[id].door = -1;
//}

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

float 					get_perp_slope(t_vec2d p1, t_vec2d p2)
{
	float 				slope;

	if (p2.x == p1.x)
		return (0);
	slope = (float)(p2.y - p1.y) / (p2.x - p1.x);
	if (slope == 0)
		return (20000);
	return (-1 / slope);
}

unsigned short			intersects(t_vec2d p, t_vec2d l1, t_vec2d l2)
{
	float 				slope;
	float 				intercept;


	if ((l2.x == l1.x && p.x == l2.x) || (l2.y == l1.y && p.y == l2.y))
	{
		if (p.x <= get_max(l1.x, l2.x) && p.x >= get_min(l1.x, l2.x) &&
			p.y <= get_max(l1.y, l2.y) && p.y >= get_min(l1.y, l2.y))
			return (TRUE);
	}
	if (l2.x != l1.x && l2.y != l1.y)
	{
		slope = (float)(l2.y - l1.y) / (l2.x - l1.x);
		intercept = l1.y - slope * l1.x;
		if (p.x * slope + intercept == p.y)
		{
			if (p.x <= get_max(l1.x, l2.x) && p.x >= get_min(l1.x, l2.x) &&
				p.y <= get_max(l1.y, l2.y) && p.y >= get_min(l1.y, l2.y))
				return (TRUE);
		}
	}
	return (FALSE);
}

int						find_wall_inter(t_vec2d p, t_world world)
{
	int 				i;

	i = 0;
	while (i < world.n_walls)
	{
		if (intersects(p, world.vecs[world.walls[i].v1], world.vecs[world.walls[i].v1]))
			return (i);
		i++;
	}
	return (-1);
}

void					add_door(t_world *world, int wall, t_grid *grid)
{
	float				slope;
	t_vec2d				p[4];
	int 				v[4];
	int 				w[4];
	t_vec2d				*new;

	if (!within(wall, -1, world->n_walls))
		return ;
	p[0] = world->vecs[world->walls[wall].v1];
	p[1] = world->vecs[world->walls[wall].v2];
	p[2] = (t_vec2d){ -1, -1 };
	p[3] = (t_vec2d){ -1, -1 };
	slope = get_perp_slope(p[0], p[1]);
	if (p[0].x == p[1].x || p[0].y == p[1].y || slope == 1 || slope == -1)
	{
		if ((new = get_point_at_d(p[0], 1.f, slope)))
		{
			if (node_in_sector(new[0], world) == -1 &&
				grid->nodes[new[0].x][new[0].y] == NODE_EMPTY && find_wall_inter(new[0], *world) == -1)
				p[2] = new[0];
			if (node_in_sector(new[1], world) == -1 &&
				grid->nodes[new[1].x][new[1].y] == NODE_EMPTY && find_wall_inter(new[1], *world) == -1)
				p[2] = p[2].x != -1 ? (t_vec2d){ -1, -1 } : new[1];
			free(new);
		}
		else
			return ;
		if ((new = get_point_at_d(p[1], 1.f, slope)))
		{
			int sec;
			t_vec2d one = new[0];
			t_vec2d two = new[1];
			sec = node_in_sector(one, world);
			sec = node_in_sector(two, world);
			if (node_in_sector(new[0], world) == -1 &&
				grid->nodes[new[0].x][new[0].y] == NODE_EMPTY && find_wall_inter(new[0], *world) == -1)
				p[3] = new[0];
			if (node_in_sector(new[1], world) == -1 &&
				grid->nodes[new[1].x][new[1].y] == NODE_EMPTY && find_wall_inter(new[1], *world) == -1)
				p[3] = p[3].x != -1 ? (t_vec2d){ -1, -1 } : new[1];
			free(new);
		}
		else
			return ;
		if (p[2].x == -1 || p[3].x == -1)
			return ;
		if (add_sector(&world->sec, world->n_sec) == FAIL)
			return ;
		world->sec[world->n_sec].is_door = TRUE;
		world->sec[world->n_sec].n_walls = 4;
		world->sec[world->n_sec].n_v = 4;
		world->sec[world->n_sec].sec_walls = ft_memalloc(sizeof(int) * 4);
		v[0] = world->walls[wall].v1;
		v[1] = world->walls[wall].v2;
		w[0] = wall;
		grid->active[0] = p[2];
		if (add_vector(&world->vecs, (v[2] = world->n_vecs++), grid, 0) == FAIL)
			return ;
		grid->active[0] = p[3];
		if (add_vector(&world->vecs, world->n_vecs, grid, 0) == FAIL)
			return ;
		v[3] = world->n_vecs++;
		if (add_wall(&world->walls, world->n_walls, v[1], v[3]) == FAIL)
			return ;
		w[1] = world->n_walls++;
		if (add_wall(&world->walls, world->n_walls, v[3], v[2]) == FAIL)
			return ;
		w[2] = world->n_walls++;
		if (add_wall(&world->walls, world->n_walls, v[2], v[0]) == FAIL)
			return ;
		w[3] = world->n_walls++;
		world->sec[world->n_sec].sec_walls[0] = w[0];
		world->sec[world->n_sec].sec_walls[1] = w[1];
		world->sec[world->n_sec].sec_walls[2] = w[2];
		world->sec[world->n_sec].sec_walls[3] = w[3];
		get_sec_v(&world->sec[world->n_sec++], world->walls);
	}
}