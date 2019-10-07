/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main_menu.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mshpakov <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/09/24 16:51:31 by mshpakov          #+#    #+#             */
/*   Updated: 2019/09/24 16:51:34 by mshpakov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "builder.h"

void					render_alien(SDL_Renderer *rend)
{
	static char			files[1][30] = { "./buttons/maptools.png" };
	SDL_Texture			*alien;
	t_vec				size;

	alien = load_texture(files[0], rend, &size);
	SDL_RenderCopy(rend, alien, NULL, &(SDL_Rect){ (W_W - size.x * 3) / 2, \
	100, size.x * 3, size.y * 3 });
	SDL_DestroyTexture(alien);
}

void					r_mainmenu(t_sdl *sdl, t_grid *grid, t_media *media, \
																t_prog *prog)
{
	if (!sdl || !sdl->rend || !grid || !media || !prog->modes ||
	!prog->modes[prog->m_id].btn)
		return ;
	SDL_SetRenderDrawColor(sdl->rend, 55, 55, 55, 255);
	SDL_RenderClear(sdl->rend);
	render_btns(prog, sdl);
	render_alien(sdl->rend);
	SDL_RenderPresent(sdl->rend);
}

unsigned short			u_mainmenu(t_sdl *sdl, t_grid *grid, t_media *media, \
																t_prog *prog)
{
	if (!sdl || !grid || !media || !prog->modes ||
	!prog->modes[prog->m_id].btn)
		return (FAIL);
	if (btn_light(sdl->mouse, prog->modes[prog->m_id].btn,\
	prog->modes[prog->m_id].n_btn, prog) == SUCCESS)
		return (SUCCESS);
	prog->btn_lit = -1;
	return (SUCCESS);
}

int						i_mainmenu(t_sdl *sdl, t_grid *grid, t_media *media, \
																t_prog *prog)
{
	int					quit;
	SDL_Event			event;

	quit = sdl && grid && media && prog ? FALSE : TRUE;
	while (SDL_PollEvent(&event) && quit == FALSE)
	{
		SDL_GetMouseState(&sdl->mouse.x, &sdl->mouse.y);
		if (event.type == SDL_QUIT || (event.type == SDL_KEYUP &&
				event.key.keysym.sym == SDLK_ESCAPE))
			quit = TRUE;
		else if (event.type == SDL_MOUSEBUTTONUP)
		{
			if (prog->btn_lit == 0)
				prog->m_id = MODE_LEVELS;
			else if (prog->btn_lit == 1)
				quit = TRUE;
		}
	}
	prog->btn_lit = -1;
	return (quit);
}
