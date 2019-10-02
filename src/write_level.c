/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   write_level.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mshpakov <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/09/29 17:16:37 by mshpakov          #+#    #+#             */
/*   Updated: 2019/09/29 17:16:38 by mshpakov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "builder.h"

void					write_items(t_sec sector, int fd)
{
	int 				i;

	if (sector.items && sector.n_it > 0)
	{
		i = 0;
		while (i < sector.n_it)
		{
			ft_putstr_fd("(", fd);
			ft_putnbr_fd(sector.items[i].p.x, fd);
			ft_putstr_fd(",", fd);
			ft_putnbr_fd(sector.items[i].p.y, fd);
			ft_putstr_fd(" ", fd);
			ft_putnbr_fd(sector.items[i].id, fd);
			ft_putstr_fd(")", fd);
			i++;
		}
	}
}

int 					get_n_sect(int section, t_world world)
{
	int 				n;

	if (section == 0)
		n = world.n_v;
	else if (section == 1)
		n = world.n_w;
	else if (section == 2)
		n = world.n_s;
	else if (section == 3)
		n = 2;
	else
		return (-1);
	return (n);
}

void					write_sector(t_sec sec, int fd)
{
	if (sec.is_door)
		ft_putstr_fd("door ", fd);
	ft_putstr_fd("floor(", fd);
	ft_putnbr_fd(sec.fl, fd);
	ft_putstr_fd(" ", fd);
	ft_putnbr_fd(sec.fl_t, fd);
	ft_putstr_fd(") ceil(", fd);
	ft_putnbr_fd(sec.ceil, fd);
	ft_putstr_fd(" ", fd);
	ft_putnbr_fd(sec.ceil_t, fd);
	ft_putstr_fd(") walls '", fd);
	int j = 0;
	while (j < sec.n_w)
	{
		ft_putnbr_fd(sec.s_walls[j], fd);
		ft_putstr_fd(" ", fd);
		j++;
	}
	ft_putstr_fd("' items '", fd);
	write_items(sec, fd);
	ft_putendl_fd("'", fd);
}

void					write_tmp(int section, int fd, int i, t_world world)
{
	t_vec				tmp;
	char 				del;

	tmp = (t_vec){ 0, 0 };
	if (section == 2)
		return ;
	if (section == 0)
		tmp = (t_vec){ world.vecs[i].x, world.vecs[i].y };
	else if (section == 1)
		tmp = (t_vec){ world.walls[i].v1, world.walls[i].v2 };
	else if (section == 3)
		tmp = i == 0 ? world.p_start : world.p_end;
	del = section == 1 ? '-' : ',';
	ft_putnbr_fd(tmp.x, fd);
	ft_putchar_fd(del, fd);
	ft_putnbr_fd(tmp.y, fd);
	if (section == 0)
		ft_putchar_fd('\n', fd);
	else if (section == 3)
	{
		ft_putstr_fd(" (sector: ", fd);
		ft_putnbr_fd(nod_in_sec(tmp, &world), fd);
		if (i == 0)
			ft_putendl_fd(") start", fd);
		else
			ft_putendl_fd(") end", fd);
	}
}

unsigned short			write_level_section(int fd, t_world world, int section)
{
	static char			title[4][9] = { "Vectors", "Walls", "Sectors", \
	"Player" };
	unsigned short		i;
	int					n;

	ft_putendl_fd(title[section], fd);
	if ((n = get_n_sect(section, world)) == -1)
		return (FAIL);
	i = -1;
	while (++i < n)
	{
		ft_putnbr_fd(i, fd);
		ft_putstr_fd(") ", fd);
		write_tmp(section, fd, i, world);
		if (section == 1)
		{
			if (world.walls[i].type == WALL_FILLED)
				ft_putstr_fd(" filled ", fd);
			else if (world.walls[i].type == WALL_EMPTY)
				ft_putstr_fd(" empty ", fd);
			ft_putnbr_fd(world.walls[i].txtr, fd);
			ft_putchar_fd('\n', fd);
		}
		else if (section == 2 && world.sec)
			write_sector(world.sec[i], fd);
	}
	ft_putchar_fd('\n', fd);
	return (SUCCESS);
}


