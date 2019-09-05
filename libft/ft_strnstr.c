/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strnstr.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mshpakov <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/10/29 12:50:44 by mshpakov          #+#    #+#             */
/*   Updated: 2018/10/29 12:50:45 by mshpakov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char		*ft_strnstr(const char *hay, const char *need, size_t len)
{
	size_t i;
	size_t nlen;

	nlen = ft_strlen(need);
	i = len - nlen + 1;
	if (!*need)
		return ((char *)hay);
	if (!*hay)
		return (NULL);
	while (*hay && (int)i-- > 0)
	{
		if (*hay == *need)
		{
			if (ft_strncmp(hay, need, nlen) == 0)
				return ((char *)hay);
		}
		hay++;
	}
	return (NULL);
}
