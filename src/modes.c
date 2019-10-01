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

void					free_btn(t_btn *btn, int n)
{
	int 				i;

	i = 0;
	if (!btn)
		return ;
	while (i < n)
	{
		if (btn[i].text)
		{
			free(btn[i].text);
			btn[i].text = NULL;
		}
		i++;
	}
	free(btn);
	btn = NULL;
}

void					free_modes(t_mode *modes)
{
	int 				i;

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

t_btn				*init_btn(int n_btn)
{
	int 				i;
	t_btn			*btn;

	btn = (t_btn *)ft_memalloc(sizeof(t_btn) * n_btn);
	if (!btn)
		return (NULL);
	ft_bzero(btn, sizeof(t_btn) * n_btn);
	i = 0;
	while (i < n_btn)
	{
		btn[i].vis_lit_on[0] = TRUE;
		btn[i].vis_lit_on[1] = FALSE;
		btn[i].vis_lit_on[2] = FALSE;
		btn[i].box = (t_rec){ 0, 0, 10, 10 };
		btn[i].text_color = 0;
		btn[i].text = NULL;
		btn[i].reg_i = -1;
		btn[i].lit_i = -1;
		i++;
	}
	return (btn);
}

unsigned short			main_menu_btn(t_btn *btn)
{
	t_rec				box;
	int 				i;

	if (!btn)
		return (FAIL);
    box.w = W_W / 4;
    box.h = (box.w + 20) * N_MM_BTNS / 3;
	box.x = (W_W  - box.w) / 2;
	box.y = (W_H  - box.h) / 2;
	distribute_btn_v(btn, (t_vec){ 0, N_MM_BTNS }, box, 20);
	i = 0;
	while (i < N_MM_BTNS)
	{
		btn[i].reg_i = TXTR_RECG;
		btn[i].lit_i = TXTR_RECG_L;
		i++;
	}
	btn[0].text = ft_strdup("START");
	btn[1].text = ft_strdup("QUIT");
	return (SUCCESS);
}

unsigned short			levels_btn(t_btn *btn, t_world *worlds, int n_worlds)
{
	t_rec				box;
	int 				i;
	char				*s;
	char 				*tmp;
	int 				n_levels = n_worlds + 1;
	int 				n_btn = n_levels * 3 - 2;

	if (!btn || !worlds)
		return (FAIL);
	box.w = W_W / 4;
	box.h = (box.w + 20) * n_levels / 3;
	box.x = (W_W  - box.w) / 2;
	box.y = (W_H  - box.h) / 2;
	distribute_btn_v(btn, (t_vec){ 0, n_levels }, box, 20);
	i = 0;
	s = NULL;
	while (i < n_levels)
	{
		if (i == n_worlds)
		{
			btn[i].text = NULL;
			btn[i].reg_i = TXTR_ADD_MAP;
			btn[i].lit_i = TXTR_ADD_MAP_L;
			btn[i].box.x += (btn[i].box.w - btn[i].box.h) / 2;
			btn[i].box.w = btn[i].box.h;
		}
		else
		{
			tmp = ft_itoa(i);
			s = ft_strjoin(tmp, ". ");
			free(tmp);
			if (s)
			{
				btn[i].text = ft_strjoin(s, worlds[i].name);
				free(s);
			}
			btn[i].reg_i = TXTR_RECG;
			btn[i].lit_i = TXTR_RECG_L;
		}
		i++;
	}
	int j = 0;
	while (i + 1 < n_btn)
	{
		btn[i].reg_i = TXTR_EDIT;
		btn[i].lit_i = TXTR_EDIT_L;
		btn[i + 1].reg_i = TXTR_DEL;
		btn[i + 1].lit_i = TXTR_DEL_L;
		btn[i].box = btn[j].box;
		btn[i + 1].box = btn[j].box;
		btn[i].box.x += btn[j].box.w + 10;
		btn[i + 1].box.x += btn[j].box.w + 50;
		btn[i].box.w = 30;
		btn[i + 1].box.w = 30;
		btn[i].box.h = 30;
		btn[i + 1].box.h = 30;
		i += 2;
		j++;
	}
	return (SUCCESS);
}

unsigned short			textures_btn(t_btn *btn, int n_t)
{
    int 				i;

    if (!btn || n_t < 1)
    	return (FAIL);
    get_txtr_btn_boxes(btn, n_t, 0);
    i = -1;
    while (++i < n_t)
    {
        btn[i].vis_lit_on[0] = TRUE;
		btn[i].reg_i = TXTR_PANEL_GR;
		btn[i].lit_i = TXTR_RECG_L;
    }
    return (SUCCESS);
}

unsigned short			sel_item_btn(t_btn *btn, t_it_f *it_f, int n_itf)
{
	t_rec				button_box;
	int 				i;

	if (!btn || !it_f)
		return (FAIL);
	button_box = n_itf > 25 ? (t_rec){ W_W * 0.1, W_H * 0.05, W_W * 0.4,\
	W_H * 0.9 / 25 } : (t_rec){ W_W * 0.3, (W_H - (W_H * 0.9 / 25) * n_itf)\
	/ 2, W_W * 0.4, W_H * 0.9 / 25 };
	i = -1;
	while (++i < n_itf)
	{
		btn[i].vis_lit_on[0] = TRUE;
		btn[i].reg_i = TXTR_RECG;
		btn[i].lit_i = TXTR_RECG_L;
		if (it_f[i].name)
			btn[i].text = ft_strdup(it_f[i].name);
		btn[i].box = button_box;
		button_box.y += button_box.h;
		if (i == 25)
		{
			button_box.x = W_W * 0.5;
			button_box.y = W_H * 0.05;
		}
	}
	return (SUCCESS);
}

void                    refresh_level_list(t_media *media, t_mode *mode)
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
	modes[MODE_EDITOR].n_btn = TOTAL_EDITOR_BTNS;
	modes[MODE_SEL_ITEM].n_btn = media->n_itf;
}

unsigned short			init_modes(t_media *media, t_prog *prog)
{
	int 				i;

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
