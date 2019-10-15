/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strstr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mshpakov <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/10/29 12:50:31 by mshpakov          #+#    #+#             */
/*   Updated: 2018/10/29 12:50:32 by mshpakov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char		*ft_strstr(const char *hay, const char *need)
{
	if (!*need)
		return ((char *)hay);
	while (*hay)
	{
		if (*hay == *need)
		{
			if (ft_strncmp(hay, need, ft_strlen(need)) == 0)
				return ((char *)hay);
		}
		hay++;
	}
	return (NULL);
}
