
#include "builder.h"



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
	char 				*tmp;

	if (init)
	{
		if (input)
			free(input);
		return((input = ft_strdup(init)));
	}
	if (ft_isalnum(change))
	{
		if ((tmp = ft_strdup(input)))
		{
			if (input)
				free(input);
			input = ft_strjoin(tmp, &change);
			free(tmp);
		}
	}
	else if (input && change == 8)
		input = del_char(input);
	if (!input || !ft_strlen (input))
		return (NULL);
	return (input);
}

void					r_levels(t_sdl *sdl, t_grid *grid, t_media *media, t_prog *prog)
{
	char 				*input;

	if (!sdl || !sdl->rend || !grid || !media || !prog)
		return ;
	SDL_SetRenderDrawColor(sdl->rend, 55, 55, 55, 255);
	SDL_RenderClear(sdl->rend);
	render_btn(prog->modes[prog->mode_id].btn, sdl, prog->modes[prog->mode_id].n_btn, prog->mode_id, prog->t);
	if (select_it(1, ST_SELECT, -1) == INP)
	{
		input = get_input(NULL, 0);
		if (input)
			write_text(input, sdl, (t_rec){ WIN_W / 4, WIN_H / 12, WIN_W / 2, WIN_H / 10 }, WHITE, TRUE);
	}
	SDL_RenderPresent(sdl->rend);
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

void					turn_btns_off(t_prog *prog)
{
	int 				i;

	if (prog->modes && prog->modes[prog->mode_id].btn && prog->modes[prog->mode_id].n_btn > 0)
	{
		i = 0;
		while (i < prog->modes[prog->mode_id].n_btn)
		{
			prog->modes[prog->mode_id].btn[i].vis_lit_on[2] = FALSE;
			i++;
		}
	}
	prog->btn_on = -1;
}

unsigned short			u_levels(t_sdl *sdl, t_grid *grid, t_media *media, t_prog *prog)
{
	int 				world;
	int 				edit_del;
	char 				*new;
	char 				*tmp;

	if (!sdl || !grid || !media || !prog->modes || !prog->modes[prog->mode_id].btn)
		return (FAIL);
	world = select_it(1, WORLD_SELECT, -1);
	if (select_it(1, ST_SELECT, -1) != NORMAL)
		return (SUCCESS);
	if (within(prog->btn_on, media->n_worlds, prog->modes[prog->mode_id].n_btn))
	{
		turn_btns_off(prog);
	}
	light_button(sdl, prog->modes[prog->mode_id].btn,  prog->modes[prog->mode_id].n_btn, prog);
	if ((prog->click.x || prog->click.y) && prog->btn_lit != -1)
	{
		turn_btns_off(prog);
		prog->btn_on = prog->btn_lit;
		prog->modes[prog->mode_id].btn[prog->btn_on].vis_lit_on[2] = TRUE;
		prog->click = (t_vec2d){ 0, 0 };
		if (within(prog->btn_on, media->n_worlds, prog->modes[prog->mode_id].n_btn))
		{
			if (within((world = get_w_id(prog->btn_on, media->n_worlds, &edit_del)), -1, media->n_worlds))
			{
				select_it(0, WORLD_SELECT, world);
				if (edit_del == 0 && (tmp = get_input(media->worlds[world].filename, 0)))
					select_it(0, ST_SELECT, INP);
				else if (edit_del == 1)
				{
					delete_world(media, world);
					select_it(0, WORLD_SELECT, -1);
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
			tmp = get_full_path(new, media->extensions[0], media->paths[0]);
			if (tmp)
			{
				if (my_rename(media->worlds[world].full_path, tmp) == SUCCESS)
				{
					if (media->worlds[world].filename)
						free(media->worlds[world].filename);
					if (media->worlds[world].full_path)
						free(media->worlds[world].full_path);
					media->worlds[world].filename = ft_strdup(new);
					media->worlds[world].full_path = get_full_path(new, media->extensions[0], media->paths[0]);
				}
				free(tmp);
			}
		}
		select_it(0, WORLD_SELECT, -1);
		refresh_level_list(media, &prog->modes[prog->mode_id]);
	}
	return (SUCCESS);
}

void					switch_mode(t_prog *prog, int new_mode_id, int new_state)
{
	if (prog)
	{
		prog->modes[prog->mode_id].btn[prog->btn_on].vis_lit_on[2] = FALSE;
		prog->last_mode_id = prog->mode_id;
		prog->mode_id = new_mode_id;
		prog->btn_lit = -1;
		select_it(0, ST_SELECT, new_state);
		prog->click = (t_vec2d){ 0, 0 };
	}
}

int						i_levels(t_sdl *sdl, t_grid *grid, t_media *med, t_prog *prog)
{
	int					quit;
	SDL_Event			e;
//	char 				*tmp;
	
	quit = sdl && grid && med && prog ? FALSE : TRUE;
	while(SDL_PollEvent(&e) && quit == FALSE)
	{
		SDL_GetMouseState(&sdl->mouse.x, &sdl->mouse.y);
		if (e.type == SDL_QUIT || (e.type == SDL_KEYUP &&
		e.key.keysym.sym == SDLK_ESCAPE))
			quit = TRUE;
		else if (e.type == SDL_KEYUP)
		{
			if (ft_isalnum(e.key.keysym.sym) || e.key.keysym.sym == 8)
				get_input(NULL, e.key.keysym.sym);
//			if (ft_isalnum(e.key.keysym.sym) || e.key.keysym.sym == 8)
//				tmp = get_input(NULL, e.key.keysym.sym);
			if (e.key.keysym.sym == SDLK_RETURN && select_it(1, ST_SELECT, INP))
				select_it(0, ST_SELECT, NORMAL);
		}
		else if(e.type == SDL_MOUSEBUTTONDOWN)
			prog->click = sdl->mouse;
		else if (e.type == SDL_MOUSEBUTTONUP &&
		within((med->w_id = prog->btn_on), -1, med->n_worlds + 1))
			switch_mode(prog, MODE_EDITOR, NORMAL);
	}
	return (quit);
}
