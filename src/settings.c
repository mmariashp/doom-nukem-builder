/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   settings.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mshpakov <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/10/07 17:32:09 by mshpakov          #+#    #+#             */
/*   Updated: 2019/10/07 19:35:31 by mshpakov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "builder.h"

int 					default_heights(char set_get, char of_what, int value)
{
	static int 			t[2] = { 0 };

	if (set_get == 1 && within(of_what, -1, 2))
		return (t[(int)of_what]);
	else if (set_get == 0)
	{
		if (within(of_what, -1, 2))
		{
			t[(int)of_what] = value;
			return (t[(int)of_what]);
		}
		else
		{
			t[0] = 0;
			t[1] = 20;
		}
	}
	return (-1);
}

int 					default_texture(char set_get, char of_what, int value)
{
	static int 			t[5] = { 0 };

	if (set_get == 1 && within(of_what, -1, 5))
		return (t[(int)of_what]);
	else if (set_get == 0)
	{
		if (within(of_what, -1, 5))
		{
			t[(int)of_what] = value;
			return (t[(int)of_what]);
		}
		else
		{
			t[0] = def_wall_t;
			t[1] = def_floor_t;
			t[2] = def_ceil_t;
			t[3] = def_door_t;
			t[4] = def_tran_t;
		}
	}
	return (-1);
}

unsigned short			edit_def_texture(int n_t, t_texture *txtr)
{
	int					of_what;
	int					texture;

	if (!txtr)
		return (FAIL);

	if (within((texture = select_it(1, T_SELECT, -1)), -1, n_t) &&
	within((of_what = select_it(1, DEF_T_SELECT, -1)), -1, 5))
		default_texture(0, (char)of_what, texture);
	return (SUCCESS);
}

void					change_def_textures(t_prog *prog, int n_t)
{
	int					t;
	int					of_what;

	if (!prog || !prog->modes)
		return ;
	of_what = prog->btn_on - 1;
	select_it(0, DEF_T_SELECT, of_what);
	t = default_texture(1, (char)of_what, -1);
	turn_btns_off(prog);
	prog->btn_lit = -1;
	prog->last = MODE_SETTINGS;
	prog->m_id = MODE_TEXTURES;
	if (within(t, -1, get_min(n_t, prog->modes[prog->m_id].n_btn)))
	{
		prog->modes[prog->m_id].btn[t].vis_lit_on[2] = TRUE;
		select_it(0, T_SELECT, (prog->btn_on = t));
	}
}

void					change_def_heights(int b)
{
	int					f_shift;
	int					c_shift;
	int 				floor;
	int 				ceil;

	f_shift = 0;
	c_shift = 0;
	floor = default_heights(1, 0, -1);
	ceil = default_heights(1, 1, -1);
	if (b == S_F_UP_BTN)
		f_shift++;
	else if (b == S_F_DOWN_BTN)
		f_shift--;
	else if (b == S_C_UP_BTN)
		c_shift++;
	else if (b == S_C_DOWN_BTN)
		c_shift--;
	floor += f_shift;
	ceil += c_shift;
	floor = clamp(floor, MIN_HEIGHT, MAX_HEIGHT);
	ceil = clamp(ceil, MIN_HEIGHT, MAX_HEIGHT);
	default_heights(0, 0, floor);
	default_heights(0, 1, ceil);
}

void					render_defaults(t_sdl *sdl, t_media *media)
{
	int 				i;
	int 				t;
	t_rec				tmp;
	char 				*str;

	if (!sdl || !media)
		return ;
	i = -1;
	while (++i < 5)
	{
		t = default_texture(1, i, -1);
		tmp = settings_boxes(3, i + 1);
		tmp.x -= tmp.w * 1.1;
		SDL_RenderCopy(sdl->rend, media->txtr[t].sdl_t, NULL, \
		&(SDL_Rect){ tmp.x, tmp.y, tmp.w, tmp.h });
	}
	while (i < 7)
	{
		t = default_heights(1, i - 5, -1);
		tmp = settings_boxes(2, i);
		tmp.x += tmp.w * 0.8;
		tmp.w = tmp.h;
		str = ft_itoa(t);
		if (str)
		{
			write_txt(str, sdl, (t_txtb){ tmp, TRUE, ED_CLR });
			free(str);
		}
		i++;
	}
}

void					render_settings_layout(t_sdl *sdl, t_prog *prog)
{
	int 				i;
	static char 		h[3][20] = { "DEFAULT VALUES", "TEXTURES", "HEIGHTS" };
	static char 		v[7][20] = { "wall", "floor", "ceiling", "door", \
	"transparent wall", "floor", "ceiling" };
	t_rec				tmp;

	tmp = settings_boxes(1, 0);
	SDL_RenderCopy(sdl->rend, prog->t[TXTR_PANEL_GR], NULL, \
	&(SDL_Rect){ tmp.x, tmp.y, tmp.w, tmp.h });
	tmp = settings_boxes(1, 1);
	SDL_RenderCopy(sdl->rend, prog->t[TXTR_PANEL_GR], NULL, \
	&(SDL_Rect){ tmp.x, tmp.y, tmp.w, tmp.h });
	i = -1;
	while (++i < 3)
		write_txt(h[i], sdl, (t_txtb){ settings_boxes(0, i), FALSE, WHITE });
	i = -1;
	while (++i < 7)
		write_txt(v[i], sdl, (t_txtb){ settings_boxes(2, i), FALSE, ED_CLR });
}

void					r_settings(t_sdl *sdl, t_grid *grid, t_media *media, \
t_prog *prog)
{
	if (!sdl || !grid || !media || !prog)
		return ;
	SDL_SetRenderDrawColor(sdl->rend, 55, 55, 55, 255);
	SDL_RenderClear(sdl->rend);
	render_settings_layout(sdl, prog);
	render_defaults(sdl, media);
	render_btns(prog, sdl);
	SDL_RenderPresent(sdl->rend);
}

unsigned short			u_settings(t_sdl *sdl, t_grid *grid, t_media *media, \
t_prog *prog)
{
	int					tmp2;

	if (!sdl || !grid || !media || !prog->modes || !prog->modes[prog->m_id].btn)
		return (FAIL);
	if (prog->last == MODE_TEXTURES)
		edit_def_texture(media->n_t, media->txtr);
	if (prog->m_id != prog->last)
	{
		prog->click = (t_vec){ 0, 0 };
		prog->btn_lit = -1;
		prog->last = prog->m_id;
		return (SUCCESS);
	}
	if ((tmp2 = manage_btn(media, prog, grid, sdl->mouse)) < 2)
		return (tmp2);
	prog->last = prog->m_id;
	prog->btn_lit = -1;
	return (SUCCESS);
}

int						i_settings(t_sdl *sdl, t_grid *grid, t_media *media, \
t_prog *prog)
{
	int					quit;
	SDL_Event			event;

	quit = FALSE;
	if (!sdl || !media || !grid || !prog)
		return (TRUE);
	while (SDL_PollEvent(&event))
	{
		SDL_GetMouseState(&sdl->mouse.x, &sdl->mouse.y);
		if (event.type == SDL_QUIT || (event.type == SDL_KEYUP &&
									   event.key.keysym.sym == SDLK_ESCAPE))
			return (TRUE);
		else if (event.type == SDL_MOUSEBUTTONDOWN)
			prog->click = sdl->mouse;
		else if (event.type == SDL_MOUSEBUTTONUP && prog->btn_on == S_BACK_BTN)
		{
			turn_btns_off(prog);
			prog->last = prog->m_id;
			prog->m_id = MODE_EDITOR;
			prog->click = (t_vec){ 0, 0 };
			return (quit);
		}
	}
	return (quit);
}