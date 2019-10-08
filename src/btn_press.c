/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   btn_press.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mshpakov <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/09/27 20:05:25 by mshpakov          #+#    #+#             */
/*   Updated: 2019/09/27 20:05:27 by mshpakov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "builder.h"

void					item_edit(t_prog *prog, t_world *world, int n_itf)
{
	int					sector;
	int					item_i;

	if (!prog || !world || !prog->modes[prog->m_id].btn
		|| !prog->modes || !world->sec)
		prog->btn_lit = -1;
	prog->modes[prog->m_id].btn[prog->btn_on].vis_lit_on[2] = FALSE;
	prog->btn_on = -1;
	prog->last = prog->m_id;
	prog->m_id = MODE_SEL_ITEM;
	sector = select_it(1, S_SELECT, -1);
	item_i = select_it(1, I_SELECT, -1);
	if (within(sector, -1, world->n_s) &&
		within(item_i, -1, world->sec[sector].n_it) &&
		within(world->sec[sector].items[item_i].id, -1, n_itf))
	{
		select_it(0, SEL_I_SELECT, world->sec[sector].items[item_i].id);
		prog->btn_on = world->sec[sector].items[item_i].id;
		prog->modes[prog->m_id].btn[prog->btn_on].vis_lit_on[2] = TRUE;
	}
	prog->redraw = 0;
}

void					sec_edit_mng_btn(t_prog *prog, t_world *world, \
t_media *media, t_grid *grid)
{
	if (prog->btn_on == DEL_SEC_BTN)
	{
		zoom_to_box(grid, world->vecs, world->n_v);
		select_it(0, ST_SEL, SEC_SEARCH);
		delete_sector(select_it(1, S_SELECT, -1), world);
		select_it(0, S_SELECT, -1);
		validate_media(media);
	}
	else if (prog->btn_on == B_ITEM_DEL)
	{
		prog->btn_lit = -1;
		turn_btns_off(prog);
		delete_item(&world->sec[select_it(1, S_SELECT, -1)], \
		select_it(1, I_SELECT, -1));
		select_it(0, I_SELECT, -1);
	}
	else if (within(prog->btn_on, F_UP_BTN - 1, C_DOWN_BTN + 1))
		change_heights(prog->btn_on, world->sec);
	else if (prog->btn_on == B_ITEM_EDIT)
		item_edit(prog, world, media->n_itf);
}

void					edit_mng_btn(t_prog *prog, t_media *m, t_grid *grid, \
int s)
{
	select_it(0, LAST_ST_SEL, s);
	if (prog->btn_on == DESELECT_BTN)
	{
		zoom_to_box(grid, m->worlds[m->w].vecs, m->worlds[m->w].n_v);
		if (s == SEC_EDIT)
			select_it(0, ST_SEL, SEC_SEARCH);
		else
		{
			select_it(0, ST_SEL, WALL_SEARCH);
			select_it(0, W_SELECT, -1);
		}
		select_it(0, S_SELECT, -1);
		lit_it(0, W_SELECT, -1);
	}
	else if ((s == SEC_EDIT && (prog->btn_on == FT_EDIT_BTN || prog->btn_on ==\
	CT_EDIT_BTN)) || (s == WALL_EDIT && prog->btn_on == WT_EDIT_BTN))
		prep_texture_edit(&m->worlds[m->w], prog, m->n_t);
	else if (s == WALL_EDIT && prog->btn_on == PORT_BTN)
	{
		edit_wall_type(prog->btn_on, &m->worlds[m->w]);
		turn_btns_off(prog);
	}
	else if (s == SEC_EDIT)
		sec_edit_mng_btn(prog, &m->worlds[m->w], m, grid);
}

void					prep_settings(t_prog *prog, t_grid *grid, \
t_media *media)
{
	if (!prog || !grid || !media)
		return ;
	turn_btns_off(prog);
	prog->btn_lit = -1;
	prog->last = MODE_EDITOR;
	prog->m_id = MODE_SETTINGS;
}

void					settings_mng_btn(t_prog *prog, t_media *media)
{
	if (within(prog->btn_on, S_F_UP_BTN - 1, S_C_DOWN_BTN + 1))
		change_def_heights(prog->btn_on);
	else if (within(prog->btn_on, S_WT_BTN - 1, S_TT_BTN + 1))
		change_def_textures(prog, media->n_t);
}

unsigned short			btn_press(t_prog *prog, t_grid *grid, t_media *media, \
int state)
{
	if (!prog || !grid || !media)
		return (FAIL);
	turn_btn_on(prog, grid);
	if (prog->m_id == MODE_LEVELS)
		return (SUCCESS);
	else if (prog->m_id == MODE_SETTINGS)
		settings_mng_btn(prog, media);
	else if (state == NORMAL || state == SEC_SEARCH || state == WALL_SEARCH)
	{
		if (prog->btn_on == SETTINGS_BTN)
			prep_settings(prog, grid, media);
		select_it(0, LAST_ST_SEL, state);
		state = prog->btn_on == SEC_BTN ? SEC_SEARCH : NORMAL;
		state = prog->btn_on == WALL_BTN || prog->btn_on == DOOR_ADD_BTN ? \
		WALL_SEARCH : state;
		select_it(0, ST_SEL, state);
	}
	else if (state == SEC_EDIT || state == WALL_EDIT)
		edit_mng_btn(prog, media, grid, state);
	return (SUCCESS);
}
