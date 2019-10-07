/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   modes.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mshpakov <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/09/28 20:10:58 by mshpakov          #+#    #+#             */
/*   Updated: 2019/09/28 20:10:59 by mshpakov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "builder.h"

void					switch_mode(t_prog *prog, int new_m_id, int new_state)
{
	if (prog)
	{
		prog->modes[prog->m_id].btn[prog->btn_on].vis_lit_on[2] = FALSE;
		prog->last = prog->m_id;
		prog->m_id = new_m_id;
		prog->btn_lit = -1;
		select_it(0, ST_SEL, new_state);
		prog->click = (t_vec){ 0, 0 };
	}
}

void					free_modes(t_mode *modes)
{
	int					i;

	if (!modes)
		return ;
	i = -1;
	while (++i < N_MODES)
	{
		if (modes[i].btn)
			free_btn(modes[i].btn, modes[i].n_btn);
	}
	free(modes);
	modes = NULL;
}

void					refresh_level_list(t_media *media, t_mode *mode)
{
	if (!media || !mode)
		return ;
	if (mode->n_btn == media->n_worlds + 1)
		return ;
	if (mode->btn)
		free_btn(mode->btn, mode->n_btn);
	mode->n_btn = (media->n_worlds + 1) * 3;
	mode->btn = init_btn(mode->n_btn);
	if (!mode->btn)
		return ;
	levels_btn(mode->btn, media->worlds, media->n_worlds);
}

void					get_loop_fun(t_mode *modes, t_media *media)
{
	if (!modes || !media)
		return ;
	modes[MODE_MAIN_MENU].input = &i_mainmenu;
	modes[MODE_MAIN_MENU].update = &u_mainmenu;
	modes[MODE_MAIN_MENU].render = &r_mainmenu;
	modes[MODE_LEVELS].input = &i_levels;
	modes[MODE_LEVELS].update = &u_levels;
	modes[MODE_LEVELS].render = &r_levels;
	modes[MODE_EDITOR].input = &i_editor;
	modes[MODE_EDITOR].update = &u_editor;
	modes[MODE_EDITOR].render = &r_editor;
	modes[MODE_TEXTURES].input = &i_textures;
	modes[MODE_TEXTURES].update = &u_textures;
	modes[MODE_TEXTURES].render = &r_textures;
	modes[MODE_SEL_ITEM].input = &i_sel_item;
	modes[MODE_SEL_ITEM].update = &u_sel_item;
	modes[MODE_SEL_ITEM].render = &r_sel_item;
	modes[MODE_MAIN_MENU].n_btn = N_MM_BTNS;
	modes[MODE_LEVELS].n_btn = (media->n_worlds + 1) * 3;
	modes[MODE_EDITOR].n_btn = 8;
	modes[MODE_TEXTURES].n_btn = media->n_t;
	modes[MODE_MAIN_MENU].n_btn = N_MM_BTNS;
	modes[MODE_LEVELS].n_btn = (media->n_worlds + 1) * 3;
	modes[MODE_EDITOR].n_btn = TOT_EDIT_BTNS;
	modes[MODE_SEL_ITEM].n_btn = media->n_itf;
}

unsigned short			init_modes(t_media *media, t_prog *prog)
{
	int					i;

	if (!media || !prog || !(prog->modes = \
	(t_mode *)ft_memalloc(sizeof(t_mode) * N_MODES)))
		return (FAIL);
	ft_bzero(prog->modes, sizeof(t_mode) * N_MODES);
	get_loop_fun(prog->modes, media);
	i = 0;
	while (i < N_MODES)
	{
		prog->modes[i].btn = NULL;
		if (prog->modes[i].n_btn > 0 && !(prog->modes[i].btn = \
		init_btn(prog->modes[i].n_btn)))
			return (FAIL);
		i++;
	}
	main_menu_btn(prog->modes[MODE_MAIN_MENU].btn);
	levels_btn(prog->modes[MODE_LEVELS].btn, media->worlds, media->n_worlds);
	textures_btn(prog->modes[MODE_TEXTURES].btn, media->n_t);
	sel_item_btn(prog->modes[MODE_SEL_ITEM].btn, media->it_f, media->n_itf);
	return (SUCCESS);
}
