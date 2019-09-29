/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   manage_btn.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mshpakov <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/09/27 20:05:25 by mshpakov          #+#    #+#             */
/*   Updated: 2019/09/27 20:05:27 by mshpakov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "builder.h"

unsigned short			btn_refresh(t_prog *prog, int state)
{
	int 				on;

	if (!prog)
		return (FAIL);
	on = prog->btn_on;
	on = select_it(1, LAST_ST_SEL, -1) == WALL_EDIT ? WALL_BTN : on;
	get_btn(state, &prog->modes[prog->m_id]);
	turn_btns_off(prog);
	if (state == SEC_SEARCH)
		prog->btn_on = SEC_BTN;
	else if (state == WALL_SEARCH)
		prog->btn_on = on;
	else if (state == NORMAL)
		prog->btn_on = DRAG_BTN;
	if (within(prog->btn_on, -1, prog->modes[prog->m_id].n_btn))
		prog->modes[prog->m_id].btn[prog->btn_on].vis_lit_on[2] = TRUE;
	select_it(0, LAST_ST_SEL, state);
	prog->redraw = 1;
	return (SUCCESS);
}

unsigned short			btn_light(t_vec mouse, t_btn *btn, int n_btn, t_prog *prog)
{
	unsigned short		res;
	unsigned short		i;

	if (!btn || !prog)
		return (FAIL);
	i = -1;
	res = FAIL;
	prog->btn_lit = -1;
	while (++i < n_btn)
	{
		if (btn[i].vis_lit_on[0] && mouse_over(btn[i].box, mouse) == TRUE)
		{
			btn[i].vis_lit_on[1] = TRUE;
			prog->btn_lit = i;
			prog->redraw = 1;
			res = SUCCESS;
		}
		else
		{
			if (btn[i].vis_lit_on[1] == TRUE)
				prog->redraw = 1;
			btn[i].vis_lit_on[1] = FALSE;
		}
	}
	return (res);
}

void					turn_btns_off(t_prog *prog)
{
	int 				i;

	if (prog->modes && prog->modes[prog->m_id].btn && prog->modes[prog->m_id].n_btn > 0)
	{
		i = 0;
		while (i < prog->modes[prog->m_id].n_btn)
		{
			prog->modes[prog->m_id].btn[i].vis_lit_on[2] = FALSE;
			i++;
		}
	}
	prog->btn_on = -1;
}

void					turn_btn_on(t_prog *prog, t_grid *grid)
{
	if (!prog || !grid)
		return ;
	if (within(prog->btn_on, -1, prog->modes[prog->m_id].n_btn) == TRUE)
		prog->modes[prog->m_id].btn[prog->btn_on].vis_lit_on[2] = FALSE;
	prog->btn_on = prog->btn_lit;
	prog->click = (t_vec){ 0, 0 };
	prog->modes[prog->m_id].btn[prog->btn_on].vis_lit_on[2] = TRUE;
	vec_set(grid->p, -1, -1, 2);
	prog->redraw = 1;
}

unsigned short			btn_press(t_prog *prog, t_grid *grid, t_media *media, int state)
{
	int					sector;

	if (!prog || !grid || !media)
		return (FAIL);
	turn_btn_on(prog, grid);
	if (prog->m_id == MODE_LEVELS)
		return (SUCCESS);
	if (state == NORMAL || state == SEC_SEARCH || state == WALL_SEARCH)
	{
		state = NORMAL;
		state = prog->btn_on == SEC_BTN ? SEC_SEARCH : state;
		state = prog->btn_on == WALL_BTN || prog->btn_on == DOOR_ADD_BTN? WALL_SEARCH : state;
		select_it(0, ST_SEL, state);
	}
	else if (state == SEC_EDIT || state == WALL_EDIT)
	{
		if (prog->btn_on == DESELECT_BTN)
		{
			zoom_to_map(media->worlds[media->w].n_v, media->worlds[media->w].vecs, grid);
			if (state == SEC_EDIT)
			{
				select_it(0, ST_SEL, SEC_SEARCH);
				select_it(0, S_SELECT, -1);
			}
			else
			{
				select_it(0, ST_SEL, WALL_SEARCH);
				select_it(0, W_SELECT, -1);
			}
		}
		else if ((state == SEC_EDIT && (prog->btn_on == FT_EDIT_BTN || prog->btn_on == CT_EDIT_BTN)) ||
				 (state == WALL_EDIT && prog->btn_on == WT_EDIT_BTN))
			prep_texture_edit(&media->worlds[media->w], prog, media->n_t);
		else if (state == WALL_EDIT && (prog->btn_on == W_PORTAL_BTN || prog->btn_on == W_DOOR_BTN))
		{
			edit_wall_type(prog->btn_on, &media->worlds[media->w]);
			turn_btns_off(prog);
		}
		else if (state == SEC_EDIT)
		{
			if (prog->btn_on == DEL_SEC_BTN)
			{
				zoom_to_map(media->worlds[media->w].n_v, media->worlds[media->w].vecs, grid);
				select_it(0, ST_SEL, SEC_SEARCH);
				delete_sector(select_it(1, S_SELECT, -1), &media->worlds[media->w]);
				select_it(0, S_SELECT, -1);
				validate_media(media);
				return (SUCCESS);
			}
			if (prog->btn_on == B_ITEM_DEL)
			{
				prog->btn_lit = -1;
				turn_btns_off(prog);
				delete_item(&media->worlds[media->w].sec[select_it(1, S_SELECT, -1)], select_it(1, I_SELECT, -1));
				select_it(0, I_SELECT, -1);
			}
			else if (within(prog->btn_on, F_UP_BTN - 1, C_DOWN_BTN + 1))
			{
				change_heights(prog->btn_on, media->worlds[media->w].sec);
				prog->btn_lit = -1;
				turn_btns_off(prog);
			}
			else if (prog->btn_on == B_ITEM_EDIT)
			{
				prog->btn_lit = -1;
				prog->modes[prog->m_id].btn[prog->btn_on].vis_lit_on[2] = FALSE;
				prog->btn_on = -1;
				prog->last = prog->m_id;
				prog->m_id = MODE_SEL_ITEM;
				sector = select_it(1, S_SELECT, -1);
				int item_i = select_it(1, I_SELECT, -1);
				if (within(sector, -1, media->worlds[media->w].n_s) && \
					within(item_i, -1, media->worlds[media->w].sec[sector].n_it) && \
					within(media->worlds[media->w].sec[sector].items[item_i].id, -1, media->n_itf))
				{
					select_it(0, SEL_I_SELECT, media->worlds[media->w].sec[sector].items[item_i].id);
					prog->btn_on = media->worlds[media->w].sec[sector].items[item_i].id;
					prog->modes[prog->m_id].btn[prog->btn_on].vis_lit_on[2] = TRUE;
				}
				prog->redraw = 0;
			}
		}
	}
	prog->click = (t_vec){ 0, 0 };
	return (SUCCESS);
}

unsigned short			manage_btn(t_media *media, t_prog *prog, t_grid *grid,
		t_vec mouse)
{
	int					state;

	state = select_it(1, ST_SEL, -1);
	if (prog->m_id == MODE_EDITOR && select_it(1, LAST_ST_SEL, -1) != state)
		return (btn_refresh(prog, state));
	if (prog->btn_lit != -1 && (prog->click.x || prog->click.y))
		return (btn_press(prog, grid, media, state));
	if (btn_light(mouse, prog->modes[prog->m_id].btn, \
	prog->modes[prog->m_id].n_btn, prog) == SUCCESS)
	{
		prog->redraw = 1;
		if (prog->m_id == MODE_EDITOR)
			return (SUCCESS);
	}
	return (3);
}


