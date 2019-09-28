
#include "builder.h"

unsigned short			count_items(char const *s)
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

int 					get_sec_items(t_sec *sec, char *l)
{
	t_item				*items;
	int					i;

	if (!sec || !l || !(l = ft_strchr(l, '\'')) || (sec->n_items = \
	count_items(l++)) > MAX_SEC_ITEMS || !(items = (t_item *)ft_memalloc(\
	sizeof(t_item) * sec->n_items)))
		return (FAIL);
	i = 0;
	while (*l && *l != '\'' && i < sec->n_items)
	{
		if (*l == '(')
		{
			if (!++l)
				break ;
			items[i].p.x = clamp(ft_atoi(l), 0, GRID_SIZE - 1);
			if (!(l = ft_strchr(l, ',')) || !++l)
				break ;
			items[i].p.y = clamp(ft_atoi(l), 0, GRID_SIZE - 1);
			if (!(l = ft_strchr(l, ' ')) || !++l)
				break ;
			items[i].id = clamp(ft_atoi(l), 0, MAX_ITEMFULL - 1);
			if (!(l = ft_strchr(l, ')')))
				break ;
			i++;
		}
		l++;
	}
	if (i != sec->n_items)
	{
		sec->n_items = 0;
		free(items);
		return (FAIL);
	}
	sec->items = items;
	return (SUCCESS);
}