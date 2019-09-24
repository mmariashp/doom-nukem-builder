
#include "builder.h"


int						my_rename(const char *oldname, const char *newname)
{
	char 				*tmp;
	char 				*tmp2;

	if (!oldname || !newname)
		return (FAIL);
	tmp = ft_strjoin("mv ", oldname);
	if (tmp)
	{
		tmp2 = ft_strjoin(tmp, " ");
		free(tmp);
		if (tmp2)
		{
			tmp = ft_strjoin(tmp2, newname);
			free(tmp2);
			if (tmp)
			{
				system(tmp);
				free(tmp);
				return (SUCCESS);
			}
		}
	}
	return (FAIL);
}

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

char 					*del_char(char *input)
{
	size_t 				i;
	char 				*new;

	i = 0;
	if (!input)
		return (NULL);
	while (input[i])
		i++;
	if (!i || !(new = ft_strnew(sizeof(char) * (i - 1))))
		return (NULL);
	new = ft_strncpy(new, input, sizeof(char) * (i - 1));
	free(input);
	return (new);
}

char 					*get_input(char *init, char change)
{
	static char 		*input = NULL;

	if (init)
	{
		if (input)
		{
			free(input);
			input = NULL;
		}
		input = ft_strdup(init);
		if (!input)
			return (NULL);
		return (input);
	}
	else
	{
		if (ft_isalnum(change) || change == '_')
			input = ft_strjoin(input, &change);
		else if (input && change == 8)
			input = del_char(input);
		if (!input || !ft_strlen (input))
			return (NULL);
		return (input);
	}
}

void					render_summary(t_sdl *sdl, t_grid *grid, t_media *media, t_prog *prog)
{
	char 				*input;

	if (!sdl || !sdl->rend || !grid || !media || !prog)
		return ;
	SDL_SetRenderDrawColor(sdl->rend, 55, 55, 55, 255);
	SDL_RenderClear(sdl->rend);
	render_buttons(prog->modes[prog->mode_id].buttons, sdl->rend, prog->modes[prog->mode_id].n_buttons, prog->mode_id, prog->t);
	if (selected_item(1, STATE_SELECT, -1) == INPUT)
	{
		input = get_input(NULL, 0);
		if (input)
		{
			write_text(get_input(NULL, 0), sdl->rend, (t_rec){ WIN_W / 4, WIN_H / 12, WIN_W / 2, WIN_H / 10 }, WHITE, TRUE);
		}
	}
	SDL_RenderPresent(sdl->rend);
}

int 					get_b_id(int world_id, int n_worlds, int edit_del)
{
	if (edit_del == 0)
		return (n_worlds + 1 + world_id * 2);
	else
		return (n_worlds + world_id * 2);
}

int 					get_w_id(int btn_id, int n_worlds, int *edit_del)
{
	if (n_worlds % 2 == 0)
	{
		if (btn_id % 2 == 0)
		{
			*edit_del = 1;
			return ((btn_id - n_worlds - 2) / 2);
		}
		*edit_del = 0;
		return ((btn_id - n_worlds - 1) / 2);
	}
	else
	{
		if (btn_id % 2 == 0)
		{
			*edit_del = 0;
			return ((btn_id - n_worlds - 1) / 2);
		}
		*edit_del = 1;
		return ((btn_id - n_worlds - 2) / 2);
	}
}

unsigned short			update_summary(t_sdl *sdl, t_grid *grid, t_media *media, t_prog *prog)
{
	int 				world;
	int 				edit_del;
	char 				*new;

	if (!sdl || !grid || !media || !prog->modes || !prog->modes[prog->mode_id].buttons)
		return (FAIL);
	world = selected_item(1, WORLD_SELECT, -1);
	if (selected_item(1, STATE_SELECT, -1) == NORMAL)
	{
		if (within(prog->button_on, media->n_worlds, prog->modes[prog->mode_id].n_buttons))
		{
			prog->modes[prog->mode_id].buttons[prog->button_on].vis_lit_on[2] = FALSE;
			prog->button_on = -1;
		}
		if (light_button(sdl, prog->modes[prog->mode_id].buttons,  prog->modes[prog->mode_id].n_buttons, prog) == SUCCESS) // when mouse is over a button
			prog->features[F_REDRAW] = 1;
		if ((prog->click.x || prog->click.y) && prog->button_lit != -1)
		{
			if (within(prog->button_on, -1, prog->modes[prog->mode_id].n_buttons) == TRUE)
				prog->modes[prog->mode_id].buttons[prog->button_on].vis_lit_on[2] = FALSE;
			prog->button_on = prog->button_lit;
			prog->modes[prog->mode_id].buttons[prog->button_on].vis_lit_on[2] = TRUE;
			prog->click = (t_vec2d){ 0, 0 };
			if (within(prog->button_on, media->n_worlds, prog->modes[prog->mode_id].n_buttons))
			{
				world = get_w_id(prog->button_on, media->n_worlds, &edit_del);
				if (within(world, -1, media->n_worlds))
				{
					selected_item(0, WORLD_SELECT, world);
					if (edit_del == 0)
					{
						selected_item(0, STATE_SELECT, INPUT);
						get_input(media->worlds[world].filename, 0);
					}
					else
					{
						delete_world(media, world);
						selected_item(0, WORLD_SELECT, -1);
						refresh_level_list(media, &prog->modes[prog->mode_id]);
					}
				}
			}
		}
		else if (within(world, -1, media->n_worlds))
		{
			new = get_input(NULL, 0);
			if (new)
			{
				if (my_rename(media->worlds[world].full_path, \
				get_full_path(new, media->extensions[0], media->paths[0])) == SUCCESS)
				{
					if (media->worlds[world].filename)
						free(media->worlds[world].filename);
					if (media->worlds[world].full_path)
						free(media->worlds[world].full_path);
					media->worlds[world].filename = ft_strdup(new);
					media->worlds[world].full_path = get_full_path(new, media->extensions[0], media->paths[0]);
				}
			}
			selected_item(0, WORLD_SELECT, -1);
			refresh_level_list(media, &prog->modes[prog->mode_id]);
		}
	}
	prog->features[F_REDRAW] = 1;
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
			else if (ft_isalnum(event.key.keysym.sym) ||
			event.key.keysym.sym == '_' || event.key.keysym.sym == 8)
			{
				get_input(NULL, event.key.keysym.sym);
				break ;
			}
			else if (event.key.keysym.sym == SDLK_RETURN && selected_item(1, STATE_SELECT, INPUT))
			{
				selected_item(0, STATE_SELECT, NORMAL);
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
				if (within(prog->button_on, -1, media->n_worlds + 1))
				{
					prog->modes[prog->mode_id].buttons[prog->button_on].vis_lit_on[2] = FALSE;
					prog->last_mode_id = prog->mode_id;
                    prog->mode_id = MODE_EDITOR;
                    media->w_id = prog->button_on;
					prog->button_lit = -1;
                    prog->button_on = DRAG_BTN;
                    selected_item(0, STATE_SELECT, NORMAL);
//                    prog->modes[prog->mode_id].buttons[DRAG_BTN].vis_lit_on[2] = TRUE;
                    return (quit);
				}
				prog->click = (t_vec2d){ 0, 0 };
			}
		}
	}
	return (quit);
}