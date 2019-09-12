
#include "builder.h"

void					render_editor(t_sdl *sdl, t_grid *grid, t_media *media, t_prog *prog)
{
	if (!sdl || !media || !grid || prog->features[F_REDRAW] == 0)
		return ;
	SDL_SetRenderDrawColor(sdl->rend, 55, 55, 55, 255);
	SDL_RenderClear(sdl->rend);

	render_grid(media->worlds[media->world_id], grid, prog, sdl->mouse);
	render_screen(sdl->rend, prog->screen);

	if (prog->button_on == SECTOR_BUTTON && grid->active[0].y >= 0 && grid->active[0].y < media->worlds[media->world_id].n_sectors)
	    render_sector_menu(sdl, grid, &media->worlds[media->world_id].sectors[grid->active[0].y], media->txtrs, media->n_textures);

	render_buttons(prog->modes[prog->mode_id].buttons, sdl, prog->modes[prog->mode_id].n_buttons);
	SDL_RenderPresent(sdl->rend);
	prog->features[F_REDRAW] = 0;

}

void					update_editor(t_sdl *sdl, t_grid *grid, t_media *media, t_prog *prog)
{
	static short 		world_id = -1;
	static				int s = -1;

	if (!sdl || !media || !grid)
		return;
	if (prog->last_mode_id == MODE_TEXTURES)
	{
		prog->last_mode_id = prog->mode_id;
		if (grid->active[1].x >= 0 && grid->active[1].x < media->n_textures)
		{
			if (grid->active[0].y >= 0 && grid->active[0].y < media->worlds[media->world_id].n_sectors)
			{
				media->worlds[media->world_id].sectors[grid->active[0].y].floor_txtr = grid->active[1].x;
			}
		}
	}
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
		if (prog->button_on < SECTOR_BUTTON || prog->button_on == DESELECT_SEC_BUTTON)
        {
            grid->active[0] = (t_vec2d){ -1, -1 };
            grid->active[1] = (t_vec2d){ -1, -1 };
        }
        prog->features[F_REDRAW] = 1;
	}
	if (light_button(sdl, prog->modes[prog->mode_id].buttons,  prog->modes[prog->mode_id].n_buttons, prog) == SUCCESS) // when mouse is over a button
    {
		prog->features[F_REDRAW] = 1;
    }
	if (world_id != media->world_id) // when opening a map
	{
		world_id = media->world_id;
		clean_grid(grid);
		if (media->world_id >= media->n_worlds && add_world(&media->worlds, media->n_worlds) == SUCCESS) // if ADD WORLD chosen
			media->n_worlds++;
		else
			zoom_to_map(media->worlds[media->world_id].n_vectors, media->worlds[media->world_id].vertices, grid);
        fill_grid(media->worlds[world_id].n_vectors, media->worlds[world_id].vertices, grid);
		prog->features[F_REDRAW] = 1;
		update_sector_status(media->worlds[world_id].sectors, media->worlds[world_id].walls, media->worlds[world_id].vertices, media->worlds[world_id].n_sectors);
		return ;
	}
	if (prog->button_on == DRAW_BUTTON) // draw mode
	{
		if ((prog->move.x || prog->move.y) && mouse_over(grid->box, sdl->mouse))
		{
			if (grid->active[0].x == -1)
				grid->active[0] = find_node(sdl->mouse.x, sdl->mouse.y, grid);
			else if (grid->active[1].x == -1)
				grid->active[1] = find_node(sdl->mouse.x, sdl->mouse.y, grid);
			if (grid->active[0].x != -1)
				add_to_media(grid, media);
			prog->features[F_REDRAW] = 1;
			prog->move.x = 0;
			prog->move.y = 0;
		}
		else if (grid->active[0].x != -1 && grid->active[1].x == -1)
			prog->features[F_REDRAW] = 1;
	}
	else if (prog->button_on == DISTORT_BUTTON ) // move mode
	{
		move_vector(prog, sdl->mouse, grid, &media->worlds[world_id]);
	}
	else if (prog->button_on == DRAG_BUTTON) // view mode
	{
		if (prog->move.x || prog->move.y)
			move_grid(prog, sdl->mouse, grid);
	}
	else if (prog->button_on == DELETE_BUTTON) // delete mode
	{

		if (prog->move.x || prog->move.y)
		{
			if (mouse_over(grid->box, sdl->mouse))
			{
				int m = -1;
				grid->active[0] = find_node(sdl->mouse.x, sdl->mouse.y, grid);
				if (grid->nodes[grid->active[0].x][grid->active[0].y] == NODE_FULL)
					m = find_vector(media->worlds[world_id].vertices, grid->active[0], media->worlds[world_id].n_vectors);
				delete_vector(m, &media->worlds[world_id]);
				grid->nodes[grid->active[0].x][grid->active[0].y] = NODE_EMPTY;
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
		if (s == -1)
		{
            grid->active[0] = (t_vec2d) {-1, -1};
        }
		if ((k = in_sector(sdl->mouse, &media->worlds[world_id], grid)) != -1)
		{
			grid->active[0].x = k;
			if (prog->move.x || prog->move.y)
			{
				s = k;
				grid->active[0].y = s;
                clean_grid(grid);
                fill_grid(media->worlds[world_id].n_vectors, media->worlds[world_id].vertices, grid);
                zoom_to_sector(&media->worlds[media->world_id].sectors[s], media->worlds[media->world_id].vertices, grid, prog);
                int i = DESELECT_SEC_BUTTON;
                while ( i < WT_EDIT_BUTTON + 1)
                    prog->modes[prog->mode_id].buttons[i++].vis_lit_on[0] = TRUE;
                i = 0;
                while ( i < DESELECT_SEC_BUTTON)
                    prog->modes[prog->mode_id].buttons[i++].vis_lit_on[0] = FALSE;
			}
		}
		prog->features[F_REDRAW] = 1;
	}
	else if (prog->button_on == DESELECT_SEC_BUTTON && s != -1) // sector mode
	{
        int i = DESELECT_SEC_BUTTON;
        while ( i < WT_EDIT_BUTTON + 1)
            prog->modes[prog->mode_id].buttons[i++].vis_lit_on[0] = FALSE;
        i = 0;
        while ( i < DESELECT_SEC_BUTTON)
            prog->modes[prog->mode_id].buttons[i++].vis_lit_on[0] = TRUE;
		s = -1;
		prog->features[F_REDRAW] = 1;
		prog->modes[prog->mode_id].buttons[prog->button_on].vis_lit_on[2] = FALSE;
		prog->button_on = SECTOR_BUTTON;
		prog->modes[prog->mode_id].buttons[prog->button_on].vis_lit_on[2] = TRUE;
		clean_grid(grid);
        zoom_to_map(media->worlds[media->world_id].n_vectors, media->worlds[media->world_id].vertices, grid);
        fill_grid(media->worlds[world_id].n_vectors, media->worlds[world_id].vertices, grid);
    }
    else if (prog->button_on == F_UP_BUTTON && s != -1) // sector mode
    {
        prog->features[F_REDRAW] = 1;
        prog->modes[prog->mode_id].buttons[prog->button_on].vis_lit_on[2] = FALSE;
        prog->button_on = SECTOR_BUTTON;
        prog->modes[prog->mode_id].buttons[prog->button_on].vis_lit_on[2] = TRUE;
        media->worlds[media->world_id].sectors[s].floor++;
    }
    else if (prog->button_on == F_DOWN_BUTTON && s != -1) // sector mode
    {

        prog->features[F_REDRAW] = 1;
        prog->modes[prog->mode_id].buttons[prog->button_on].vis_lit_on[2] = FALSE;
        prog->button_on = SECTOR_BUTTON;
        prog->modes[prog->mode_id].buttons[prog->button_on].vis_lit_on[2] = TRUE;
        media->worlds[media->world_id].sectors[s].floor--;
    }
    else if (prog->button_on == C_UP_BUTTON && s != -1) // sector mode
    {

        prog->features[F_REDRAW] = 1;
        prog->modes[prog->mode_id].buttons[prog->button_on].vis_lit_on[2] = FALSE;
        prog->button_on = SECTOR_BUTTON;
        prog->modes[prog->mode_id].buttons[prog->button_on].vis_lit_on[2] = TRUE;
        media->worlds[media->world_id].sectors[s].ceiling++;
    }
    else if (prog->button_on == C_DOWN_BUTTON && s != -1) // sector mode
    {

        prog->features[F_REDRAW] = 1;
        prog->modes[prog->mode_id].buttons[prog->button_on].vis_lit_on[2] = FALSE;
        prog->button_on = SECTOR_BUTTON;
        prog->modes[prog->mode_id].buttons[prog->button_on].vis_lit_on[2] = TRUE;
        media->worlds[media->world_id].sectors[s].ceiling--;
    }
	if (prog->zoom != 0)
	{
		zoom_grid(prog, sdl->mouse, grid);
	}
	update_sector_status(media->worlds[world_id].sectors, media->worlds[world_id].walls, media->worlds[world_id].vertices, media->worlds[world_id].n_sectors);
}

int						input_editor(t_sdl *sdl, t_grid *grid, t_media *media, t_prog *prog)
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
				prog->move = sdl->mouse;
			if(event.type == SDL_MOUSEBUTTONUP)
			{
                if (prog->button_lit == BACK_BUTTON)
                {
					prog->last_mode_id = prog->mode_id;
                    prog->mode_id = MODE_SUMMARY;
					prog->modes[prog->mode_id].buttons[prog->button_lit].vis_lit_on[2] = FALSE;
					prog->button_lit = -1;
                    prog->button_on = -1;
                    media->world_id = -1;
                    refresh_level_list(media, &prog->modes[MODE_SUMMARY], sdl);
                    return (quit);
                }
                if (prog->button_lit == FT_EDIT_BUTTON)
                {
                    prog->features[F_REDRAW] = 1;
                    prog->modes[prog->mode_id].buttons[prog->button_on].vis_lit_on[2] = FALSE;
					prog->last_mode_id = prog->mode_id;
                    prog->mode_id = MODE_TEXTURES;
                    prog->button_lit = -1;
                    if (grid->active[0].y >= 0 && grid->active[0].y < media->n_textures)
					{
                    	prog->button_on = media->worlds[media->world_id].sectors[grid->active[0].y].floor_txtr;
						prog->modes[prog->mode_id].buttons[prog->button_on].vis_lit_on[2] = TRUE;
					}
                    else
						prog->button_on = -1;
                    return (quit);
                }
				if (prog->button_lit == WALL_BUTTON)
				{
					prog->features[F_REDRAW] = 1;
					prog->modes[prog->mode_id].buttons[prog->button_on].vis_lit_on[2] = FALSE;
					prog->last_mode_id = prog->mode_id;
					prog->mode_id = MODE_WALLS;
					prog->button_lit = -1;
					prog->button_on = -1;
					prog->move.x = 0;
					prog->move.y = 0;
					return (quit);
				}
				prog->move.x = 0;
				prog->move.y = 0;
			}
		}
	}
	return (quit);
}