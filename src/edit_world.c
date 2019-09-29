/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   edit_world.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mshpakov <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/09/29 12:32:09 by mshpakov          #+#    #+#             */
/*   Updated: 2019/09/29 12:32:10 by mshpakov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "builder.h"

void					edit_wall_type(int btn_on, t_world *world)
{
	int					wall;

	if (!world || \
	!within((wall = select_it(1, W_SELECT, -1)), -1, world->n_w))
		return ;
	if (btn_on == W_PORTAL_BTN)
	{
		if (world->walls[wall].type != WALL_FILLED)
			world->walls[wall].type = WALL_FILLED;
		else if (world->walls[wall].type == WALL_FILLED)
			world->walls[wall].type = WALL_EMPTY;
	}
}

void					edit_item_name(int n_itf, t_it_f *it_f, \
t_world *world)
{
	int					sector;
	int					item_i;
	int					item_sel;

	if (!it_f || !world)
		return ;
	sector = select_it(1, S_SELECT, -1);
	item_i = select_it(1, I_SELECT, -1);
	item_sel = select_it(1, SEL_I_SELECT, -1);
	if (within(sector, -1, world->n_s) && \
		within(item_i, -1, world->sec[sector].n_it) && \
		within(item_sel, -1, n_itf))
		world->sec[sector].items[item_i].id = item_sel;
}

void					change_heights(int b, t_sec *sec)
{
	int					s;
	int					f_shift;
	int					c_shift;

	s = select_it(1, S_SELECT, -1);
	f_shift = 0;
	c_shift = 0;
	if (b == F_UP_BTN)
		f_shift++;
	else if (b == F_DOWN_BTN)
		f_shift--;
	else if (b == C_UP_BTN)
		c_shift++;
	else if (b == C_DOWN_BTN)
		c_shift--;
	if (f_shift || c_shift)
	{
		sec[s].fl += f_shift;
		sec[s].ceil += c_shift;
		sec[s].fl = clamp(sec[s].fl, MIN_HEIGHT, MAX_HEIGHT);
		sec[s].ceil = clamp(sec[s].ceil, MIN_HEIGHT, MAX_HEIGHT);
	}
}
