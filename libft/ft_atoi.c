/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_atoi.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mshpakov <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/10/27 14:23:34 by mshpakov          #+#    #+#             */
/*   Updated: 2018/10/27 14:23:36 by mshpakov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int		ft_atoi(const char *str)
{
	unsigned long int	r;
	int					m;
	unsigned long int	l;

	r = 0;
	m = 1;
	l = 922337203685477580;
	while (ft_isspace(*str) == 1)
		str++;
	if (*str == '+' && *(str + 1) != '-')
		str++;
	if (*str == '-' && *(str - 1) != '+')
	{
		m = -1;
		str++;
	}
	while (*str != '\0' && ft_isdigit(*str) == 1)
	{
		if ((r > l || (r == l && (*str - '0') > 7)) && m == 1)
			return (-1);
		else if ((r > l || (r == l && (*str - '0') > 8)) && m == -1)
			return (0);
		r = r * 10 + (__typeof__(r))(*str++ - 48);
	}
	return ((int)((long)m * (long)r));
}
