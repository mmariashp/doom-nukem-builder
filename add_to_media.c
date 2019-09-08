
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

t_sector				*realloc_sectors(t_sector *sectors, int n)
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
		new[j] = sectors[j];
		j++;
	}
	free(sectors);
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

unsigned short			add_vector(t_vec2d **vertices, short n_vectors, t_t *t, short i)
{
	if (!t)
		return (FAIL);
	*vertices = realloc_vertices(*vertices, n_vectors + 1);
	if (!*vertices)
		return (FAIL);
	(*vertices)[n_vectors] = t->active[i];
	t->grid.nodes[t->active[i].x][t->active[i].y] = NODE_FULL;
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

unsigned short			add_sector(t_sector **sectors, short n_sectors)
{
	*sectors = realloc_sectors(*sectors, n_sectors + 1);
	if (!*sectors)
		return (FAIL);
	(*sectors)[n_sectors].sec_walls = NULL;
	(*sectors)[n_sectors].floor = 0;
	(*sectors)[n_sectors].ceiling = 0;
	(*sectors)[n_sectors].floor_txtr = 0;
	(*sectors)[n_sectors].ceil_txtr = 0;
	(*sectors)[n_sectors].n_walls = 0;
	return (SUCCESS);
}

unsigned short			add_wall(t_wall **walls, short n_walls, int one, int two)
{
	*walls = realloc_walls(*walls, n_walls + 1);
	if (!*walls)
		return (FAIL);
	(*walls)[n_walls].type = WALL_FILLED;
	(*walls)[n_walls].v1 = one;
	(*walls)[n_walls].v2 = two;
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

unsigned short			add_world(t_world **worlds, short n_worlds)
{
	*worlds = realloc_worlds(*worlds, n_worlds + 1);
	if (!*worlds)
		return (FAIL);
	(*worlds)[n_worlds].filename = ft_strdup("F.txt");
	(*worlds)[n_worlds].full_path = ft_strdup("./media/maps/");
	(*worlds)[n_worlds].textures = NULL;
	(*worlds)[n_worlds].n_textures = 0;
	(*worlds)[n_worlds].sectors = NULL;
	(*worlds)[n_worlds].n_sectors = 0;
	(*worlds)[n_worlds].walls = NULL;
	(*worlds)[n_worlds].n_walls = 0;
	(*worlds)[n_worlds].vertices = NULL;
	(*worlds)[n_worlds].n_vectors = 0;
	return (SUCCESS);
}

void					add_to_media(t_t *t, t_media *media)
{
	static short 		first_vector = -1;
	static short 		sector = -1;
	static short 		last_id = -1;
	short 				wall;
	short 				id;
	short 				done;

	if (!media || !t)
		return ;
	id = -1;
	done = FALSE;
	wall = -1;
	if (t->active[1].x == -1)
	{
		if (t->grid.nodes[t->active[0].x][t->active[0].y] == NODE_FULL)
			id = find_vector(media->worlds[media->world_id].vertices, t->active[0], media->worlds[media->world_id].n_vectors);
		if (id == -1)
		{
			if (add_vector(&media->worlds[media->world_id].vertices, media->worlds[media->world_id].n_vectors, t, 0) == FAIL)
				return ;
			id = media->worlds[media->world_id].n_vectors++;
		}
		if (add_sector(&media->worlds[media->world_id].sectors, media->worlds[media->world_id].n_sectors) == FAIL)
			return ;
		sector = media->worlds[media->world_id].n_sectors++;
		if (add_sector_v(&media->worlds[media->world_id].sectors[sector].v, media->worlds[media->world_id].sectors[sector].n_walls, id) == FAIL)
			return ;
		last_id = id;
		first_vector = id;
	}
	else
	{
		if (t->grid.nodes[t->active[1].x][t->active[1].y] == NODE_FULL)
			id = find_vector(media->worlds[media->world_id].vertices, t->active[1], media->worlds[media->world_id].n_vectors);
		if (id == -1)
		{
			if (add_vector(&media->worlds[media->world_id].vertices, media->worlds[media->world_id].n_vectors, t, 1) == FAIL)
				return ;
			id = media->worlds[media->world_id].n_vectors++;
		}
		else
		{
			if (id == first_vector)
				done = TRUE;
			else if (t->grid.nodes[media->worlds[media->world_id].vertices[last_id].x][media->worlds[media->world_id].vertices[last_id].y] == NODE_FULL &&
					 t->grid.nodes[media->worlds[media->world_id].vertices[id].x][media->worlds[media->world_id].vertices[id].y] == NODE_FULL)
				wall = find_wall(last_id, id, media->worlds[media->world_id].walls, media->worlds[media->world_id].n_walls);
		}
		if (wall == -1)
		{
			if (add_wall(&media->worlds[media->world_id].walls, media->worlds[media->world_id].n_walls, last_id, id) == FAIL)
				return ;
			wall = media->worlds[media->world_id].n_walls++;
		}
		if (add_secwall(&media->worlds[media->world_id].sectors[sector].sec_walls, media->worlds[media->world_id].sectors[sector].n_walls, wall) == FAIL)
			return ;
		media->worlds[media->world_id].sectors[sector].n_walls++;
		if (add_sector_v(&media->worlds[media->world_id].sectors[sector].v, media->worlds[media->world_id].sectors[sector].n_walls, id) == FAIL)
			return ;
		t->active[0] = t->active[1];
		t->active[1] = (t_vec2d){ -1, -1 };
		last_id = id;
	}
	if (done == TRUE)
	{
		t->active[0] = (t_vec2d){ -1, -1 };
		t->active[1] = (t_vec2d){ -1, -1 };
		last_id = -1;
	}
}