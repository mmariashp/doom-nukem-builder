
#include "builder.h"

void					render_walls(t_sdl *sdl, t_grid *grid, t_media *media, t_prog *prog)
{
	if (!sdl || !media || !grid || prog->features[F_REDRAW] == 0)
		return ;
	SDL_SetRenderDrawColor(sdl->rend, 55, 55, 55, 255);
	SDL_RenderClear(sdl->rend);
	render_grid(media->worlds[media->world_id], grid, prog, sdl->mouse);
	render_screen(sdl->rend, prog->screen);
	render_buttons(prog->modes[prog->mode_id].buttons, sdl, prog->modes[prog->mode_id].n_buttons);
	SDL_RenderPresent(sdl->rend);
	prog->features[F_REDRAW] = 0;

}

void					update_walls(t_sdl *sdl, t_grid *grid, t_media *media, t_prog *prog)
{
	if (!sdl || !media || !grid)
		return;
	if (prog->button_lit != -1 && (prog->move.x || prog->move.y)) // when pressing an on screen button
	{
		if (prog->button_on != -1)
			prog->modes[prog->mode_id].buttons[prog->button_on].vis_lit_on[2] = FALSE;
		prog->button_on = prog->button_lit;
		prog->modes[prog->mode_id].buttons[prog->button_on].vis_lit_on[2] = TRUE;
		prog->features[F_REDRAW] = 1;
	}
	if (light_button(sdl, prog->modes[prog->mode_id].buttons,  prog->modes[prog->mode_id].n_buttons, prog) == SUCCESS) // when mouse is over a button
	{
		prog->features[F_REDRAW] = 1;
	}
	if (prog->zoom != 0)
	{
		zoom_grid(prog, sdl->mouse, grid);
	}
	update_sector_status(media->worlds[media->world_id].sectors, media->worlds[media->world_id].walls, media->worlds[media->world_id].vertices, media->worlds[media->world_id].n_sectors);
}

int						input_walls(t_sdl *sdl, t_grid *grid, t_media *media, t_prog *prog)
{
	int					quit;
	SDL_Event			event;

	quit = FALSE;
	if (!sdl || !media || !grid)
		return (TRUE);
	while(SDL_PollEvent(&event))
	{
		SDL_GetMouseState(&sdl->mouse.x, &sdl->mouse.y);
		if (event.type == SDL_QUIT)
		{
			quit = TRUE;
			break ;
		}
		else if (event.type == SDL_KEYUP || event.type == SDL_KEYDOWN)
		{
			if (event.key.keysym.sym == SDLK_ESCAPE)
			{
				quit = TRUE;
				break ;
			}
		}
		if(event.type == SDL_MOUSEWHEEL)
		{
			if(event.wheel.y > 0) // scroll up
				prog->zoom++;
			else if(event.wheel.y < 0) // scroll down
				prog->zoom--;
		}
		if( event.type == SDL_MOUSEMOTION || event.type == SDL_MOUSEBUTTONDOWN || event.type == SDL_MOUSEBUTTONUP )
		{
			if(event.type == SDL_MOUSEBUTTONDOWN)
				prog->move = sdl->mouse;
			if(event.type == SDL_MOUSEBUTTONUP)
			{
				if (prog->button_lit == W_BACK_BUTTON)
				{
					prog->last_mode_id = prog->mode_id;
					prog->mode_id = MODE_EDITOR;
					prog->modes[prog->mode_id].buttons[prog->button_lit].vis_lit_on[2] = FALSE;
					prog->button_lit = -1;
					prog->button_on = -1;
					return (quit);
				}
				prog->move.x = 0;
				prog->move.y = 0;
			}
		}
	}
	return (quit);
}