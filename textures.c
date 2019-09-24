
#include "builder.h"

void					render_texture_icons(t_button *buttons, t_sdl *sdl, int n_buttons, t_texture *txtrs)
{
	if (!buttons || n_buttons < 1 || !sdl || !txtrs)
		return ;
	SDL_Rect rect;
	t_rec				text;
	int i = 0;

	while (i < n_buttons)
	{
		text = buttons[i].box;
		text.h /= 6;

		rect = (SDL_Rect){ buttons[i].box.x + buttons[i].box.w * 0.1, buttons[i].box.y + buttons[i].box.h * 0.15,
						   buttons[i].box.w * 0.8, buttons[i].box.h * 0.8 };
		if (txtrs[i].sdl_t)
			SDL_RenderCopy(sdl->rend, txtrs[i].sdl_t, NULL, &rect);
		if (txtrs[i].name)
			write_text(txtrs[i].name, sdl->rend, text, 0, TRUE);
		i++;
	}
}

void					render_textures(t_sdl *sdl, t_grid *grid, t_media *media, t_prog *prog)
{
    if (!sdl || !media || !grid || prog->features[F_REDRAW] == 0)
        return ;
    SDL_SetRenderDrawColor(sdl->rend, 55, 55, 55, 255);
    SDL_RenderClear(sdl->rend);
    render_buttons(prog->modes[prog->mode_id].buttons, sdl->rend, prog->modes[prog->mode_id].n_buttons, prog->mode_id, prog->t);
    render_texture_icons(prog->modes[prog->mode_id].buttons, sdl, prog->modes[prog->mode_id].n_buttons, media->txtrs);
    SDL_RenderPresent(sdl->rend);
    prog->features[F_REDRAW] = 1;
}

unsigned short			update_textures(t_sdl *sdl, t_grid *grid, t_media *media, t_prog *prog)
{
    if (!sdl || !grid || !media || !prog->modes || !prog->modes[prog->mode_id].buttons)
        return (FAIL);
    if (light_button(sdl, prog->modes[prog->mode_id].buttons, prog->modes[prog->mode_id].n_buttons, prog) == SUCCESS) // when mouse is over a button
        return (SUCCESS);
    prog->button_lit = -1;
	return (SUCCESS);
}

int						input_textures(t_sdl *sdl, t_grid *grid, t_media *media, t_prog *prog)
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
            if(event.type == SDL_MOUSEBUTTONUP)
            {
                if (prog->button_lit != -1)
                {
					selected_item(0, T_SELECT, prog->button_lit);
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
