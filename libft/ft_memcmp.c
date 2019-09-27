/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memcmp.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mshpakov <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/11/05 11:14:44 by mshpakov          #+#    #+#             */
/*   Updated: 2018/11/05 11:14:50 by mshpakov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int		ft_memcmp(const void *s1, const void *s2, size_t n)
{
	const unsigned char *a;
	const unsigned char *b;

	a = (const unsigned char *)s1;
	b = (const unsigned char *)s2;
	if (a == b || n == 0)
		return (0);
	while (*a == *b && (int)--n > 0)
	{
		a++;
		b++;
	}
	return (*a - *b);
}
