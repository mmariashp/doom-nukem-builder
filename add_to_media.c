
#include "builder.h"

t_world					*realloc_worlds(t_world *world, int n)
{
	t_world			*new;
	int					j;

	j = 0;
	if (n == 0)
		return (NULL);
	if (!(new = (t_world *)malloc(sizeof(t_world) * n)))
		return (NULL);
	while (j < n - 1)
	{
		new[j] = world[j];
		j++;
	}
	free(world);
	return (new);
}

t_sector				*realloc_sec(t_sector *sec, int n)
{
	t_sector			*new;
	int					j;

	j = 0;
	if (n == 0)
		return (NULL);
	if (!(new = (t_sector *)malloc(sizeof(t_sector) * n)))
		return (NULL);
	while (j < n - 1)
	{
		new[j] = sec[j];
		j++;
	}
	free(sec);
	return (new);
}

int						*realloc_sector_v(int *v, int n)
{
	int					*new;
	int					j;

	j = 0;
	if (n == 0)
		return (NULL);
	if (!(new = (int *)malloc(sizeof(int) * n)))
		return (NULL);
	while (j < n - 1)
	{
		new[j] = v[j];
		j++;
	}
	if (n > 1 && v)
		free(v);
	return (new);
}

int						*realloc_secwalls(int *secwalls, int n)
{
	int					*new;
	int					j;

	j = 0;
	if (n == 0)
		return (NULL);
	if (!(new = (int *)malloc(sizeof(int) * n)))
		return (NULL);
	while (j < n - 1)
	{
		new[j] = secwalls[j];
		j++;
	}
	if (secwalls)
		free(secwalls);
	return (new);
}

t_wall					*realloc_walls(t_wall *walls, int n)
{
	t_wall				*new;
	int					j;

	j = 0;
	if (n == 0)
		return (NULL);
	if (!(new = (t_wall *)malloc(sizeof(t_wall) * n)))
		return (NULL);
	while (j < n - 1)
	{
		new[j] = walls[j];
		j++;
	}
	free(walls);
	return (new);
}

t_vec2d					*realloc_vertices(t_vec2d *vertices, int n)
{
	t_vec2d				*new;
	int					j;

	j = 0;
	if (n == 0)
		return (NULL);
	if (!(new = (t_vec2d *)malloc(sizeof(t_vec2d) * n)))
		return (NULL);
	while (j < n - 1)
	{
		new[j] = vertices[j];
		j++;
	}
	free(vertices);
	return (new);
}

unsigned short			add_vector(t_vec2d **vertices, short n_vectors, t_grid *grid, short i)
{
	if (!grid)
		return (FAIL);
	*vertices = realloc_vertices(*vertices, n_vectors + 1);
	if (!*vertices)
		return (FAIL);
	(*vertices)[n_vectors] = grid->active[i];
	grid->nodes[grid->active[i].x][grid->active[i].y] = NODE_FULL;
	return (SUCCESS);
}

unsigned short			add_sector_v(int **v, short n_v, int id)
{
	*v = realloc_sector_v(*v, n_v + 1);
	if (!*v)
		return (FAIL);
	(*v)[n_v] = id;
	return (SUCCESS);
}

unsigned short			add_sector(t_sector **sec, short n_sec)
{
	*sec = realloc_sec(*sec, n_sec + 1);
	if (!*sec)
		return (FAIL);
	(*sec)[n_sec].sec_walls = NULL;
	(*sec)[n_sec].items = NULL;
	(*sec)[n_sec].floor = 0;
	(*sec)[n_sec].ceiling = 0;
	(*sec)[n_sec].floor_txtr = 0;
	(*sec)[n_sec].ceil_txtr = 0;
	(*sec)[n_sec].n_walls = 0;
	(*sec)[n_sec].n_items = 0;
    (*sec)[n_sec].n_v = 0;
	return (SUCCESS);
}

unsigned short			add_wall(t_wall **walls, short n_walls, int one, int two)
{
	*walls = realloc_walls(*walls, n_walls + 1);
	if (!*walls)
		return (FAIL);
	(*walls)[n_walls].type = WALL_FILLED;
    (*walls)[n_walls].txtr = 0;
	(*walls)[n_walls].v1 = one;
	(*walls)[n_walls].v2 = two;
    (*walls)[n_walls].door = -1;
	return (SUCCESS);
}



