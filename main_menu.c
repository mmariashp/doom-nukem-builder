
#include "builder.h"

void					render_main_menu(t_sdl *sdl, t_t *t, t_media *media)
{
	if (!sdl || !t || !media || !sdl->modes || !sdl->modes[sdl->mode_id].buttons)
		return ;
//	SDL_SetRenderDrawColor(sdl->rend, 0, 0, 0, 255);
	SDL_RenderClear(sdl->rend);
//	SDL_FillRect(sdl->surf, NULL, 0x00);

	render_buttons(sdl->modes[sdl->mode_id].buttons, sdl);

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

void					update_main_menu(t_sdl *sdl, t_t *t, t_media *media)
{
	if (!sdl || !t || !media || !sdl->modes || !sdl->modes[sdl->mode_id].buttons)
		return;
	if (light_button(sdl) == SUCCESS)
		return ;
	sdl->button_lit = -1;
}

int						input_main_menu(t_sdl *sdl, float *grid_scale, t_media *media)
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
				if (sdl->button_lit == 0)
					mode = 1;
				else if (sdl->button_lit == 1)
				{
					quit = TRUE;
					break ;
				}

			}
		}
	}
	if (mode == 1)
		sdl->mode_id = SUMMARY;
	sdl->button_lit = -1;
	return (quit);
}