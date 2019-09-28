
#include "builder.h"

void					render_item_icons(t_prog *prog, t_media *media, SDL_Renderer *rend)
{
	int 				i;
	t_rec				box;

	if (!rend || !media || !prog)
		return ;
	i = 0;
	while (i < prog->modes[prog->m_id].n_btn && i < media->n_itemfull)
	{
		box = prog->modes[prog->m_id].btn[i].box;
		box.x += box.w * 0.1;
		box.w = box.h;
		draw_item(media->itemfull[i].type, box, rend, prog->t);
		i++;
	}
}

void					r_sel_item(t_sdl *sdl, t_grid *grid, t_media *media, t_prog *prog)
{
	if (!sdl || !media || !grid || prog->redraw == 0)
		return ;
	SDL_SetRenderDrawColor(sdl->rend, 55, 55, 55, 255);
	SDL_RenderClear(sdl->rend);
	render_btns(prog, sdl);
	render_item_icons(prog, media, sdl->rend);
	SDL_RenderPresent(sdl->rend);
	prog->redraw = 1;
}

unsigned short			u_sel_item(t_sdl *sdl, t_grid *grid, t_media *media, t_prog *prog)
{
	if (!sdl || !grid || !media || !prog->modes || !prog->modes[prog->m_id].btn)
		return (FAIL);
	if (prog->m_id != prog->last)
	{
		prog->click = (t_vec2d){ 0, 0 };
		prog->btn_lit = -1;
		prog->last = prog->m_id;
		return (SUCCESS);
	}
	if (btn_light(sdl->mouse, prog->modes[prog->m_id].btn, prog->modes[prog->m_id].n_btn, prog) == SUCCESS) // when mouse is over a button
		return (SUCCESS);
	prog->btn_lit = -1;
	return (SUCCESS);
}

int						i_sel_item(t_sdl *sdl, t_grid *grid, t_media *media, t_prog *prog)
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
				if (prog->btn_lit != -1)
				{
					select_it(0, SEL_I_SELECT, prog->btn_lit);
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
