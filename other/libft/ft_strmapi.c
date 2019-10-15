/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strmapi.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mshpakov <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/10/30 16:19:53 by mshpakov          #+#    #+#             */
/*   Updated: 2018/10/30 16:19:54 by mshpakov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strmapi(char const *s, char (*f)(unsigned int, char))
{
	char		*new;
	size_t		i;

	i = 0;
	if (s == NULL || !*s || !*f)
		return (NULL);
	if (!(new = (char *)malloc(sizeof(char) * (ft_strlen(s) + 1))))
		return (NULL);
	while (*s)
	{
		new[i] = f(i, *s++);
		i++;
	}
	new[i] = '\0';
	return (new);
}