unsigned short			add_secwall(int **secwalls, short n_secwalls, int wall)
{
	if (n_secwalls > 0 && !*secwalls)
		return (FAIL);
	*secwalls = realloc_secwalls(*secwalls, n_secwalls + 1);
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
	*worlds = realloc_worlds(*worlds, n_worlds + 1);
	if (!*worlds || !path || !ext)
		return (FAIL);
	(*worlds)[n_worlds].filename = get_filename(i);
	(*worlds)[n_worlds].full_path = get_full_path((*worlds)[n_worlds].filename, ext, path);
//	(*worlds)[n_worlds].textures = ft_memalloc(sizeof(int) * 1);
//	(*worlds)[n_worlds].n_txtrs = 1;
	if (!(*worlds)[n_worlds].filename || !(*worlds)[n_worlds].full_path)
		return (FAIL);
//	(*worlds)[n_worlds].textures[0] = 0;
	(*worlds)[n_worlds].sec = NULL;
	(*worlds)[n_worlds].n_sec = 0;
	(*worlds)[n_worlds].walls = NULL;
	(*worlds)[n_worlds].n_walls = 0;
	(*worlds)[n_worlds].vertices = NULL;
	(*worlds)[n_worlds].n_vectors = 0;
	(*worlds)[n_worlds].p_start = (t_vec2d){ 1, 1 };
	(*worlds)[n_worlds].p_end = (t_vec2d){ 1, 3 };
	i++;
	return (SUCCESS);
}

void					add_to_media(t_grid *grid, t_media *media)
{
	static short 		first_vector = -1;
	static short 		sector = -1;
	static short 		last_id = -1;
	short 				wall;
	short 				id;
	short 				done;

	if (!media || !grid)
		return ;
	id = -1;
	done = FALSE;
	wall = -1;
	if (grid->active[1].x == -1)
	{
		if (grid->nodes[grid->active[0].x][grid->active[0].y] == NODE_FULL)
			id = find_vector(media->worlds[media->world_id].vertices, grid->active[0], media->worlds[media->world_id].n_vectors);
		if (id == -1)
		{
			if (add_vector(&media->worlds[media->world_id].vertices, media->worlds[media->world_id].n_vectors, grid, 0) == FAIL)
				return ;
			id = media->worlds[media->world_id].n_vectors++;
		}
		if (add_sector(&media->worlds[media->world_id].sec, media->worlds[media->world_id].n_sec) == FAIL)
			return ;
		sector = media->worlds[media->world_id].n_sec++;
		if (add_sector_v(&media->worlds[media->world_id].sec[sector].v, media->worlds[media->world_id].sec[sector].n_v, id) == FAIL)
			return ;
        media->worlds[media->world_id].sec[sector].n_v++;
		last_id = id;
		first_vector = id;
	}
	else
	{
		if (grid->nodes[grid->active[1].x][grid->active[1].y] == NODE_FULL)
			id = find_vector(media->worlds[media->world_id].vertices, grid->active[1], media->worlds[media->world_id].n_vectors);
		if (id == -1)
		{
			if (add_vector(&media->worlds[media->world_id].vertices, media->worlds[media->world_id].n_vectors, grid, 1) == FAIL)
				return ;
			id = media->worlds[media->world_id].n_vectors++;
		}
		else
		{
			if (id == first_vector)
				done = TRUE;
			else if (grid->nodes[media->worlds[media->world_id].vertices[last_id].x][media->worlds[media->world_id].vertices[last_id].y] == NODE_FULL &&
					 grid->nodes[media->worlds[media->world_id].vertices[id].x][media->worlds[media->world_id].vertices[id].y] == NODE_FULL)
				wall = find_wall(last_id, id, media->worlds[media->world_id].walls, media->worlds[media->world_id].n_walls);
		}
		if (wall == -1)
		{
			if (add_wall(&media->worlds[media->world_id].walls, media->worlds[media->world_id].n_walls, last_id, id) == FAIL)
				return ;
			wall = media->worlds[media->world_id].n_walls++;
		}
		if (add_secwall(&media->worlds[media->world_id].sec[sector].sec_walls, media->worlds[media->world_id].sec[sector].n_walls, wall) == FAIL)
			return ;
		media->worlds[media->world_id].sec[sector].n_walls++;
		if (add_sector_v(&media->worlds[media->world_id].sec[sector].v, media->worlds[media->world_id].sec[sector].n_v, id) == FAIL)
			return ;
        media->worlds[media->world_id].sec[sector].n_v++;
		grid->active[0] = grid->active[1];
		grid->active[1] = (t_vec2d){ -1, -1 };
		last_id = id;
	}
	if (done == TRUE)
	{
		grid->active[0] = (t_vec2d){ -1, -1 };
		grid->active[1] = (t_vec2d){ -1, -1 };
		last_id = -1;
	}
}