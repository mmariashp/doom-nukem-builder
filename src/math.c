/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   math.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mshpakov <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/09/28 20:11:09 by mshpakov          #+#    #+#             */
/*   Updated: 2019/09/28 20:11:10 by mshpakov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "builder.h"

int                     get_max(int one, int two)
{
    if (one > two)
        return (one);
    return (two);
}

int                     get_min(int one, int two)
{
    if (one < two)
        return (one);
    return (two);
}

int						clamp(int n, int min, int max)
{
	if (n < min)
		return (min);
	else if (n > max)
		return (max);
	return (n);
}

float					clamp_f(float n, float min, float max)
{
	if (n < min)
		return (min);
	else if (n > max)
		return (max);
	return (n);
}

t_vec					add(t_vec one, t_vec two)
{
	return ((t_vec){ one.x + two.x, one.y + two.y });
}

t_vec					sub_one_from_two(t_vec one, t_vec two)
{
	return ((t_vec){ two.x - one.x, two.y - one.y });
}

t_vec					mult(t_vec one, t_vec two)
{
	return ((t_vec){ one.x * two.x, one.y * two.y });
}

t_vec					scl(t_vec vec, int scl)
{
	return ((t_vec){ vec.x * scl, vec.y * scl });
}
