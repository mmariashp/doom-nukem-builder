
#include "builder.h"

unsigned short			add_vec(t_vec2d **vecs, short n_vecs, t_grid *grid, short i)
{
	if (!grid)
		return (FAIL);
	*vecs = (t_vec2d *)realloc_tab(*vecs, sizeof(t_vec2d) * (n_vecs + 1), sizeof(t_vec2d) * n_vecs);
	if (!*vecs)
		return (FAIL);
	(*vecs)[n_vecs] = grid->p[i];
	grid->nodes[grid->p[i].x][grid->p[i].y] = NODE_FULL;
	return (SUCCESS);
}

unsigned short			add_sector_v(int **v, short n_v, int id)
{
	*v = (int *)realloc_tab(*v, sizeof(int) * (n_v + 1), sizeof(int) * n_v);
	if (!*v)
		return (FAIL);
	(*v)[n_v] = id;
	return (SUCCESS);
}

unsigned short			add_sector(t_sec **sec, short n_sec)
{
	*sec = (t_sec *)realloc_tab(*sec, sizeof(t_sec) * (n_sec + 1), sizeof(t_sec) * n_sec);
	if (!*sec)
		return (FAIL);
	(*sec)[n_sec].s_walls = NULL;
	(*sec)[n_sec].items = NULL;
	(*sec)[n_sec].fl = 0;
	(*sec)[n_sec].ceiling = 20;
	(*sec)[n_sec].fl_txtr = 0;
	(*sec)[n_sec].ceil_txtr = 0;
	(*sec)[n_sec].n_walls = 0;
	(*sec)[n_sec].n_items = 0;
    (*sec)[n_sec].n_v = 0;
	return (SUCCESS);
}

unsigned short			add_wall(t_wall **walls, short n_walls, int one, int two)
{
	*walls = (t_wall *)realloc_tab(*walls, sizeof(t_wall) * (n_walls + 1), sizeof(t_wall) * n_walls);
	if (!*walls)
		return (FAIL);
	(*walls)[n_walls].type = WALL_FILLED;
    (*walls)[n_walls].txtr = 0;
	(*walls)[n_walls].v1 = one;
	(*walls)[n_walls].v2 = two;
	return (SUCCESS);
}

unsigned short			add_secwall(int **secwalls, short n_secwalls, int wall)
{
	if (n_secwalls > 0 && !*secwalls)
		return (FAIL);
	*secwalls = (int *)realloc_tab(*secwalls, sizeof(int) * (n_secwalls + 1), sizeof(int) * n_secwalls);
	if (!*secwalls)
		return (FAIL);
	(*secwalls)[n_secwalls] = wall;
	return (SUCCESS);
}

char 					*get_filename(int i)
{
	char 				*nb;
	char 				*name;

	nb = ft_itoa(i);
	if (nb)
	{
		name = ft_strjoin("new_level_", nb);
		free(nb);
		if (name)
		{
			return (name);
		}
	}
	return (NULL);
}

unsigned short			add_world(t_world **worlds, short n_worlds, char *ext, char *path)
{
	static int i = 0;
	*worlds = (t_world *)realloc_tab(*worlds, sizeof(t_world) * (n_worlds + 1), sizeof(t_world) * n_worlds);
	if (!*worlds || !path || !ext)
		return (FAIL);
	(*worlds)[n_worlds].filename = get_filename(i);
	(*worlds)[n_worlds].full_path = get_full_path((*worlds)[n_worlds].filename, ext, path);
	if (!(*worlds)[n_worlds].filename || !(*worlds)[n_worlds].full_path)
		return (FAIL);
	(*worlds)[n_worlds].sec = NULL;
	(*worlds)[n_worlds].n_sec = 0;
	(*worlds)[n_worlds].walls = NULL;
	(*worlds)[n_worlds].n_walls = 0;
	(*worlds)[n_worlds].vecs = NULL;
	(*worlds)[n_worlds].n_vecs = 0;
	(*worlds)[n_worlds].p_start = (t_vec2d){ 1, 1 };
	(*worlds)[n_worlds].p_end = (t_vec2d){ 1, 3 };
	i++;
	return (SUCCESS);
}

