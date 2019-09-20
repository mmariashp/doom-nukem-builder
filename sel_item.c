
#include "builder.h"

void					render_item_icons(t_prog *prog, t_media *media, SDL_Renderer *rend)
{
	int 				i;
	t_rec				box;

	if (!rend || !media || !prog)
		return ;
	i = 0;
	while (i < prog->modes[prog->mode_id].n_buttons && i < media->n_itemfull)
	{
		box = prog->modes[prog->mode_id].buttons[i].box;
		box.x += box.w * 0.1;
		box.w = box.h;
		draw_items_or_free(0, media->itemfull[i].type, box, rend);
		i++;
	}
}

void					render_sel_item(t_sdl *sdl, t_grid *grid, t_media *media, t_prog *prog)
{
	if (!sdl || !media || !grid || prog->features[F_REDRAW] == 0)
		return ;
	SDL_SetRenderDrawColor(sdl->rend, 55, 55, 55, 255);
	SDL_RenderClear(sdl->rend);
	render_buttons(prog->modes[prog->mode_id].buttons, sdl->rend, prog->modes[prog->mode_id].n_buttons, prog->mode_id);
	render_item_icons(prog, media, sdl->rend);
	SDL_RenderPresent(sdl->rend);
	prog->features[F_REDRAW] = 1;
}

unsigned short			update_sel_item(t_sdl *sdl, t_grid *grid, t_media *media, t_prog *prog)
{
	if (!sdl || !grid || !media || !prog->modes || !prog->modes[prog->mode_id].buttons)
		return (FAIL);
	if (prog->mode_id != prog->last_mode_id)
	{
		prog->click = (t_vec2d){ 0, 0 };
		prog->button_lit = -1;
		prog->last_mode_id = prog->mode_id;
		return (SUCCESS);
	}
	if (light_button(sdl, prog->modes[prog->mode_id].buttons, prog->modes[prog->mode_id].n_buttons, prog) == SUCCESS) // when mouse is over a button
		return (SUCCESS);
	prog->button_lit = -1;
	return (SUCCESS);
}

int						input_sel_item(t_sdl *sdl, t_grid *grid, t_media *media, t_prog *prog)
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
		if( event.type == SDL_MOUSEMOTION || event.type == SDL_MOUSEBUTTONDOWN || event.type == SDL_MOUSEBUTTONUP )
		{
			if(event.type == SDL_MOUSEBUTTONDOWN)
			{
				if (prog->button_lit != -1)
				{
					selected_item(0, SEL_I_SELECT, prog->button_lit);
					int i = 0;
					while (i < prog->modes[prog->mode_id].n_buttons)
					{
						prog->modes[prog->mode_id].buttons[i].vis_lit_on[1] = FALSE;
						prog->modes[prog->mode_id].buttons[i].vis_lit_on[2] = FALSE;
						i++;
					}
					prog->last_mode_id = prog->mode_id;
					prog->mode_id = MODE_EDITOR;

					prog->click.x = 0;
					prog->click.y = 0;
					return (quit);
				}
			}
		}
	}
	return (quit);
}
