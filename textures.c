
#include "builder.h"

void					render_texture_icons(t_btn *btn, t_sdl *sdl, int n_btn, t_texture *txtrs)
{
	if (!btn || n_btn < 1 || !sdl || !txtrs)
		return ;
	SDL_Rect rect;
	t_rec				text;
	int i = 0;

	while (i < n_btn)
	{
		text = btn[i].box;
		text.h /= 6;

		rect = (SDL_Rect){ btn[i].box.x + btn[i].box.w * 0.1, btn[i].box.y + btn[i].box.h * 0.15,
						   btn[i].box.w * 0.8, btn[i].box.h * 0.8 };
		if (txtrs[i].sdl_t)
			SDL_RenderCopy(sdl->rend, txtrs[i].sdl_t, NULL, &rect);
		if (txtrs[i].name)
			write_text(txtrs[i].name, sdl, text, 0, TRUE);
		i++;
	}
}

void					r_textures(t_sdl *sdl, t_grid *grid, t_media *media, t_prog *prog)
{
    if (!sdl || !media || !grid || prog->features[F_REDRAW] == 0)
        return ;
    SDL_SetRenderDrawColor(sdl->rend, 55, 55, 55, 255);
    SDL_RenderClear(sdl->rend);
    render_btns(prog, sdl);
    render_texture_icons(prog->modes[prog->m_id].btn, sdl, prog->modes[prog->m_id].n_btn, media->txtrs);
    SDL_RenderPresent(sdl->rend);
    prog->features[F_REDRAW] = 1;
}

unsigned short			u_textures(t_sdl *sdl, t_grid *grid, t_media *media, t_prog *prog)
{
    if (!sdl || !grid || !media || !prog->modes || !prog->modes[prog->m_id].btn)
        return (FAIL);
    if (btn_light(sdl->mouse, prog->modes[prog->m_id].btn, prog->modes[prog->m_id].n_btn, prog) == SUCCESS) // when mouse is over a button
        return (SUCCESS);
    prog->btn_lit = -1;
	return (SUCCESS);
}

int						i_textures(t_sdl *sdl, t_grid *grid, t_media *media, t_prog *prog)
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
                if (prog->btn_lit != -1)
                {
					select_it(0, T_SELECT, prog->btn_lit);
                    int i = 0;
                    while (i < prog->modes[prog->m_id].n_btn)
                    {
                        prog->modes[prog->m_id].btn[i].vis_lit_on[1] = FALSE;
                        prog->modes[prog->m_id].btn[i].vis_lit_on[2] = FALSE;
                        i++;
                    }
					prog->last = prog->m_id;
                    prog->m_id = MODE_EDITOR;

                    prog->click.x = 0;
                    prog->click.y = 0;
                    return (quit);
                }
            }
        }
    }
    return (quit);
}
