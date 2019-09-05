/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strtrim.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mshpakov <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/11/05 11:23:28 by mshpakov          #+#    #+#             */
/*   Updated: 2018/11/05 11:23:32 by mshpakov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strtrim(char const *s)
{
	int		i;
	int		len;
	char	*new;
	int		j;

	i = -1;
	if (!s)
		return (NULL);
	len = ft_strlen(s);
	while (ft_iswhspace(s[len - 1]) == 1)
		len--;
	while (ft_iswhspace(s[++i]) == 1)
		len--;
	if (len < 0)
		len = 0;
	if (!(new = (char *)malloc(sizeof(char) * (len + 1))))
		return (NULL);
	j = 0;
	while (j < len)
		new[j++] = s[i++];
	new[j] = '\0';
	return (new);
}
