
#include "builder.h"

unsigned short			add_vector(t_vec2d **vecs, short n_vecs, t_grid *grid, short i)
{
	if (!grid)
		return (FAIL);
	*vecs = (t_vec2d *)realloc_tab(*vecs, sizeof(t_vec2d) * (n_vecs + 1), sizeof(t_vec2d) * n_vecs);
	if (!*vecs)
		return (FAIL);
	(*vecs)[n_vecs] = grid->active[i];
	grid->nodes[grid->active[i].x][grid->active[i].y] = NODE_FULL;
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

void					add_to_media(t_grid *grid, t_media *media)
{
	static short 		first_vector = -1;
	static short 		sector = -1;
	static short 		last_id = -1;
	short 				wall;
	short 				id;
	short 				done;

	if (!media || !grid || !within(grid->active[0].x, -1, GRID_SIZE) || !within(grid->active[0].y, -1, GRID_SIZE))
		return ;
	id = -1;
	done = FALSE;
	wall = -1;
	if (grid->active[1].x == -1)
	{
		if (grid->nodes[grid->active[0].x][grid->active[0].y] == NODE_FULL)
			id = find_vector(media->worlds[media->w_id].vecs, grid->active[0], media->worlds[media->w_id].n_vecs);
		if (id == -1)
		{
			if (add_vector(&media->worlds[media->w_id].vecs, media->worlds[media->w_id].n_vecs, grid, 0) == FAIL)
				return ;
			id = media->worlds[media->w_id].n_vecs++;
		}
		if (add_sector(&media->worlds[media->w_id].sec, media->worlds[media->w_id].n_sec) == FAIL)
			return ;
		sector = media->worlds[media->w_id].n_sec++;
		if (add_sector_v(&media->worlds[media->w_id].sec[sector].v, media->worlds[media->w_id].sec[sector].n_v, id) == FAIL)
			return ;
        media->worlds[media->w_id].sec[sector].n_v++;
		last_id = id;
		first_vector = id;
	}
	else
	{
		if (!within(sector, -1,  media->worlds[media->w_id].n_sec))
			return ;
		if (grid->nodes[grid->active[1].x][grid->active[1].y] == NODE_FULL)
			id = find_vector(media->worlds[media->w_id].vecs, grid->active[1], media->worlds[media->w_id].n_vecs);
		if (id == -1)
		{
			if (add_vector(&media->worlds[media->w_id].vecs, media->worlds[media->w_id].n_vecs, grid, 1) == FAIL)
				return ;
			id = media->worlds[media->w_id].n_vecs++;
		}
		else
		{
			if (id == first_vector)
				done = TRUE;
			else if (already_in_sector(id,media->worlds[media->w_id].sec[sector].v, media->worlds[media->w_id].sec[sector].n_v))
			{
				grid->active[1] = (t_vec2d){ -1, -1 };
				return ;
			}
			else if (grid->nodes[media->worlds[media->w_id].vecs[last_id].x][media->worlds[media->w_id].vecs[last_id].y] == NODE_FULL &&
					 grid->nodes[media->worlds[media->w_id].vecs[id].x][media->worlds[media->w_id].vecs[id].y] == NODE_FULL)
				wall = find_wall(last_id, id, media->worlds[media->w_id].walls, media->worlds[media->w_id].n_walls);
		}
		if (wall == -1)
		{
			if (add_wall(&media->worlds[media->w_id].walls, media->worlds[media->w_id].n_walls, last_id, id) == FAIL)
				return ;
			wall = media->worlds[media->w_id].n_walls++;
		}
		if (add_secwall(&media->worlds[media->w_id].sec[sector].sec_walls, media->worlds[media->w_id].sec[sector].n_walls, wall) == FAIL)
			return ;
		media->worlds[media->w_id].sec[sector].n_walls++;
		if (add_sector_v(&media->worlds[media->w_id].sec[sector].v, media->worlds[media->w_id].sec[sector].n_v, id) == FAIL)
			return ;
        media->worlds[media->w_id].sec[sector].n_v++;
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