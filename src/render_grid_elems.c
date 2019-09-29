/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   render_grid_elems.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mshpakov <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/09/29 16:26:34 by mshpakov          #+#    #+#             */
/*   Updated: 2019/09/29 16:26:35 by mshpakov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "builder.h"

void					draw_node(t_vec c, int r, int color, int **screen)
{
	draw_circle_fill2(c, r, color, screen);
}

void					draw_player(t_vec c, int r, int color, int **screen)
{
	draw_circle_fill2(c, r * 1.5, DARK_GRAY, screen);
	draw_circle_fill2(c, r, color, screen);
	draw_circle_fill2(c, r * 0.5, WHITE, screen);
}

void					place_player(t_world world, t_grid *grid, int **screen,\
int radius)
{
	t_vec				node;

	node.x = (int)(grid->box.x + world.p_start.x * grid->scl);
	node.y = (int)(grid->box.y + world.p_start.y * grid->scl);
	draw_player(node, radius * 1.5, RED, screen);
	node.x = (int)(grid->box.x + world.p_end.x * grid->scl);
	node.y = (int)(grid->box.y + world.p_end.y * grid->scl);
	draw_player(node, radius * 1.5, GREEN, screen);
}

void					clean_screen(int **screen)
{
	int					i;

	if (!screen)
		return ;
	i = 0;
	while (i < W_W)
	{
		ft_memset(screen[i], 0, sizeof(int) * W_H);
		i++;
	}
}
