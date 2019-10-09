/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   read_line.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mshpakov <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/09/29 19:38:13 by mshpakov          #+#    #+#             */
/*   Updated: 2019/10/09 17:09:24 by mshpakov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "builder.h"

void					empty_st(unsigned *tab, unsigned *current, \
unsigned short w)
{
	if (tab)
	{
		tab[0] = 0;
		tab[1] = 0;
		tab[2] = 0;
		tab[3] = 0;
		*current = w;
	}
}

unsigned short			read_into_w(t_vec p, t_world *world, \
unsigned *vwsp, char *line)
{
	if (p.x < MIN_VERTEX_ID || p.x > MAX_VERTEX_ID || p.x > (int)vwsp[0] ||
		p.y < MIN_VERTEX_ID || p.y > MAX_VERTEX_ID || p.y > (int)vwsp[0])
		return (FAIL);
	get_walls(&world->walls[vwsp[1]], line, p);
	if (*line == '\0' || world->walls[vwsp[1]].type == -1)
	{
		ft_putstr("Incorrect wall type for wall ");
		ft_putnbr(vwsp[1]);
		ft_putchar('\n');
		return (FAIL);
	}
	if (world->walls[vwsp[1]].type == WALL_FILLED && \
	world->walls[vwsp[1]].txtr == -1)
	{
		ft_putstr("Incorrect wall texture for wall ");
		ft_putnbr(vwsp[1]);
		ft_putchar('\n');
		return (FAIL);
	}
	vwsp[1]++;
	return (SUCCESS);
}

unsigned short			read_into_secs(t_world *world, \
unsigned *vwsp, char *line)
{
	world->sec[vwsp[2]].items = NULL;
	world->sec[vwsp[2]].s_walls = NULL;
	world->sec[vwsp[2]].v = NULL;
	world->sec[vwsp[2]].fl = 0;
	world->sec[vwsp[2]].ceil = 0;
	world->sec[vwsp[2]].fl_t = 0;
	world->sec[vwsp[2]].ceil_t = 0;
	world->sec[vwsp[2]].n_it = 0;
	world->sec[vwsp[2]].n_w = 0;
	world->sec[vwsp[2]].n_v = 0;
	world->sec[vwsp[2]].status = 0;
	world->sec[vwsp[2]].is_door = FALSE;
	while (line && *line != 'f')
	{
		if (*line++ == 'd')
			world->sec[vwsp[2]].is_door = TRUE;
	}
	if (get_sec_fl_ceil(&world->sec[vwsp[2]], line) == FAIL || \
	!(line = ft_strchr(line, 'w')) || get_s_walls(&world->sec[vwsp[2]], \
	line, world->n_w) == FAIL || !(line = ft_strchr(line, 'i')))
		return (FAIL);
	if (get_sec_items(&world->sec[vwsp[2]++], line) == FAIL)
		return (FAIL);
	return (SUCCESS);
}

unsigned				read_line(char *l, unsigned short s, \
t_world *w, unsigned short w_no)
{
	static unsigned		vwsp[4] = { 0, 0, 0, 0 };
	static unsigned		curr = 0;
	t_vec				p;
	char				sep;

	if (curr != w_no)
		empty_st(vwsp, &curr, w_no);
	if (!l || !w || !s || !(l = ft_strchr(l, ')')))
		return (FAIL);
	if (l++ && s == R_SECS && vwsp[2] < w->n_s)
		return (read_into_secs(w, vwsp, l));
	sep = s == R_WALLS ? '-' : ',';
	p.x = ft_atoi(l);
	if (!(l = ft_strchr(l, sep)))
		return (FAIL);
	p.y = ft_atoi(++l);
	if (s == R_VECTORS && vwsp[0] < w->n_v)
		w->vecs[vwsp[0]++] = (t_vec){ clamp(p.x, 0, 150), clamp(p.y, 0, 150) };
	if (s == R_WALLS && vwsp[1] < w->n_w && read_into_w(p, w, vwsp, l) == FAIL)
		return (FAIL);
	if (s == R_PLR && vwsp[3]++ == 0)
		w->p_start = p;
	else if (s == R_PLR)
		w->p_end = p;
	return (SUCCESS);
}
