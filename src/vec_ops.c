/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   vec_ops.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mshpakov <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/09/27 17:16:34 by mshpakov          #+#    #+#             */
/*   Updated: 2019/09/27 17:16:36 by mshpakov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "builder.h"

void					vec_set(t_vec *vec, int x, int y, int n)
{
	int					i;

	i = 0;
	if (vec)
	{
		while (i < n)
		{
			vec[i] = (t_vec){ x, y };
			i++;
		}
	}
}

unsigned short			vec_same(t_vec one, t_vec two)
{
	if (one.x == two.x && one.y == two.y)
		return (TRUE);
	return (FALSE);
}

void					update_min_max(t_vec *min, t_vec *max, t_vec p)
{
	if (min && max)
	{
		min->x = get_min(min->x, p.x);
		min->y = get_min(min->y, p.y);
		max->x = get_max(max->x, p.x);
		max->y = get_max(max->y, p.y);
	}
}