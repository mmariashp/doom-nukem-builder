
#include "builder.h"

void					render_editor(t_sdl *sdl, t_t *t, t_media *media, t_prog *prog)
{
	if (!sdl || !media || !t || prog->features[F_REDRAW] == 0)
		return ;
	SDL_SetRenderDrawColor(sdl->rend, 55, 55, 55, 255);

	SDL_RenderClear(sdl->rend);

	render_grid(media->worlds[media->world_id], t, prog, sdl->mouse);
	render_screen(sdl->rend, prog->screen);
	if (prog->button_on == SECTOR_BUTTON && t->active[0].y >= 0 && t->active[0].y < media->worlds[media->world_id].n_sectors)
		render_sector_menu(sdl, t, &media->worlds[media->world_id].sectors[t->active[0].y]);
	render_buttons(prog->modes[prog->mode_id].buttons, sdl, prog->modes[prog->mode_id].n_buttons);
	SDL_RenderPresent(sdl->rend);
	prog->features[F_REDRAW] = 0;
}

void					update_editor(t_sdl *sdl, t_t *t, t_media *media, t_prog *prog)
{
	static short 		world_id = -1;
	static				int s = -1;

	if (!sdl || !media || !t)
		return;
	if (prog->save == 1 || prog->button_on == SAVE_BUTTON) // when saving
	{
		rewrite_media(media);
		prog->save = 0;
		prog->button_on = DRAG_BUTTON;
		prog->modes[prog->mode_id].buttons[prog->button_on].vis_lit_on[2] = TRUE;
		prog->modes[prog->mode_id].buttons[SAVE_BUTTON].vis_lit_on[2] = FALSE;
		return ;
	}
	if (prog->button_lit != -1 && (prog->move.x || prog->move.y)) // when pressing an on screen button
	{
		if (prog->button_on != -1)
			prog->modes[prog->mode_id].buttons[prog->button_on].vis_lit_on[2] = FALSE;
		prog->button_on = prog->button_lit;
		prog->modes[prog->mode_id].buttons[prog->button_on].vis_lit_on[2] = TRUE;
		t->active[0] = (t_vec2d){ -1, -1 };
		t->active[1] = (t_vec2d){ -1, -1 };
		return ;
	}
	if (light_button(sdl, prog->modes[prog->mode_id].buttons,  prog->modes[prog->mode_id].n_buttons, prog) == SUCCESS) // when mouse is over a button
		return ;
	if (world_id != media->world_id) // when opening a map
	{
		world_id = media->world_id;
		clean_grid(&t->grid);
		if (media->world_id >= media->n_worlds && add_world(&media->worlds, media->n_worlds) == SUCCESS) // if ADD WORLD chosen
			media->n_worlds++;
		fill_grid(media->worlds[world_id].n_vectors, media->worlds[world_id].vertices, &t->grid);
		prog->features[F_REDRAW] = 1;
		update_sector_status(media->worlds[world_id].sectors, media->worlds[world_id].walls, media->worlds[world_id].vertices, media->worlds[world_id].n_sectors);
		return ;
	}
	if (prog->button_on == DRAW_BUTTON) // draw mode
	{
		if ((prog->move.x || prog->move.y) && mouse_over(t->grid.box, sdl->mouse))
		{
			if (t->active[0].x == -1)
				t->active[0] = find_node(sdl->mouse.x, sdl->mouse.y, t);
			else if (t->active[1].x == -1)
				t->active[1] = find_node(sdl->mouse.x, sdl->mouse.y, t);
			if (t->active[0].x != -1)
				add_to_media(t, media);
			prog->features[F_REDRAW] = 1;
			prog->move.x = 0;
			prog->move.y = 0;
		}
		else if (t->active[0].x != -1 && t->active[1].x == -1)
			prog->features[F_REDRAW] = 1;
	}
	else if (prog->button_on == DISTORT_BUTTON ) // move mode
		move_vector(prog, sdl->mouse, t, &media->worlds[world_id]);
	else if (prog->button_on == DRAG_BUTTON) // view mode
	{
		if (prog->move.x || prog->move.y)
			move_grid(prog, sdl->mouse, &t->grid);
	}
	else if (prog->button_on == DELETE_BUTTON) // delete mode
	{
		if (prog->move.x || prog->move.y)
		{
			if (mouse_over(t->grid.box, sdl->mouse))
			{
				int m = -1;
				t->active[0] = find_node(sdl->mouse.x, sdl->mouse.y, t);
				if (t->grid.nodes[t->active[0].x][t->active[0].y] == NODE_FULL)
					m = find_vector(media->worlds[world_id].vertices, t->active[0], media->worlds[world_id].n_vectors);
				delete_vector(m, &media->worlds[world_id]);
				t->grid.nodes[t->active[0].x][t->active[0].y] = NODE_EMPTY;
				prog->move = (t_vec2d){ 0, 0 };
				prog->features[F_REDRAW] = 1;
			}
			else
				prog->move = (t_vec2d){ 0, 0 };
		}
	}
	else if (prog->button_on == SECTOR_BUTTON && s == -1) // sector mode
	{
		int k;
		t->active[0] = (t_vec2d){ -1, -1 };
		k = in_sector(sdl->mouse, &media->worlds[world_id], t);
		if (k != -1)
		{
			t->active[0].x = k;
			if (prog->move.x || prog->move.y)
			{
				s = k;
				t->active[0].y = s;
			}
		}
		prog->features[F_REDRAW] = 1;

	}
	else if (prog->button_on == DESELECT_SEC_BUTTON && s != -1) // sector mode
	{
		s = -1;
		prog->features[F_REDRAW] = 1;
		prog->modes[prog->mode_id].buttons[prog->button_on].vis_lit_on[2] = FALSE;
		prog->button_on = SECTOR_BUTTON;
		prog->modes[prog->mode_id].buttons[prog->button_on].vis_lit_on[2] = TRUE;

	}
	if (prog->button_on == SECTOR_BUTTON)
	{
		prog->modes[prog->mode_id].buttons[7].vis_lit_on[0] = TRUE;
		prog->modes[prog->mode_id].buttons[8].vis_lit_on[0] = TRUE;
		prog->modes[prog->mode_id].buttons[9].vis_lit_on[0] = TRUE;
		prog->modes[prog->mode_id].buttons[10].vis_lit_on[0] = TRUE;
		prog->modes[prog->mode_id].buttons[11].vis_lit_on[0] = TRUE;
	}
	else
	{
		prog->modes[prog->mode_id].buttons[7].vis_lit_on[0]  = FALSE;
		prog->modes[prog->mode_id].buttons[8].vis_lit_on[0]  = FALSE;
		prog->modes[prog->mode_id].buttons[9].vis_lit_on[0]  = FALSE;
		prog->modes[prog->mode_id].buttons[10].vis_lit_on[0] = FALSE;
		prog->modes[prog->mode_id].buttons[11].vis_lit_on[0] = FALSE;
	}

	if (prog->zoom != 0)
		zoom_grid(prog, sdl->mouse, &t->grid);
	update_sector_status(media->worlds[world_id].sectors, media->worlds[world_id].walls, media->worlds[world_id].vertices, media->worlds[world_id].n_sectors);
}

