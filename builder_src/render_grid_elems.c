/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   render_grid_elems.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mshpakov <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/09/29 16:26:34 by mshpakov          #+#    #+#             */
/*   Updated: 2019/10/15 14:11:37 by mshpakov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "builder.h"

void					draw_node(t_vec c, int r, t_scr s, t_scr **screen)
{
	draw_circle_fill2(c, r, s, screen);
}

void					draw_player(t_vec c, int r, t_scr s, \
t_scr **screen)
{
	draw_circle_fill2(c, r * 1.5, s, screen);
	draw_circle_fill2(c, r, s, screen);
	draw_circle_fill2(c, r * 0.5, s, screen);
}

void					place_player(t_world world, t_grid *grid, \
t_scr **screen, int radius)
{
	t_vec				node;
	t_scr				s;

	s = (t_scr){ RED, screen_pl_start, 0 };
	node.x = (int)((float)grid->box.x + (float)world.p_start.x * grid->scl);
	node.y = (int)((float)grid->box.y + (float)world.p_start.y * grid->scl);
	draw_player(node, radius * 1.5, s, screen);
	s = (t_scr){ GREEN, screen_pl_end, 0 };
	node.x = (int)((float)grid->box.x + (float)world.p_end.x * grid->scl);
	node.y = (int)((float)grid->box.y + (float)world.p_end.y * grid->scl);
	draw_player(node, radius * 1.5, s, screen);
}

void					clean_screen(t_scr **screen)
{
	int					i;

	if (!screen)
		return ;
	i = 0;
	while (i < W_W)
	{
		ft_memset(screen[i], 0, sizeof(t_scr) * W_H);
		i++;
	}
}
