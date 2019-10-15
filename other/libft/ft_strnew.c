/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strnew.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mshpakov <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/10/30 15:00:32 by mshpakov          #+#    #+#             */
/*   Updated: 2018/10/30 15:00:33 by mshpakov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strnew(size_t size)
{
	char		*new;
	size_t		i;

	i = 0;
	if (!(new = (char *)malloc(sizeof(char) * (size + 1))))
		return (NULL);
	while (i <= size)
		new[i++] = 0;
	return (new);
}
