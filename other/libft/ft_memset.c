/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memset.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mshpakov <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/10/24 21:10:10 by mshpakov          #+#    #+#             */
/*   Updated: 2018/10/30 14:33:49 by mshpakov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memset(void *b, int c, size_t len)
{
	unsigned char	a;
	char			*d;

	a = c;
	d = b;
	while (len > 0)
	{
		*d = a;
		d++;
		len--;
	}
	if (b == NULL)
		return (NULL);
	return (b);
}
