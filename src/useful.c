/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   useful.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mshpakov <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/09/28 20:09:24 by mshpakov          #+#    #+#             */
/*   Updated: 2019/09/28 20:09:26 by mshpakov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "builder.h"

unsigned short			within(int value, int min, int max)
{
	if (value <= min)
		return (FALSE);
	if (value >= max)
		return (FALSE);
	return (TRUE);
}

void					bounding_box(t_vec *min, t_vec *max, t_vec *p, \
																		int n_p)
{
	int					i;

	if (!min || !max)
		return ;
	*min = (t_vec){ 0, 0 };
	*max = (t_vec){ W_W, W_H };
	if (!p || n_p < 3)
		return ;
	*min = (t_vec){ W_W, W_H };
	*max = (t_vec){ 0, 0 };
	i = -1;
	while (++i < n_p)
		update_min_max(min, max, p[i]);
	min->x = clamp(min->x, 0, W_W);
	max->x = clamp(max->x, 0, W_W);
	min->y = clamp(min->y, 0, W_H);
	max->y = clamp(max->y, 0, W_H);
}

unsigned short			already_in_sector(int id, int *vecs, int n_v)
{
	int					i;

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

unsigned short			can_place_vec(t_vec v, t_world world, t_grid *grid)
{
	if (!within(v.x, -1, GRID_SIZE) || !within(v.y, -1, GRID_SIZE))
		return (FALSE);
	if (grid->nod[v.x][v.y] != NODE_FULL && nod_in_sec(v, &world) != -1)
		return (FALSE);
	if (grid->nod[v.x][v.y] != NODE_FULL && find_wall_inter(v, world) != -1)
		return (FALSE);
	return (TRUE);
}

unsigned short			mouse_over(t_rec box, t_vec mouse)
{
	if (mouse.x <= box.x || mouse.x >= box.x + box.w)
		return (FALSE);
	if (mouse.y <= box.y || mouse.y >= box.y + box.h)
		return (FALSE);
	return (TRUE);
}
