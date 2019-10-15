/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memccpy.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mshpakov <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/10/29 19:01:52 by mshpakov          #+#    #+#             */
/*   Updated: 2018/10/29 19:01:54 by mshpakov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memccpy(void *dst, const void *src, int c, size_t n)
{
	unsigned char a;
	unsigned char *d;
	const unsigned char *s;

	a = (unsigned char)c;
	d = (unsigned char *)dst;
	s = (const unsigned char *)src;
	if (n == 0)
		return (NULL);
	while (n-- > 0)
	{
		*d = *s;
		if (*s == a)
			return (__extension__(++dst));
		d++;
		s++;
		__extension__(dst++);
	}
	return (NULL);
}