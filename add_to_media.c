/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   add_to_media.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mshpakov <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/09/28 20:13:17 by mshpakov          #+#    #+#             */
/*   Updated: 2019/09/28 20:13:19 by mshpakov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "builder.h"

unsigned short			add_vec(t_vec2d **vecs, short n_v, t_grid *grid, short i)
{
	if (!grid)
		return (FAIL);
	*vecs = (t_vec2d *)realloc_tab(*vecs, sizeof(t_vec2d) * (n_v + 1), \
	sizeof(t_vec2d) * n_v);
	if (!*vecs)
		return (FAIL);
	(*vecs)[n_v] = grid->p[i];
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

unsigned short			add_sector(t_sec **sec, short n_s)
{
	*sec = (t_sec *)realloc_tab(*sec, sizeof(t_sec) * (n_s + 1), \
	sizeof(t_sec) * n_s);
	if (!*sec)
		return (FAIL);
	(*sec)[n_s].s_walls = NULL;
	(*sec)[n_s].items = NULL;
	(*sec)[n_s].fl = 0;
	(*sec)[n_s].ceiling = 20;
	(*sec)[n_s].fl_txtr = 0;
	(*sec)[n_s].ceil_txtr = 0;
	(*sec)[n_s].n_w = 0;
	(*sec)[n_s].n_items = 0;
    (*sec)[n_s].n_v = 0;
	(*sec)[n_s].is_door= FALSE;
	(*sec)[n_s].status = SEC_CONVEX_CLOSED;
	return (SUCCESS);
}

unsigned short			add_wall(t_wall **walls, short n_w, int one, int two)
{
	*walls = (t_wall *)realloc_tab(*walls, sizeof(t_wall) * (n_w + 1), \
	sizeof(t_wall) * n_w);
	if (!*walls)
		return (FAIL);
	(*walls)[n_w].type = WALL_FILLED;
    (*walls)[n_w].txtr = 0;
	(*walls)[n_w].v1 = one;
	(*walls)[n_w].v2 = two;
	return (SUCCESS);
}

unsigned short			add_secwall(int **secwalls, short n_swalls, int wall)
{
	if (n_swalls > 0 && !*secwalls)
		return (FAIL);
	*secwalls = (int *)realloc_tab(*secwalls, sizeof(int) * (n_swalls + 1), \
	sizeof(int) * n_swalls);
	if (!*secwalls)
		return (FAIL);
	(*secwalls)[n_swalls] = wall;
	return (SUCCESS);
}

char 					*get_name(int i)
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
	(*worlds)[n_worlds].name = get_name(i);
	(*worlds)[n_worlds].full_path = get_full_path((*worlds)[n_worlds].name, ext, path);
	if (!(*worlds)[n_worlds].name || !(*worlds)[n_worlds].full_path)
		return (FAIL);
	(*worlds)[n_worlds].sec = NULL;
	(*worlds)[n_worlds].n_s = 0;
	(*worlds)[n_worlds].walls = NULL;
	(*worlds)[n_worlds].n_w = 0;
	(*worlds)[n_worlds].vecs = NULL;
	(*worlds)[n_worlds].n_v = 0;
	(*worlds)[n_worlds].p_start = (t_vec2d){ 1, 1 };
	(*worlds)[n_worlds].p_end = (t_vec2d){ 1, 3 };
	i++;
	return (SUCCESS);
}

unsigned short			already_in_sector(int id, int *vecs,int n_v)
{
	int 				i;

	if (!vecs)
		return (FALSE);
	i = 0;
	while (i < n_v)
	{
		if (vecs[i++] == id)
			return (TRUE);
	}
	return (FALSE);
}

unsigned short			can_place_vec(t_vec2d v, t_world world, t_grid *grid)
{
	if (!within(v.x, -1, GRID_SIZE) || !within(v.y, -1, GRID_SIZE))
	{
		printf("can't place because outside of grid\n");
		return (FALSE);
	}
	if (grid->nodes[v.x][v.y] != NODE_FULL && node_in_sec(v, &world) != -1)
	{
		printf("can't place because in sector\n");
		return (FALSE);
	}
	if (grid->nodes[v.x][v.y] != NODE_FULL && find_wall_inter(v, world) != -1)
	{
		printf("can't place because intersection\n");
		return (FALSE);
	}
	return (TRUE);
}



