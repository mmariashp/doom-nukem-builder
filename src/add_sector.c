/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   add_sector.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mshpakov <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/09/29 12:23:47 by mshpakov          #+#    #+#             */
/*   Updated: 2019/09/29 12:23:49 by mshpakov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "builder.h"

unsigned short			add_sector_v(int **v, short n_v, int id)
{
	*v = (int *)realloc_tab(*v, sizeof(int) * (n_v + 1), sizeof(int) * n_v);
	if (!*v)
		return (FAIL);
	(*v)[n_v] = id;
	return (SUCCESS);
}

unsigned short			add_sector(t_sec **sec, short n_s)
{
	*sec = (t_sec *)realloc_tab(*sec, sizeof(t_sec) * (n_s + 1), \
	sizeof(t_sec) * n_s);
	if (!*sec)
		return (FAIL);
	(*sec)[n_s].s_walls = NULL;
	(*sec)[n_s].items = NULL;
	(*sec)[n_s].fl = 0;
	(*sec)[n_s].ceil = 20;
	(*sec)[n_s].fl_t = 0;
	(*sec)[n_s].ceil_t = 0;
	(*sec)[n_s].n_w = 0;
	(*sec)[n_s].n_it = 0;
	(*sec)[n_s].n_v = 0;
	(*sec)[n_s].is_door = FALSE;
	(*sec)[n_s].status = SEC_CONVEX_CLOSED;
	return (SUCCESS);
}

unsigned short			add_secwall(int **secwalls, short n_swalls, int wall)
{
	if (n_swalls > 0 && !*secwalls)
		return (FAIL);
	*secwalls = (int *)realloc_tab(*secwalls, sizeof(int) * (n_swalls + 1), \
	sizeof(int) * n_swalls);
	if (!*secwalls)
		return (FAIL);
	(*secwalls)[n_swalls] = wall;
	return (SUCCESS);
}