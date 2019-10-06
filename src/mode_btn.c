/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mode_btn.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mshpakov <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/09/28 20:10:58 by mshpakov          #+#    #+#             */
/*   Updated: 2019/09/28 20:10:59 by mshpakov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "builder.h"

unsigned short			main_menu_btn(t_btn *btn)
{
	t_rec				box;
	int					i;

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
