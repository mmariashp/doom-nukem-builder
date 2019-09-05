/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strlcat.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mshpakov <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/10/29 12:49:08 by mshpakov          #+#    #+#             */
/*   Updated: 2018/10/29 12:49:10 by mshpakov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

size_t	ft_strlcat(char *dst, const char *src, size_t dstsize)
{
	size_t	i;
	size_t	j;
	size_t	dstlen;

	i = 0;
	j = 0;
	if (ft_strlen(dst) > dstsize)
		dstlen = dstsize;
	else
		dstlen = ft_strlen(dst);
	while (dst[i] != '\0')
		i++;
	while (src[j] != '\0' && i < dstsize - 1 && dstsize > 0)
		dst[i++] = src[j++];
	dst[i] = '\0';
	return (ft_strlen(src) + dstlen);
}
