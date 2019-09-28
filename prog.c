/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   prog.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mshpakov <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/09/28 17:58:28 by mshpakov          #+#    #+#             */
/*   Updated: 2019/09/28 17:58:29 by mshpakov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "builder.h"

void					free_prog_txtr(SDL_Texture **t)
{
	int					i;

	if (t)
	{
		i = 0;
		while (i < TOTAL_TXTRS)
		{
			if (t[i])
				SDL_DestroyTexture(t[i]);
			i++;
		}
		free(t);
	}
}

SDL_Texture				**get_prog_txtr(SDL_Renderer *rend)
{
	int					i;
	SDL_Texture			**t;
	char				*tmp;
	static char			name[TOTAL_TXTRS][30] = { "rct_g", "rct_g_l", "rct_y", \
	"rct_y_l", "panel", "panel_l", "yes", "yes_l", "no", "no_l", "move", \
	"move_l", "draw", "draw_l", "distort", "distort_l", "iso", "iso_l", "sec", \
	"sec_l", "wall", "wall_l", "d_add", "d_add_l", "d_del", "d_del_l", \
	"player", "player_l", "save", "save_l", "exit", "exit_l", "back", "back_l",\
	"del", "del_l", "edit", "edit_l", "up", "up_l", "down", "down_l", "coin", \
	"key", "object", "enemy", "super", "health", "ammo", "light", "door" };

	if (!rend || !(t = ft_memalloc(sizeof(SDL_Texture *) * TOTAL_TXTRS)))
		return (NULL);
	i = -1;
	while (++i < TOTAL_TXTRS)
	{
		if ((tmp = get_full_path(name[i], ".png", "./buttons/")))
		{
			t[i] = load_texture(tmp, rend, 0);
			free(tmp);
			if (t[i])
				continue ;
		}
		free_prog_txtr(t);
		return (NULL);
	}
	return (t);
}

void					free_prog(t_prog *prog)
{
	if (!prog)
		return ;
	if (prog->modes)
	{
		free_modes(prog->modes);
		prog->modes = NULL;
	}
	if (prog->screen)
	{
		free_tab((void **)prog->screen, WIN_W);
		prog->screen = NULL;
	}
	if (prog->t)
	{
		free_prog_txtr(prog->t);
		prog->t = NULL;
	}
	free(prog);
	prog = NULL;
}

unsigned short			init_prog_values(t_prog *prog, SDL_Renderer *rend)
{
	int 				k;

	if (!prog)
		return (FAIL);
	if (!(prog->screen = (int **)ft_memalloc(sizeof(int *) * WIN_W)))
		return (FAIL);
	k = -1;
	while (++k < WIN_W)
	{
		if (!(prog->screen[k] = (int *)ft_memalloc(sizeof(int) * WIN_H)))
			return (FAIL);
		ft_bzero(prog->screen[k], sizeof(int) * WIN_H);
	}
	if (!(prog->t = get_prog_txtr(rend)))
		return (FAIL);
	prog->m_id = 0;
	prog->last = 0;
	prog->modes = NULL;
	prog->btn_lit = -1;
	prog->btn_on = -1;
	prog->zoom = 0;
	prog->click = (t_vec2d){ 0, 0 };
	prog->move = (t_vec2d){ 0, 0 };
	prog->redraw = TRUE;
	return (SUCCESS);
}

t_prog					*get_prog(SDL_Renderer *rend)
{
	t_prog				*prog;

	if (!rend || !(prog = (t_prog *)ft_memalloc(sizeof(t_prog))))
		return (NULL);
	ft_bzero(prog, sizeof(t_prog));
	if (init_prog_values(prog, rend) == FAIL)
	{
		free_prog(prog);
		return (NULL);
	}
	return (prog);
}
