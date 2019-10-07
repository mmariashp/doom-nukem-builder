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

int 					default_heights(char set_get, char of_what, \
int world_no, int value)
{
	static int 			t[2][MAX_LEVELS] = { { 0 } };

	if (set_get == 1 && within(world_no, -1, MAX_LEVELS) &&
		within(of_what, -1, 2))
		return (t[(int)of_what][world_no]);
	else if (set_get == 0 && within(world_no, -1, MAX_LEVELS))
	{
		if (within(of_what, -1, 2))
		{
			t[(int)of_what][world_no] = value;
			return (t[(int)of_what][world_no]);
		}
		else
		{
			t[0][world_no] = 0;
			t[1][world_no] = 20;
		}
	}
	return (-1);
}

int 					default_texture(char set_get, char of_what, \
int world_no, int value)
{
	static int 			t[5][MAX_LEVELS] = { { 0 } };

	if (set_get == 1 && within(world_no, -1, MAX_LEVELS) &&
		within(of_what, -1, 5))
		return (t[(int)of_what][world_no]);
	else if (set_get == 0 && within(world_no, -1, MAX_LEVELS))
	{
		if (within(of_what, -1, 5))
		{
			t[(int)of_what][world_no] = value;
			return (t[(int)of_what][world_no]);
		}
		else
		{
			t[0][world_no] = DEFAULT_WALL_T;
			t[1][world_no] = DEFAULT_FLR_T;
			t[2][world_no] = DEFAULT_CEIL_T;
			t[3][world_no] = DEFAULT_DOOR_T;
			t[4][world_no] = DEFAULT_TRAN_T;
		}
	}
	return (-1);
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
		t = default_texture(1, i, media->w, -1);
		tmp = settings_boxes(3, i + 1);
		tmp.x -= tmp.w * 1.1;
		SDL_RenderCopy(sdl->rend, media->txtr[t].sdl_t, NULL, \
		&(SDL_Rect){ tmp.x, tmp.y, tmp.w, tmp.h });
	}
	while (i < 7)
	{
		t = default_heights(1, i - 5, media->w, -1);
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
	if (!sdl || !grid || !media || !prog->modes || !prog->modes[prog->m_id].btn)
		return (FAIL);
	if (prog->m_id != prog->last)
	{
		prog->click = (t_vec){ 0, 0 };
		prog->btn_lit = -1;
		prog->last = prog->m_id;
		return (SUCCESS);
	}
	if (btn_light(sdl->mouse, prog->modes[prog->m_id].btn, \
	prog->modes[prog->m_id].n_btn, prog) == SUCCESS)
		return (SUCCESS);
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
		if (event.type == SDL_MOUSEBUTTONDOWN && prog->btn_lit != -1)
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