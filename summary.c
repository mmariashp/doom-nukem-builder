
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
//	size = area / media->n_textures;
//	size = sqrt(size);
//	x = rec.x;
//	y = rec.y;
//	while (i < media->n_textures)
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

void					render_summary(t_sdl *sdl, t_t *t, t_media *media)
{
	if (!sdl || !t || !media)
		return ;
	SDL_RenderClear(sdl->rend);
	render_buttons(sdl->modes[sdl->mode_id].buttons, sdl);
	SDL_RenderPresent(sdl->rend);
}

void					update_summary(t_sdl *sdl, t_t *t, t_media *media)
{
	if (!sdl || !t || !media || !sdl->modes || !sdl->modes[sdl->mode_id].buttons)
		return;
	if (light_button(sdl) == SUCCESS)
		return ;
	sdl->button_lit = -1;
}

int						input_summary(t_sdl *sdl, float *grid_scale, t_media *media)
{
	int					quit;
	SDL_Event			event;
	int 				mode;
	int 				level;

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
			if (event.key.keysym.sym == SDLK_ESCAPE)
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
				if (sdl->button_lit != -1)
				{
					mode = 1;
					level = sdl->button_lit;
				}
			}
		}
	}
	if (mode == 1)
	{
		sdl->mode_id = LEVEL_EDIT;
		media->world_id = level;
	}
	return (quit);
}