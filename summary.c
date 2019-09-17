
#include "builder.h"

void					render_frame(t_rec rec, int color, SDL_Renderer *rend)
{
	if (!rend)
		return ;
	draw_line((t_line){ (t_vec2d){ rec.x, rec.y }, (t_vec2d){ rec.x + rec.w, rec.y } }, color, rend);
	draw_line((t_line){ (t_vec2d){ rec.x, rec.y }, (t_vec2d){ rec.x, rec.y + rec.h } }, color, rend);
	draw_line((t_line){ (t_vec2d){ rec.x + rec.w, rec.y }, (t_vec2d){ rec.x + rec.w, rec.y + rec.h } }, color, rend);
	draw_line((t_line){ (t_vec2d){ rec.x + rec.w, rec.y + rec.h }, (t_vec2d){ rec.x, rec.y + rec.h } }, color, rend);
}

void					render_txtr_box(t_vec2d size, t_vec2d pos, SDL_Texture *t, SDL_Renderer *rend)
{
	SDL_Rect			rect = { pos.x, pos.y, size.x, size.y };
	if (!t || !rend)
		return ;
	SDL_RenderCopy(rend, t, 0, &rect);
}

//void					render_textures(t_rec rec, t_media *media, SDL_Renderer *rend)
//{
//	int					i = 0;
//	int 				x;
//	int 				y;
//	int 				size;
//
//
//	int area = (rec.w) * (rec.h);
//	size = area / media->n_txtrs;
//	size = sqrt(size);
//	x = rec.x;
//	y = rec.y;
//	while (i < media->n_txtrs)
//	{
//		if (rec.w - x < size)
//		{
//			x = rec.x;
//			y += size;
//		}
//		render_txtr_box((t_vec2d){ size, size }, (t_vec2d){ x, y }, media->txtrs[i].sdl_t, rend);
//		x += size;
//		i++;
//	}
//
//}

void					render_summary(t_sdl *sdl, t_grid *grid, t_media *media, t_prog *prog)
{
	if (!sdl || !grid || !media)
		return ;
	SDL_SetRenderDrawColor(sdl->rend, 55, 55, 55, 255);
	SDL_RenderClear(sdl->rend);
//	static SDL_Texture *f = NULL;
//	if (f == NULL)
//		f = load_texture("grey_panel.png", sdl);
//	SDL_RenderCopy(sdl->rend, f, 0, 0);
	render_buttons(prog->modes[prog->mode_id].buttons, sdl->rend, prog->modes[prog->mode_id].n_buttons);
	SDL_RenderPresent(sdl->rend);
}

unsigned short			update_summary(t_sdl *sdl, t_grid *grid, t_media *media, t_prog *prog)
{
	if (!sdl || !grid || !media || !prog->modes || !prog->modes[prog->mode_id].buttons)
		return (FAIL);
	if (light_button(sdl, prog->modes[prog->mode_id].buttons,  prog->modes[prog->mode_id].n_buttons, prog) == SUCCESS) // when mouse is over a button
		prog->features[F_REDRAW] = 1;
	if ((prog->click.x || prog->click.y) && prog->button_lit != -1)
	{
		if (within(prog->button_on, -1, prog->modes[prog->mode_id].n_buttons) == TRUE)
			prog->modes[prog->mode_id].buttons[prog->button_on].vis_lit_on[2] = FALSE;
		prog->button_on = prog->button_lit;
		prog->modes[prog->mode_id].buttons[prog->button_on].vis_lit_on[2] = TRUE;
		prog->click.x = 0;
		prog->click.y = 0;
		prog->features[F_REDRAW] = 1;
	}
	return (SUCCESS);
}

int						input_summary(t_sdl *sdl, t_grid *grid, t_media *media, t_prog *prog)
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
			if(event.type == SDL_MOUSEBUTTONDOWN)
				prog->click = sdl->mouse;
			if (event.type == SDL_MOUSEBUTTONUP)
			{
				if (prog->button_on <= media->n_worlds)
				{
					prog->modes[prog->mode_id].buttons[prog->button_on].vis_lit_on[2] = FALSE;
					prog->last_mode_id = prog->mode_id;
                    prog->mode_id = MODE_EDITOR;
                    media->world_id = prog->button_on;
					prog->button_lit = -1;
                    prog->button_on = DRAG_BTN;
                    selected_item(0, STATE_SELECT, NORMAL);
                    prog->modes[prog->mode_id].buttons[DRAG_BTN].vis_lit_on[2] = TRUE;
                    return (quit);
				}
				prog->click = (t_vec2d){ 0, 0 };
			}
		}
	}
	return (quit);
}