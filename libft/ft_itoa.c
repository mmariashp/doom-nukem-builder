/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_itoa.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mshpakov <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2018/11/06 17:43:43 by mshpakov          #+#    #+#             */
/*   Updated: 2018/11/06 17:43:45 by mshpakov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_itoa(int n)
{
	int		i;
	int		j;
	char	*str;

	i = n;
	j = 1;
	while ((i = i / 10))
		j++;
	if (n < 0)
		j++;
	if (!(str = (char *)malloc(sizeof(char) * (j + 1))))
		return (NULL);
	i = 0;
	if (n < 0)
		str[i++] = '-';
	if (n == -2147483648)
		str[i++] = '2';
	n = ft_abs_cut(n);
	str[j] = '\0';
	while (--j >= i)
	{
		str[j] = n % 10 + '0';
		n = n / 10;
	}
	return (str);
}
