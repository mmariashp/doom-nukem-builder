/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   values.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mshpakov <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/09/28 20:08:58 by mshpakov          #+#    #+#             */
/*   Updated: 2019/09/28 20:09:00 by mshpakov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "builder.h"

void					render_values(int state, int n, t_value *values, t_sdl *sdl, SDL_Texture **t, t_texture *txtrs, int n_txtrs)
{
	int 				i;
	t_rec				box;

	if (!values || !sdl)
		return ;
	i = -1;
	n = clamp(n, 0, 255);
	while (++i < n)
	{
		box = layout_menu(2, (char)i);
		if (i == 0 && within(TXTR_RECT_GR_L, -1, TOT_TXTRS) && t && t[TXTR_RECT_GR_L])
			render_box(box, t[TXTR_RECT_GR_L], sdl->rend);
		else if (within(TXTR_RECT_GR, -1, TOT_TXTRS) && t && t[TXTR_RECT_GR])
			render_box(box, t[TXTR_RECT_GR], sdl->rend);
		write_text(menu_lines(state, i), sdl, box, EDIT_TEXT_COLOR, FALSE);
		box = layout_menu(5, (char)i);
		if (values[i].text)
			write_text(values[i].text, sdl, box, EDIT_TEXT_COLOR, FALSE);
		else if (values[i].media_prog == 0 && within(values[i].t_id, -1, n_txtrs) && txtrs[values[i].t_id].sdl_t)
		{
			box.w = box.h;
			render_box(box, txtrs[values[i].t_id].sdl_t, sdl->rend);
		}
		else if (values[i].media_prog == 1 && within(values[i].t_id, -1, TOT_TXTRS) && t[values[i].t_id])
		{
			box.w = box.h;
			render_box(box, t[values[i].t_id], sdl->rend);
		}
	}
}

t_value					*init_values(int n)
{
	t_value				*new;
	int 				i;

	new = ft_memalloc(sizeof(t_value) * n);
	if (!new)
		return (NULL);
	i = 0;
	while (i < n)
	{
		new[i].text = NULL;
		new[i].t_id = -1;
		new[i].media_prog = 0;
		i++;
	}
	return (new);
}

void					free_values(t_value *values, int n)
{
	int 				i;

	i = 0;
	if (!values)
		return ;
	while (i < n)
	{
		if (values[i].text)
		{
			free(values[i].text);
			values[i].text = NULL;
		}
		i++;
	}
	free(values);
	values = NULL;
}