int						input_editor(t_sdl *sdl, float *grid_scale, t_media *media, t_prog *prog)
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
			if (event.key.keysym.sym == 'w')
			{
				printf("%c\n", event.key.keysym.sym);
			}
			else if (event.key.keysym.sym == 's')
			{
				printf("%c\n", event.key.keysym.sym);
				prog->save = 1;
				break ;
			}
			else if (event.key.keysym.sym == 'a')
			{
				printf("%c\n", event.key.keysym.sym);
			}
			else if (event.key.keysym.sym == 'd')
			{
				printf("%c\n", event.key.keysym.sym);
			}
			else if (event.key.keysym.sym == 'q')
			{
				printf("%c\n", event.key.keysym.sym);
			}
			else if (event.key.keysym.sym == 'e')
			{
				SDL_ShowSimpleMessageBox(0, "Mouse", "Left button was pressed!", sdl->window);
				printf("%c\n", event.key.keysym.sym);
			}
			else if (event.key.keysym.sym == SDLK_RCTRL)
			{
				printf("%c\n", event.key.keysym.sym);
			}
			else if (event.key.keysym.sym == ' ')
			{
				printf("%c\n", event.key.keysym.sym);
			}
			else if (event.key.keysym.sym == SDLK_ESCAPE)
			{
				quit = TRUE;
				break ;
			}
		}
		if(event.type == SDL_MOUSEWHEEL)
		{
			if(event.wheel.y > 0) // scroll up
				prog->zoom++;
			else if(event.wheel.y < 0) // scroll down
				prog->zoom--;
		}
		if( event.type == SDL_MOUSEMOTION || event.type == SDL_MOUSEBUTTONDOWN || event.type == SDL_MOUSEBUTTONUP )
		{
			if(event.type == SDL_MOUSEBUTTONDOWN)
			{
				if (prog->button_lit == BACK_BUTTON)
				{
					prog->mode_id = MODE_SUMMARY;
					prog->button_lit = -1;
					prog->button_on = -1;
					media->world_id = -1;
					return (quit);
				}
				prog->move.x = sdl->mouse.x;
				prog->move.y = sdl->mouse.y;
			}
			if(event.type == SDL_MOUSEBUTTONUP)
			{
				prog->move.x = 0;
				prog->move.y = 0;
			}
		}
	}
	return (quit);
}