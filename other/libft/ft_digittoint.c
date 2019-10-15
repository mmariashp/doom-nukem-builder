/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_digittoint.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mshpakov <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/11/09 18:01:54 by mshpakov          #+#    #+#             */
/*   Updated: 2018/11/09 18:01:55 by mshpakov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int		ft_digittoint(int c)
{
	if (ft_isxdigit(c) == 0)
		return (0);
	if (ft_isdigit(c) == 1)
		return (c - '0');
	if (ft_isupper(c) == 1)
		c = ft_tolower(c);
	return (c - 87);
}
