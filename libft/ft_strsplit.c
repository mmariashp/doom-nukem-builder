/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strsplit.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mshpakov <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/11/05 11:23:44 by mshpakov          #+#    #+#             */
/*   Updated: 2018/11/05 11:23:46 by mshpakov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	**ft_strsplit(char const *s, char c)
{
	char	**a;
	int		j;

	j = -1;
	if (!s || !(a = (char **)malloc(sizeof(s) * (ft_count_words(s, c) + 1))))
		return (NULL);
	while (*s)
	{
		while (*s && *s == c)
			s++;
		if (*s && *s != c)
		{
			if (!(a[++j] = ft_strsub(s, 0, ft_strulen(s, c))))
				return (NULL);
			while (*s && *s != c)
				s++;
		}
	}
	a[++j] = NULL;
	return (a);
}
