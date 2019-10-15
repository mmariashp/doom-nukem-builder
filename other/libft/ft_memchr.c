/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memchr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mshpakov <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/10/29 19:02:17 by mshpakov          #+#    #+#             */
/*   Updated: 2018/10/29 19:02:18 by mshpakov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memchr(const void *s, int c, size_t n)
{
	const unsigned char *s2;
	unsigned char a;

	a = (unsigned char)c;
	s2 = (const unsigned char *)s;
	while (n-- > 0)
	{
		if (*s2++ == a)
			return ((void *)s);
		__extension__(s++);
	}
	return (NULL);
}
