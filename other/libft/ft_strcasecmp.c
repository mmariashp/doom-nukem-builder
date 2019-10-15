/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strcasecmp.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mshpakov <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/11/09 18:30:13 by mshpakov          #+#    #+#             */
/*   Updated: 2018/11/09 18:30:17 by mshpakov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int		ft_strcasecmp(const char *s1, const char *s2)
{
	int a;

	while (ft_tolower(*s1) == ft_tolower(*s2) && *s1 && *s2)
	{
		s1++;
		s2++;
	}
	a = (unsigned char)(ft_tolower(*s1)) - (unsigned char)(ft_tolower(*s2));
	return (a);
}
