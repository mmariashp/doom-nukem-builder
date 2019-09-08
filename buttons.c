
#include "builder.h"

void					render_button(t_button *button, t_sdl *sdl)
{
	SDL_Rect			rect;
	SDL_Color			sdl_color;
	SDL_Surface			*surf;
	SDL_Texture			*txtr;
	unsigned char		r;
	unsigned char		g;
	unsigned char		b;

	if (!button || !sdl->rend)
		return ;
	if (button->vis_lit_on[1] == FALSE && button->vis_lit_on[2] == FALSE)
	{

		rect = (SDL_Rect){ button->box.x, button->box.y,
						   button->box.w, button->box.h };
		if (button->back)
			SDL_RenderCopy(sdl->rend, button->back, NULL, &rect);
		if (button->front)
			SDL_RenderCopy(sdl->rend, button->front, NULL, &rect);
	}
	else if ((button->vis_lit_on[1] == TRUE || button->vis_lit_on[2] == TRUE))
	{
		rect = (SDL_Rect){ button->box.x, button->box.y,
						   button->box.w, button->box.h };

		if (button->lit_back)
			SDL_RenderCopy(sdl->rend, button->lit_back, NULL, &rect);
		if (button->lit)
			SDL_RenderCopy(sdl->rend, button->lit, NULL, &rect);
	}
	if (button->text)
	{
		rect = (SDL_Rect){ button->box.x + button->box.w * 0.05, button->box.y + button->box.h * 0.05,
						   button->box.w * 0.9, button->box.h * 0.8 };
		if (button->vis_lit_on[1] == TRUE)
			get_rgb(&r, &g, &b, LIT_COLOR);
		else
			get_rgb(&r, &g, &b, button->text_color);
		sdl_color = (SDL_Color){ r, g, b, 0 };
		surf = TTF_RenderText_Solid(sdl->font, button->text, sdl_color);
		txtr = SDL_CreateTextureFromSurface(sdl->rend, surf);
		SDL_FreeSurface(surf);
		SDL_RenderCopy(sdl->rend, txtr, NULL, &rect);
		SDL_DestroyTexture(txtr);
	}
}

void					render_buttons(t_button *buttons, t_sdl *sdl, int n_buttons)
{
	short 				i;

	i = 0;

	while (i < n_buttons)
	{
		if (buttons[i].vis_lit_on[0] == TRUE)
			render_button(&buttons[i], sdl);
		i++;
	}
}

unsigned short			light_button(t_sdl *sdl, t_button *buttons, int n_buttons, t_prog *prog)
{
	unsigned short		res;
	unsigned short		i;

	if (!sdl)
		return (FAIL);
	i = 0;
	res = FAIL;
	while (i < n_buttons)
	{
		if (mouse_over(buttons[i].box, sdl->mouse) == TRUE)
		{
			buttons[i].vis_lit_on[1] = TRUE;
			prog->button_lit = i;
			prog->features[F_REDRAW] = 1;
			res = SUCCESS;
		}
		else
		{
			if (buttons[i].vis_lit_on[1] == TRUE)
				prog->features[F_REDRAW] = 1;
			buttons[i].vis_lit_on[1] = FALSE;
		}
		i++;
	}
	if (res == FAIL)
		prog->button_lit = -1;
	return (res);
}