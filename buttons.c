
#include "builder.h"

void					render_button(t_button *button, t_sdl *sdl, SDL_Texture **t)
{
	SDL_Rect			rect;

	if (!button || !sdl || !sdl->rend)
		return ;
	if (button->vis_lit_on[1] == FALSE && button->vis_lit_on[2] == FALSE)
	{
		rect = (SDL_Rect){ button->box.x, button->box.y,
						   button->box.w, button->box.h };
		if (t && within(button->reg_i, -1, TOTAL_TXTRS) && t[button->reg_i])
			SDL_RenderCopy(sdl->rend, t[button->reg_i], NULL, &rect);
	}
	else if ((button->vis_lit_on[1] == TRUE || button->vis_lit_on[2] == TRUE))
	{
		rect = (SDL_Rect){ button->box.x, button->box.y,
						   button->box.w, button->box.h };
		if (t && within(button->lit_i, -1, TOTAL_TXTRS) && t[button->lit_i])
			SDL_RenderCopy(sdl->rend, t[button->lit_i], NULL, &rect);
	}
	if (button->text)
	{
        if (button->vis_lit_on[1] == TRUE)
            write_text(button->text, sdl, button->box, LIT_COLOR, TRUE);
        else
            write_text(button->text, sdl, button->box, button->text_color, TRUE);
	}
}

void					render_button_big(t_button *button, t_sdl *sdl, SDL_Texture **t)
{
	SDL_Rect			rect;

	if (!button || !sdl || !sdl->rend || !t)
		return ;
	rect = (SDL_Rect){ button->box.x , button->box.y - button->box.h * 0.3,
					   button->box.w * 1.3, button->box.h * 1.3 };
	if (within(button->reg_i, -1, TOTAL_TXTRS) && t[button->reg_i])
		SDL_RenderCopy(sdl->rend, t[button->reg_i], NULL, &rect);
}

void					render_btn(t_button *btn, t_sdl *sdl, int n_btn, int m_id, SDL_Texture **t)
{
	short 				i;
	short 				state;

	i = 0;
	state = select_it(1, ST_SELECT, -1);
	if (!btn || !sdl)
		return ;
	while (i < n_btn)
	{
		if (btn[i].vis_lit_on[0] == TRUE)
		{
			if (m_id == MODE_EDITOR && (btn[i].vis_lit_on[1] == TRUE ||
			btn[i].vis_lit_on[2] == TRUE) && state == SEC_EDIT &&
			i > B_ITEM_DEL)
				render_button_big(&btn[i], sdl, t);
			else
				render_button(&btn[i], sdl, t);
		}
		i++;
	}
}

unsigned short			btn_light(t_vec2d mouse, t_button *btn, int n_btn, t_prog *prog)
{
	unsigned short		res;
	unsigned short		i;

	if (!btn || !prog)
		return (FAIL);
	i = -1;
	res = FAIL;
	prog->btn_lit = -1;
	while (++i < n_btn)
	{
		if (btn[i].vis_lit_on[0] && mouse_over(btn[i].box, mouse) == TRUE)
		{
			btn[i].vis_lit_on[1] = TRUE;
			prog->btn_lit = i;
			prog->features[F_REDRAW] = 1;
			res = SUCCESS;
		}
		else
		{
			if (btn[i].vis_lit_on[1] == TRUE)
				prog->features[F_REDRAW] = 1;
			btn[i].vis_lit_on[1] = FALSE;
		}
	}
	return (res);
}