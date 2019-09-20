
#include "builder.h"

void					render_button(t_button *button, SDL_Renderer *rend)
{
	SDL_Rect			rect;

	if (!button || !rend)
		return ;
	if (button->vis_lit_on[1] == FALSE && button->vis_lit_on[2] == FALSE)
	{
		rect = (SDL_Rect){ button->box.x, button->box.y,
						   button->box.w, button->box.h };
		if (button->txtr)
			SDL_RenderCopy(rend, button->txtr, NULL, &rect);
	}
	else if ((button->vis_lit_on[1] == TRUE || button->vis_lit_on[2] == TRUE))
	{
		rect = (SDL_Rect){ button->box.x, button->box.y,
						   button->box.w, button->box.h };
		if (button->lit)
			SDL_RenderCopy(rend, button->lit, NULL, &rect);
	}
	if (button->text)
	{
        if (button->vis_lit_on[1] == TRUE)
            write_text(button->text, rend, button->box, LIT_COLOR, TRUE);
        else
            write_text(button->text, rend, button->box, button->text_color, TRUE);
	}
}

void					render_button_big(t_button *button, SDL_Renderer *rend)
{
	SDL_Rect			rect;

	if (!button || !rend)
		return ;
	rect = (SDL_Rect){ button->box.x , button->box.y - button->box.h * 0.3,
					   button->box.w * 1.3, button->box.h * 1.3 };
	if (button->txtr)
		SDL_RenderCopy(rend, button->txtr, NULL, &rect);
}

void					render_buttons(t_button *buttons, SDL_Renderer *rend, int n_buttons, int mode_id)
{
	short 				i;
	short 				state;

	i = 0;
	state = selected_item(1, STATE_SELECT, -1);

	while (i < n_buttons)
	{
		if (buttons[i].vis_lit_on[0] == TRUE)
		{
			if (mode_id == MODE_EDITOR && (buttons[i].vis_lit_on[1] == TRUE || buttons[i].vis_lit_on[2] == TRUE) && state == SECTOR_EDIT && i > B_ITEM_DEL)
				render_button_big(&buttons[i], rend);
			else
				render_button(&buttons[i], rend);
		}
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