/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   math_ops.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mshpakov <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/10/04 17:16:16 by mshpakov          #+#    #+#             */
/*   Updated: 2019/10/04 17:16:18 by mshpakov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "builder.h"

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
