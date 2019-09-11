
#include "builder.h"

void					render_textures(t_sdl *sdl, t_grid *grid, t_media *media, t_prog *prog)
{
    if (!sdl || !media || !grid || prog->features[F_REDRAW] == 0)
        return ;
    SDL_SetRenderDrawColor(sdl->rend, 55, 55, 55, 255);
    SDL_RenderClear(sdl->rend);

//    render_grid(media->worlds[media->world_id], grid, prog, sdl->mouse);
//    render_screen(sdl->rend, prog->screen);
    render_buttons(prog->modes[prog->mode_id].buttons, sdl, prog->modes[prog->mode_id].n_buttons);
    SDL_RenderPresent(sdl->rend);
    prog->features[F_REDRAW] = 1;
}

void					update_textures(t_sdl *sdl, t_grid *grid, t_media *media, t_prog *prog)
{
    if (!sdl || !grid || !media || !prog->modes || !prog->modes[prog->mode_id].buttons)
        return;
    if (light_button(sdl, prog->modes[prog->mode_id].buttons, prog->modes[prog->mode_id].n_buttons, prog) == SUCCESS) // when mouse is over a button
    {
        return ;
    }
    prog->button_lit = -1;
}

int						input_textures(t_sdl *sdl, float *grid_scale, t_media *media, t_prog *prog)
{
    int					quit;
    SDL_Event			event;

    quit = FALSE;
    if (!sdl || !media || !grid_scale)
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
