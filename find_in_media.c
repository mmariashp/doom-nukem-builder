/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   find_in_media.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mshpakov <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/09/28 16:43:54 by mshpakov          #+#    #+#             */
/*   Updated: 2019/09/28 16:43:55 by mshpakov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "builder.h"

short					find_vec(t_vec2d *vecs, t_vec2d p, int n)
{
	short				i;

	if (!vecs)
		return (-1);
	i = 0;
	while (i < n)
	{
		if (vecs[i].x == p.x && vecs[i].y == p.y)
			return (i);
		i++;
	}
	return (-1);
}

short					find_wall(short one, short two, t_wall *walls,
																	short n_w)
{
	short				i;

	i = 0;
	if (!walls || n_w < 1)
		return (-1);
	while (i < n_w)
	{
		if (one == walls[i].v1 && two == walls[i].v2)
			return (i);
		if (two == walls[i].v1 && one == walls[i].v2)
			return (i);
		i++;
	}
	return (-1);
}
