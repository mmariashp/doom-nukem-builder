
#include "builder.h"

void					render_main_menu(t_sdl *sdl, t_grid *grid, t_media *media, t_prog *prog)
{
	if (!sdl || !sdl->rend || !grid || !media || !prog->modes || !prog->modes[prog->mode_id].buttons)
		return ;
	SDL_SetRenderDrawColor(sdl->rend, 55, 55, 55, 255);
	SDL_RenderClear(sdl->rend);
//	SDL_FillRect(sdl->surf, NULL, 0x00);
//	static SDL_Texture *f = NULL;
//	if (f == NULL)
//		f = load_texture("grey_panel.png", sdl);

//	SDL_RenderCopy(sdl->rend, f, 0, 0);
	render_buttons(prog->modes[prog->mode_id].buttons, sdl->rend, prog->modes[prog->mode_id].n_buttons, prog->mode_id, prog->t) ;

//	sdl->t = SDL_CreateTextureFromSurface(sdl->rend, sdl->surf);
//	SDL_RenderCopy(sdl->rend, sdl->t, NULL, NULL);
//	SDL_DestroyTexture(sdl->t);
	SDL_RenderPresent(sdl->rend);
}

unsigned short			mouse_over(t_rec box, t_vec2d mouse)
{
	if (mouse.x <= box.x || mouse.x >= box.x + box.w)
		return (FALSE);
	if (mouse.y <= box.y || mouse.y >= box.y + box.h)
		return (FALSE);
	return (TRUE);
}

unsigned short			update_main_menu(t_sdl *sdl, t_grid *grid, t_media *media, t_prog *prog)
{
	if (!sdl || !grid || !media || !prog->modes || !prog->modes[prog->mode_id].buttons)
		return (FAIL);
	if (light_button(sdl, prog->modes[prog->mode_id].buttons,  prog->modes[prog->mode_id].n_buttons, prog) == SUCCESS) // when mouse is over a button
		return (SUCCESS);
	prog->button_lit = -1;
	return (SUCCESS);
}

int						input_main_menu(t_sdl *sdl, t_grid *grid, t_media *media, t_prog *prog)
{
	int					quit;
	SDL_Event			event;

	quit = FALSE;
	if (!sdl || !grid || !media)
		return (TRUE);
	while(SDL_PollEvent(&event))
	{
		if (event.type == SDL_QUIT)
		{
			quit = TRUE;
			break ;
		}
		else if (event.type == SDL_KEYUP)
		{
			if (event.key.keysym.sym == SDLK_ESCAPE)
			{
				quit = TRUE;
				break ;
			}
		}
		if( event.type == SDL_MOUSEMOTION || event.type == SDL_MOUSEBUTTONDOWN || event.type == SDL_MOUSEBUTTONUP )
		{
			SDL_GetMouseState(&sdl->mouse.x, &sdl->mouse.y);
			if (event.type == SDL_MOUSEBUTTONUP)
			{
				if (prog->button_lit == 0)
					prog->mode_id = MODE_SUMMARY;
				else if (prog->button_lit == 1)
					quit = TRUE;
				return (quit);
			}
		}
	}
	prog->button_lit = -1;
	return (quit);
}