
#include "builder.h"

void					render_main_menu(t_sdl *sdl, t_t *t, t_media *media, t_prog *prog)
{
	if (!sdl || !sdl->rend || !t || !media || !prog->modes || !prog->modes[prog->mode_id].buttons)
		return ;
	SDL_SetRenderDrawColor(sdl->rend, 55, 55, 55, 255);
	SDL_RenderClear(sdl->rend);
//	SDL_FillRect(sdl->surf, NULL, 0x00);
//	static SDL_Texture *f = NULL;
//	if (f == NULL)
//		f = load_texture("grey_panel.png", sdl);

//	SDL_RenderCopy(sdl->rend, f, 0, 0);
	render_buttons(prog->modes[prog->mode_id].buttons, sdl, prog->modes[prog->mode_id].n_buttons) ;

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

void					update_main_menu(t_sdl *sdl, t_t *t, t_media *media, t_prog *prog)
{
	if (!sdl || !t || !media || !prog->modes || !prog->modes[prog->mode_id].buttons)
		return;
	if (light_button(sdl, prog->modes[prog->mode_id].buttons,  prog->modes[prog->mode_id].n_buttons, prog) == SUCCESS) // when mouse is over a button
		return ;
	prog->button_lit = -1;
}

int						input_main_menu(t_sdl *sdl, float *grid_scale, t_media *media, t_prog *prog)
{
	int					quit;
	SDL_Event			event;
	int 				mode;

	quit = FALSE;
	mode = 0;
	if (!sdl || !grid_scale || !media)
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
			if (event.key.keysym.sym == 'w')
			{
				printf("%c\n", event.key.keysym.sym);
			}
			else if (event.key.keysym.sym == 's')
			{
				printf("%c\n", event.key.keysym.sym);
			}
			else if (event.key.keysym.sym == 'a')
			{
				printf("%c\n", event.key.keysym.sym);
			}
			else if (event.key.keysym.sym == 'd')
			{
				printf("%c\n", event.key.keysym.sym);
			}
			else if (event.key.keysym.sym == 'q')
			{
				printf("%c\n", event.key.keysym.sym);
			}
			else if (event.key.keysym.sym == 'e')
			{
				printf("%c\n", event.key.keysym.sym);
			}
			else if (event.key.keysym.sym == SDLK_RCTRL)
			{
				printf("%c\n", event.key.keysym.sym);
			}
			else if (event.key.keysym.sym == ' ')
			{
				printf("MAIN HEREEEEE\n");
				mode = 1;
				printf("%c\n", event.key.keysym.sym);

			}
			else if (event.key.keysym.sym == SDLK_ESCAPE)
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
					mode = 1;
				else if (prog->button_lit == 1)
				{
					quit = TRUE;
					break ;
				}

			}
		}
	}
	if (mode == 1)
		prog->mode_id = SUMMARY;
	prog->button_lit = -1;
	return (quit);
}