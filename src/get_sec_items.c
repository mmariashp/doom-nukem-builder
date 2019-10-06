/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_sec_items.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mshpakov <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/09/27 20:05:25 by mshpakov          #+#    #+#             */
/*   Updated: 2019/09/27 20:05:27 by mshpakov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "builder.h"

unsigned short			count_it(char const *s)
{
	unsigned short		count;
	unsigned short		nb;

	count = 0;
	while (*(++s) && *s != '\'')
	{
		if (*s == '(' && ++count < MAX_SEC_ITEMS)
		{
			nb = 0;
			while (*s && *s != ')')
			{
				if (!ft_isdigit(*s))
					s++;
				else
				{
					nb++;
					while (*s && ft_isdigit(*s))
						s++;
				}
			}
			if (*s != ')' || nb != 3)
				return (0);
		}
	}
	return (count);
}

int						get_sec_items(t_sec *s, char *l)
{
	t_item				*items;
	int					i;

	if (!s || !l || !(l = ft_strchr(l, '\'')) || (s->n_it = count_it(l++)) > 20)
		return (FAIL);
	if (!(items = (t_item *)ft_memalloc(sizeof(t_item) * s->n_it)))
		return (FAIL);
	i = 0;
	while (*l && *l != '\'' && i < s->n_it)
	{
		if (*l == '(' && ++l)
		{
			items[i].p.x = clamp(ft_atoi(l), 0, GRID_SIZE - 1);
			if (!(l = ft_strchr(l, ',')) || !++l)
				break ;
			items[i].p.y = clamp(ft_atoi(l), 0, GRID_SIZE - 1);
			if (!(l = ft_strchr(l, ' ')) || !++l)
				break ;
			items[i++].id = clamp(ft_atoi(l), 0, MAX_ITEMFULL - 1);
			if (!(l = ft_strchr(l, ')')))
				break ;
		}
		l++;
	}
	if (i != s->n_it)
	{
		s->n_it = 0;
		free(items);
		return (FAIL);
	}
	s->items = items;
	return (SUCCESS);
}
