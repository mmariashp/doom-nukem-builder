/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   delete_world.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mshpakov <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/09/23 15:18:22 by mshpakov          #+#    #+#             */
/*   Updated: 2019/09/23 15:18:30 by mshpakov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "builder.h"

void					free_world(t_world *world)
{
	int 				i;

	if (!world)
		return ;
	if (world->filename)
		free(world->filename);
	if (world->full_path)
		free(world->full_path);
	if (world->sec)
	{
		i = 0;
		while (i < world->n_sec)
			free_sector(&world->sec[i++]);
		free(world->sec);
	}
	if (world->vecs)
		free(world->vecs);
	if (world->walls)
		free(world->walls);
}

void					copy_world(t_world *new, t_world *old)
{
	int 				i;

	if (!old || !new)
		return ;
	new->filename = ft_strdup(old->filename);
	new->full_path = ft_strdup(old->full_path);
	new->sec = ft_memalloc(sizeof(t_sec) *  old->n_sec);
	new->walls = ft_memalloc(sizeof(t_wall) *  old->n_walls);
	new->vecs = ft_memalloc(sizeof(t_vec2d) *  old->n_vecs);
	i = -1;
	while (new->walls && ++i < old->n_walls)
		new->walls[i] = old->walls[i];
	i = -1;
	while (new->vecs && ++i < old->n_vecs)
		new->vecs[i] = old->vecs[i];
	i = -1;
	while (new->sec && ++i < old->n_sec)
		copy_sector(&new->sec[i], &old->sec[i]);
	new->p_start = old->p_start;
	new->p_end = old->p_end;
	new->n_sec = old->n_sec;
	new->n_vecs = old->n_vecs;
	new->n_walls = old->n_walls;
	free_world(old);
}

void					delete_world(t_media *media, int id)
{
	t_world				*new;
	int 				i;
	int 				j;

	if (!media || media->n_worlds < 2)
		return ;
	new = (t_world *)ft_memalloc(sizeof(t_world) * (media->n_worlds - 1));
	if (new)
	{
		i = 0;
		j = 0;
		while (j < media->n_worlds)
		{
			if (j == id)
				free_world(&media->worlds[j++]);
			if (j < media->n_worlds)
				copy_world(&new[i++], &media->worlds[j++]);
		}
		free(media->worlds);
		media->worlds = new;
		media->n_worlds--;
	}
}