unsigned short			already_in_sector(int id, int *vecs,int n_vecs)
{
	int 				i;

	if (!vecs)
		return (FALSE);
	i = 0;
	while (i < n_vecs)
	{
		if (vecs[i++] == id)
			return (TRUE);
	}
	return (FALSE);
}

unsigned short			can_place_vec(t_vec2d v, t_world world)
{
//	unsigned short		i;
//	int 				j;

	if (!within(v.x, -1, GRID_SIZE) || !within(v.y, -1, GRID_SIZE))
	{
		printf("can't place because outside of grid\n");
		return (FALSE);
	}
	if (node_in_sector(v, &world) != -1)
	{
		printf("can't place because in sector\n");
		return (FALSE);
	}
	if (find_wall_inter(v, world) != -1)
	{
		printf("can't place because intersection\n");
		return (FALSE);
	}
	return (TRUE);
}

void					add_to_media(t_grid *grid, t_world *world)
{
	static short 		first_vector = -1;
	static short 		sector = -1;
	static short 		last_id = -1;
	short 				wall;
	short 				id;
	short 				done;

	if (!world || !grid)
		return ;
	id = -1;
	done = FALSE;
	wall = -1;
	if (grid->p[1].x == -1)
	{
		if (!can_place_vec(grid->p[0], *world))
		{
			grid->p[0] = (t_vec2d){ -1, -1 };
			return ;
		}
		if (grid->nodes[grid->p[0].x][grid->p[0].y] == NODE_EMPTY ||
		(grid->nodes[grid->p[0].x][grid->p[0].y] == NODE_FULL &&
		(id = find_vec(world->vecs, grid->p[0], world->n_vecs)) == -1))
		{
			if (add_vec(&world->vecs, world->n_vecs, grid, 0) == FAIL)
				return ;
			id = world->n_vecs++;
		}
		if (id == -1)
		{
			grid->p[0] = (t_vec2d){ -1, -1 };
			return ;
		}
		if (add_sector(&world->sec, world->n_sec) == FAIL)
			return ;
		sector = world->n_sec++;
		if (add_sector_v(&world->sec[sector].v, world->sec[sector].n_v, id) == FAIL)
			return ;
		world->sec[sector].n_v++;
		last_id = id;
		first_vector = id;
	}
	else if (within(sector, -1, world->n_sec))
	{
		if (grid->nodes[grid->p[1].x][grid->p[1].y] == NODE_FULL)
			id = find_vec(world->vecs, grid->p[1], world->n_vecs);
		if (id != first_vector && !can_place_vec(grid->p[1], *world))
		{
			grid->p[1] = (t_vec2d){ -1, -1 };
			return ;
		}
		if (id == -1 && grid->nodes[grid->p[1].x][grid->p[1].y] == NODE_EMPTY)
		{
			if (add_vec(&world->vecs, world->n_vecs, grid, 1) == FAIL)
				return ;
			id = world->n_vecs++;
		}
		else
		{
			if (id == first_vector)
				done = TRUE;
			else if (id == -1 || already_in_sector(id, world->sec[sector].v, world->sec[sector].n_v))
			{
				grid->p[1] = (t_vec2d){ -1, -1 };
				return ;
			}
			else if (grid->nodes[world->vecs[last_id].x][world->vecs[last_id].y] == NODE_FULL &&
					 grid->nodes[world->vecs[id].x][world->vecs[id].y] == NODE_FULL)
				wall = find_wall(last_id, id, world->walls, world->n_walls);
		}
		if (wall == -1)
		{
			if (add_wall(&world->walls, world->n_walls, last_id, id) == FAIL)
				return ;
			wall = world->n_walls++;
		}
		if (add_secwall(&world->sec[sector].s_walls, world->sec[sector].n_walls, wall) == FAIL)
			return ;
		world->sec[sector].n_walls++;
		if (add_sector_v(&world->sec[sector].v, world->sec[sector].n_v, id) == FAIL)
			return ;
		world->sec[sector].n_v++;
		grid->p[0] = grid->p[1];
		grid->p[1] = (t_vec2d){ -1, -1 };
		last_id = id;
	}
	else
		return ;
	if (done == TRUE)
	{
		vec_set(grid->p, -1, -1, 2);
		last_id = -1;
		sector = -1;
		first_vector = -1;
	}
}

