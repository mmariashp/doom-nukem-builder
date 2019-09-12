
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
			write_text(txtrs[i].name, sdl, text, 0, TRUE);
		i++;
	}


}

void					render_textures(t_sdl *sdl, t_grid *grid, t_media *media, t_prog *prog)
{
    if (!sdl || !media || !grid || prog->features[F_REDRAW] == 0)
        return ;
    SDL_SetRenderDrawColor(sdl->rend, 55, 55, 55, 255);
    SDL_RenderClear(sdl->rend);
    render_buttons(prog->modes[prog->mode_id].buttons, sdl, prog->modes[prog->mode_id].n_buttons);
    render_texture_icons(prog->modes[prog->mode_id].buttons, sdl, prog->modes[prog->mode_id].n_buttons, media->txtrs);
    SDL_RenderPresent(sdl->rend);
    prog->features[F_REDRAW] = 1;
}

void					update_textures(t_sdl *sdl, t_grid *grid, t_media *media, t_prog *prog)
{
    if (!sdl || !grid || !media || !prog->modes || !prog->modes[prog->mode_id].buttons)
        return;
    if (light_button(sdl, prog->modes[prog->mode_id].buttons, prog->modes[prog->mode_id].n_buttons, prog) == SUCCESS) // when mouse is over a button
    {
		printf("in textures mode lit button %d\n", prog->button_lit);
        return ;
    }
    prog->button_lit = -1;
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
//					printf("in textures mode pressed button %d\n", prog->button_lit);
//                	printf("in textures mode pressed button %d\n", prog->button_on);
                	grid->active[1].x = prog->button_lit;
					prog->last_mode_id = prog->mode_id;
                    prog->mode_id = MODE_EDITOR;
                    prog->button_on = SECTOR_BUTTON;
                    prog->modes[prog->mode_id].buttons[SECTOR_BUTTON].vis_lit_on[2] = TRUE;
                    prog->move.x = 0;
                    prog->move.y = 0;
                    return (quit);
                }
            }
        }
    }
    return (quit);
}
