/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strdup.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mshpakov <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/10/23 17:50:00 by mshpakov          #+#    #+#             */
/*   Updated: 2018/10/23 17:50:03 by mshpakov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strdup(const char *s1)
{
	char	*dst;
	int		i;
	int		j;

	i = 0;
	j = 0;
	while (s1[i] != '\0')
		i++;
	if (!(dst = (char *)malloc(i + 1)))
		return (NULL);
	while (j < i)
	{
		dst[j] = s1[j];
		j++;
	}
	dst[i] = '\0';
	return (dst);
}
