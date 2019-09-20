
#include "builder.h"

void					place_player_icons(t_world world, t_grid *grid, SDL_Renderer *rend)
{
	t_vec2d				node;

	node.x = (int)(grid->box.x + world.p_start.x * grid->scale);
	node.y = (int)(grid->box.y + world.p_start.y * grid->scale);
	write_text("START", rend, (t_rec){ node.x - 120, node.y + 5, 240, 50 }, WHITE, TRUE);
	node.x = (int)(grid->box.x + world.p_end.x * grid->scale);
	node.y = (int)(grid->box.y + world.p_end.y * grid->scale);
	write_text("END", rend, (t_rec){ node.x - 120, node.y + 5, 240, 50 }, WHITE, TRUE);
}

void					render_editor(t_sdl *sdl, t_grid *grid, t_media *media, t_prog *prog)
{
	int			state = selected_item(1, STATE_SELECT, -1);
	int			sector = selected_item(1, S_SELECT, -1);
	int 		id;
	t_rec		box;

	if (!sdl || !media || !grid || prog->features[F_REDRAW] == 0)
		return ;
	grid_refresh(grid, media, state, selected_item(1, S_SELECT, -1));
	SDL_SetRenderDrawColor(sdl->rend, 55, 55, 55, 255);
	SDL_RenderClear(sdl->rend);
	render_grid(media->worlds[media->world_id], grid, prog, sdl->mouse);
	render_screen(sdl->rend, prog->screen);
	if (state == SECTOR_EDIT || state == WALL_EDIT)
	{
		if (state == SECTOR_EDIT)
		{

			render_items(sdl->rend, &media->worlds[media->world_id], media->itemfull, media->n_itemfull, grid);
			id = selected_item(1, I_SELECT, -1);
			if (within(id, -1, media->worlds[media->world_id].sec[sector].n_items))
			{
				id = media->worlds[media->world_id].sec[sector].items[id].id ;
				if (within(id, -1, media->n_itemfull))
				{
					box = (t_rec){ 20, WIN_H - 60, 20 * ft_strlen(media->itemfull[id].filename), 50 };
					write_text(media->itemfull[id].filename, sdl->rend, box, WHITE, FALSE);
					prog->modes[prog->mode_id].buttons[B_ITEM_EDIT].box = (t_rec){ box.x + box.w , box.y, box.h, box.h };
					prog->modes[prog->mode_id].buttons[B_ITEM_DEL] .box = (t_rec){ box.x + box.w  + box.h, box.y, box.h, box.h };
					prog->modes[prog->mode_id].buttons[B_ITEM_EDIT].vis_lit_on[0] = TRUE;
					prog->modes[prog->mode_id].buttons[B_ITEM_DEL].vis_lit_on[0] = TRUE;
				}
			}
			else
			{
				prog->modes[prog->mode_id].buttons[B_ITEM_EDIT].vis_lit_on[0] = FALSE;
				prog->modes[prog->mode_id].buttons[B_ITEM_DEL].vis_lit_on[0] = FALSE;
			}

		}
		render_edit_menu(sdl->rend, media->txtrs, &media->worlds[media->world_id], state, media->n_txtrs);
	}
	render_buttons(prog->modes[prog->mode_id].buttons, sdl->rend, prog->modes[prog->mode_id].n_buttons, prog->mode_id);
	if (state == NORMAL && prog->button_on == PLAYER_BTN)
		place_player_icons(media->worlds[media->world_id], grid, sdl->rend);


	SDL_RenderPresent(sdl->rend);
	prog->features[F_REDRAW] = 0;

}

int					    *realloc_textures(int *textures, int n)
{
    int				    *new;
    int					j;

    j = 0;
    if (n == 0)
        return (NULL);
    if (!(new = (int *)malloc(sizeof(int) * n)))
        return (NULL);
    while (j < n - 1)
    {
        new[j] = textures[j];
        j++;
    }
    free(textures);
    return (new);
}

unsigned short			save_media(t_media *media, t_prog *prog)
{
	if (!media || !prog || !prog->modes || !prog->modes[prog->mode_id].buttons)
		return (FAIL);
	if (rewrite_media(media) == FAIL)
		return (FAIL);
	prog->button_on = DRAG_BTN;
	prog->modes[prog->mode_id].buttons[prog->button_on].vis_lit_on[2] = TRUE;
	prog->modes[prog->mode_id].buttons[SAVE_BTN].vis_lit_on[2] = FALSE;
	return (SUCCESS);
}

unsigned short			open_level(t_media *media, t_prog* prog, t_grid *grid)
{
	if (!media || !prog || !grid || within(media->world_id, -1, media->n_worlds + 1) == FALSE)
		return (FAIL);
	clean_grid(grid);
	if (media->world_id == media->n_worlds) // if ADD WORLD chosen
	{
		if (add_world(&media->worlds, media->n_worlds, media->extensions[0], media->paths[0]) == FAIL)
			return (FAIL);
		media->n_worlds++;
	}
	else if (media->worlds[media->world_id].n_sec > 0)
		zoom_to_map(media->worlds[media->world_id].n_vectors, media->worlds[media->world_id].vertices, grid);
	fill_grid(media->worlds[media->world_id].n_vectors, media->worlds[media->world_id].vertices, grid);
	update_sector_status(media->worlds[media->world_id].sec, media->worlds[media->world_id].walls,
			media->worlds[media->world_id].vertices, media->worlds[media->world_id].n_sec);
	prog->features[F_REDRAW] = 1;
	return (SUCCESS);
}

void					change_heights(int b, t_sector *sec)
{
	int					sector;

	sector = selected_item(1, S_SELECT, -1);
	int f_shift = 0;
	int c_shift = 0;
	if (b == F_UP_BTN) // sector mode
		f_shift++;
	else if (b == F_DOWN_BTN) // sector mode
		f_shift--;
	else if (b == C_UP_BTN) // sector mode
		c_shift++;
	else if (b == C_DOWN_BTN) // sector mode
		c_shift--;
	if (f_shift || c_shift)
	{
		sec[sector].floor += f_shift;
		sec[sector].ceiling += c_shift;
		sec[sector].floor = clamp(sec[sector].floor, MIN_HEIGHT, MAX_HEIGHT);
		sec[sector].ceiling = clamp(sec[sector].ceiling, MIN_HEIGHT, MAX_HEIGHT);
	}
}

unsigned short			edit_texture(int floor_ceil, int n_txtrs, t_texture *txtrs, t_world *world)
{
	int 				sector;
	int 				wall;
	int 				texture;
	int 				state;

	if (!txtrs)
		return (FAIL);
	texture = selected_item(1, T_SELECT, -1);
	state = selected_item(1, STATE_SELECT, -1);
	sector = selected_item(1, S_SELECT, -1);
	wall = selected_item(1, W_SELECT, -1);
	if (within(texture, -1, n_txtrs))
	{
//		if ((texture = texture_in_world(i, *world)) == -1)
//		{
//			if (add_texture(&world->textures, world->n_txtrs, i) == FAIL)
//				return (FAIL);
//			texture = world->n_txtrs++;
//		}
		if (state == SECTOR_EDIT && within(sector, -1, world->n_sec))
		{
			if (floor_ceil == 0)
				world->sec[sector].floor_txtr = texture ;
			else
				world->sec[sector].ceil_txtr = texture ;
		}
		else if (state == WALL_EDIT && within(wall, -1, world->n_walls))
			world->walls[wall].txtr = texture ;
	}
	return (SUCCESS);
}

void					edit_wall_type(int btn_on, t_world *world)
{
	int 				wall;

	if (!world)
		return ;
	wall = selected_item(1, W_SELECT, -1);
	if (!within(wall, -1, world->n_walls))
		return ;
	if (btn_on == W_PORTAL_BTN)
	{
		if (world->walls[wall].type == WALL_EMPTY)
		{
			world->walls[wall].type = WALL_FILLED;
			if (world->walls[wall].door != -1)
				delete_door(world, wall);
		}
		else if (world->walls[wall].type == WALL_FILLED)
		{
			world->walls[wall].type = WALL_EMPTY;
		}
	}
	else if (btn_on == W_DOOR_BTN)
	{
		if (world->walls[wall].door != -1)
			delete_door(world, wall);
		else
		{
			if (world->walls[wall].type == WALL_FILLED)
				world->walls[wall].type = WALL_EMPTY;
			add_door(world, wall);
		}
	}
}

void					fill_grid_items(t_sector *sector, t_grid *grid)
{
	int 				i;
	t_vec2d				p;

	if (!grid || !sector)
		return ;
	i = 0;
	while (i < sector->n_items)
	{
		p = sector->items[i].p;
		if (within(p.x, -1, GRID_SIZE) && within(p.y, -1, GRID_SIZE) && grid->nodes[p.x][p.y] == NODE_EMPTY)
		{
			grid->nodes[p.x][p.y] = (signed char)(-10 - i);
//			printf("i is %d, making node = %d\n", i, grid->nodes[p.x][p.y]);
		}
		else
			delete_item(sector, i--);
		i++;
	}
}

void					grid_refresh(t_grid *grid, t_media *media, int state, int sector)
{
	if (!grid || !media)
		return ;
	clean_grid(grid);
	fill_grid_walls(media->worlds[media->world_id].n_walls,\
	media->worlds[media->world_id].walls,\
	media->worlds[media->world_id].n_vectors,\
	media->worlds[media->world_id].vertices, grid);
	fill_grid(media->worlds[media->world_id].n_vectors,\
	media->worlds[media->world_id].vertices, grid);
	if (state == SECTOR_EDIT && within(sector, -1, media->worlds[media->world_id].n_sec))
		fill_grid_items(&media->worlds[media->world_id].sec[sector], grid);
}

void					edit_item_name(int n_itemfull, t_itemfull *itemfull, t_world *world)
{
	int 				sector;
	int 				item_i;
	int 				item_sel;

	if (!itemfull || !world)
		return ;
	sector = selected_item(1, S_SELECT, -1);
	item_i = selected_item(1, I_SELECT, -1);
	item_sel = selected_item(1, SEL_I_SELECT, -1);
	if (within(sector, -1, world->n_sec) && \
		within(item_i, -1, world->sec[sector].n_items) && \
		within(item_sel, -1, n_itemfull))
	{
		world->sec[sector].items[item_i].id = item_sel;
	}
}

unsigned short			mode_change(t_prog *prog, t_media *media, t_grid *grid, int floor_ceil)
{
	if (!prog || !media || !grid)
		return (FAIL);
	if (prog->last_mode_id == MODE_SUMMARY) // when opening a map
	{
		prog->last_mode_id = prog->mode_id;
		prog->button_on = -1;
		prog->button_lit = -1;
		return(open_level(media, prog, grid));
	}
	if (prog->last_mode_id == MODE_TEXTURES)
		edit_texture(floor_ceil, media->n_txtrs, media->txtrs, &media->worlds[media->world_id]);
	if (prog->last_mode_id == MODE_SEL_ITEM)
		edit_item_name(media->n_itemfull, media->itemfull, &media->worlds[media->world_id]);
	prog->last_mode_id = prog->mode_id;
	return (SUCCESS);
}

void					buttons_refresh(t_prog *prog, int state, int *last, SDL_Renderer *rend)
{
	if (!prog || !last || !rend)
		return ;
	free_buttons(prog->modes[prog->mode_id].buttons, prog->modes[prog->mode_id].n_buttons);
	get_buttons(state, &prog->modes[prog->mode_id], rend);
	prog->button_on = -1;
	prog->button_lit = -1;
	if (state == SECTOR_SEARCH)
		prog->button_on = SECTOR_BTN;
	else if (state == WALL_SEARCH)
		prog->button_on = WALL_BTN;
	else if (state == NORMAL)
		prog->button_on = DRAG_BTN;
	if (within(prog->button_on, -1, prog->modes[prog->mode_id].n_buttons))
		prog->modes[prog->mode_id].buttons[prog->button_on].vis_lit_on[2] = TRUE;
	*last = state;
	prog->features[F_REDRAW] = 1;
}

unsigned short			update_editor(t_sdl *sdl, t_grid *grid, t_media *media, t_prog *prog)
{
	int                 texture;
	static int			last = -2; // last state
	static int			floor_ceil = 0;
	int					sector;
	int 				wall;
	t_vec2d				grid_node;
	int					state = selected_item(1, STATE_SELECT, -1);

	if (!sdl || !media || !grid || !prog)
		return (FAIL);
	last = prog->last_mode_id == MODE_SUMMARY ? -2 : last; // to unlight buttons
	if (prog->last_mode_id != prog->mode_id) // coming from other modes
		return(mode_change(prog, media, grid, floor_ceil));
	if (prog->zoom != 0)
		zoom_grid(prog, sdl->mouse, grid);
	if (prog->move.x || prog->move.y)
		return(move_grid_keys(prog, grid));
	if (state == NORMAL && prog->button_on == SAVE_BTN) // when saving
		return (save_media(media, prog));
	if (last != state)
		buttons_refresh(prog, state, &last, sdl->rend);
	if (prog->button_lit != -1 && (prog->click.x || prog->click.y)) // when pressing an on screen button
	{
		if (within(prog->button_on, -1, prog->modes[prog->mode_id].n_buttons) == TRUE)
			prog->modes[prog->mode_id].buttons[prog->button_on].vis_lit_on[2] = FALSE;
		prog->button_on = prog->button_lit;
		prog->click = (t_vec2d){ 0, 0 };
		prog->modes[prog->mode_id].buttons[prog->button_on].vis_lit_on[2] = TRUE;
		grid->active[0] = (t_vec2d){ -1, -1 };
		grid->active[1] = (t_vec2d){ -1, -1 };

		if (state == NORMAL || state == SECTOR_SEARCH || state == WALL_SEARCH)
		{
			if (prog->button_on == SECTOR_BTN)
				selected_item(0, STATE_SELECT, SECTOR_SEARCH);
			else if (prog->button_on == WALL_BTN)
			{
				selected_item(0, STATE_SELECT, WALL_SEARCH);
			}
			else
				selected_item(0, STATE_SELECT, NORMAL);
		}
		else if (state == SECTOR_EDIT || state == WALL_EDIT)
		{
			if (prog->button_on == DESELECT_BTN)
			{
				zoom_to_map(media->worlds[media->world_id].n_vectors, media->worlds[media->world_id].vertices, grid);
				if (state == SECTOR_EDIT)
					selected_item(0, STATE_SELECT, SECTOR_SEARCH);
				else
				{
					selected_item(0, STATE_SELECT, WALL_SEARCH);
					selected_item(0, W_SELECT, -1);
				}
			}
			else if ((state == SECTOR_EDIT && (prog->button_on == FT_EDIT_BTN || prog->button_on == CT_EDIT_BTN)) ||
					(state == WALL_EDIT && prog->button_on == WT_EDIT_BTN)) //editing textures
			{
				texture = -1;
				if (state == SECTOR_EDIT && within((sector = selected_item(1, S_SELECT, -1)), -1, media->worlds[media->world_id].n_sec))
				{
					floor_ceil = prog->button_on == FT_EDIT_BTN ? 0 : 1;
					texture = floor_ceil == 0 ? media->worlds[media->world_id].sec[sector].floor_txtr :
							  media->worlds[media->world_id].sec[sector].ceil_txtr;
				}
				else if (state == WALL_EDIT && within((wall = selected_item(1, W_SELECT, -1)), -1, media->worlds[media->world_id].n_walls))
					texture = media->worlds[media->world_id].walls[wall].txtr;
				prog->modes[prog->mode_id].buttons[prog->button_on].vis_lit_on[2] = FALSE;
				prog->button_lit = -1;
				prog->button_on = -1;
				prog->mode_id = MODE_TEXTURES;
				if (within(texture, -1, media->n_txtrs))
				{
					prog->button_on = texture;
					prog->modes[prog->mode_id].buttons[prog->button_on].vis_lit_on[2] = TRUE;
					selected_item(0, T_SELECT, texture);
				}
			}
			else if (state == WALL_EDIT && (prog->button_on == W_PORTAL_BTN || prog->button_on == W_DOOR_BTN))
			{
				edit_wall_type(prog->button_on, &media->worlds[media->world_id]);
				prog->modes[prog->mode_id].buttons[prog->button_on].vis_lit_on[2] = FALSE;
				prog->button_on = -1;
			}
			else if (state == SECTOR_EDIT)
			{
				if (prog->button_on == B_ITEM_DEL)
				{
					prog->button_lit = -1;
					prog->modes[prog->mode_id].buttons[prog->button_on].vis_lit_on[2] = FALSE;
					prog->button_on = -1;
					delete_item(&media->worlds[media->world_id].sec[selected_item(1, S_SELECT, -1)], selected_item(1, I_SELECT, -1));
					selected_item(0, I_SELECT, -1);
				}
				else if (within(prog->button_on, F_UP_BTN - 1, C_DOWN_BTN + 1))
				{
					change_heights(prog->button_on, media->worlds[media->world_id].sec);
					prog->button_lit = -1;
					prog->modes[prog->mode_id].buttons[prog->button_on].vis_lit_on[2] = FALSE;
					prog->button_on = -1;
				}
				else if (prog->button_on == B_ITEM_EDIT)
				{
					prog->button_lit = -1;
					prog->modes[prog->mode_id].buttons[prog->button_on].vis_lit_on[2] = FALSE;
					prog->button_on = -1;
					prog->mode_id = MODE_SEL_ITEM;
					sector = selected_item(1, S_SELECT, -1);
					int item_i = selected_item(1, I_SELECT, -1);
					if (within(sector, -1, media->worlds[media->world_id].n_sec) && \
					within(item_i, -1, media->worlds[media->world_id].sec[sector].n_items) && \
					within(media->worlds[media->world_id].sec[sector].items[item_i].id, -1, media->n_itemfull))
					{
						selected_item(0, SEL_I_SELECT, media->worlds[media->world_id].sec[sector].items[item_i].id);
						prog->button_on = media->worlds[media->world_id].sec[sector].items[item_i].id;
						prog->modes[prog->mode_id].buttons[prog->button_on].vis_lit_on[2] = TRUE;
					}
				}

			}
		}
		prog->features[F_REDRAW] = 1;
		prog->click = (t_vec2d){ 0, 0 };
		return (SUCCESS);
	}
	if (light_button(sdl, prog->modes[prog->mode_id].buttons, prog->modes[prog->mode_id].n_buttons, prog) == SUCCESS) // when mouse is over a button
		prog->features[F_REDRAW] = 1;
	else // managing interactions not concerning buttons
	{
		if (state == SECTOR_SEARCH)
		{
			sector = lit_item(0, S_SELECT, mouse_in_sector(sdl->mouse, &media->worlds[media->world_id], grid));
			if ((prog->click.x || prog->click.y) && within(sector, -1, media->worlds[media->world_id].n_sec) == TRUE)
			{
				selected_item(0, S_SELECT, sector);
				selected_item(0, STATE_SELECT, SECTOR_EDIT);
				zoom_to_sector(&media->worlds[media->world_id].sec[sector], media->worlds[media->world_id].vertices, grid, prog);
			}
			prog->features[F_REDRAW] = 1;
		}
		else if (state == WALL_SEARCH)
		{
			if (mouse_over(grid->box, sdl->mouse) == TRUE)
			{
				grid_node = find_node(sdl->mouse.x, sdl->mouse.y, grid);
				if (within(grid_node.x, -1, GRID_SIZE) && within(grid_node.y, -1, GRID_SIZE))
				{
					wall = lit_item(0, W_SELECT, grid->nodes[grid_node.x][grid_node.y]);
					if ((prog->click.x || prog->click.y) && within(wall, -1, media->worlds[media->world_id].n_walls) == TRUE)
					{
						selected_item(0, W_SELECT, wall);
						selected_item(0, STATE_SELECT, WALL_EDIT);
						int v1 = media->worlds[media->world_id].walls[wall].v1;
						int v2 = media->worlds[media->world_id].walls[wall].v2;
						if (within(v1, -1, media->worlds[media->world_id].n_vectors) &&
							within(v2, -1, media->worlds[media->world_id].n_vectors))
							zoom_to_wall(media->worlds[media->world_id].vertices[v1], media->worlds[media->world_id].vertices[v2], grid, prog);

					}
					prog->features[F_REDRAW] = 1;
				}
			}
		}
		else if (state == SECTOR_EDIT)
		{
			sector = selected_item(1, S_SELECT, -1);
			if (mouse_in_sector(sdl->mouse, &media->worlds[media->world_id], grid) == sector)
			{
				if (prog->button_on == -1)
					move_item(prog, sdl->mouse, grid, &media->worlds[media->world_id].sec[sector]);
				else if (prog->click.x || prog->click.y)
					add_item(find_default_item(prog->button_on - B_COIN, media->itemfull, media->n_itemfull), sdl->mouse, grid, &media->worlds[media->world_id].sec[sector]);
			}
			if (prog->button_on != -1 && (prog->click.x || prog->click.y))
			{
				printf("deselect button\n");
				prog->modes[prog->mode_id].buttons[prog->button_on].vis_lit_on[2] = FALSE;
				prog->button_on = -1;
				prog->features[F_REDRAW] = 1;
				prog->click = (t_vec2d){ 0, 0 };
			}
		}
		else if (state == NORMAL)
		{
			if (prog->button_on == DRAW_BTN) // draw mode
			{
				if ((prog->click.x || prog->click.y) && mouse_over(grid->box, sdl->mouse))
				{
					if (grid->active[0].x == -1)
						grid->active[0] = find_node(sdl->mouse.x, sdl->mouse.y, grid);
					else if (grid->active[1].x == -1)
						grid->active[1] = find_node(sdl->mouse.x, sdl->mouse.y, grid);
					if (grid->active[0].x != -1)
						add_to_media(grid, media);
					prog->features[F_REDRAW] = 1;
					prog->click = (t_vec2d){ 0, 0 };
				}
				else if (grid->active[0].x != -1 && grid->active[1].x == -1)
					prog->features[F_REDRAW] = 1;
			}
			else if (prog->button_on == DISTORT_BTN ) // move mode
				move_vector(prog, sdl->mouse, grid, &media->worlds[media->world_id]);
			else if (prog->button_on == PLAYER_BTN ) // move mode
				move_player(prog, sdl->mouse, grid, &media->worlds[media->world_id]);
			else if (prog->button_on == DRAG_BTN && (prog->click.x || prog->click.y)) // view mode
				move_grid_drag(prog, sdl->mouse, grid);
			else if (prog->button_on == DELETE_BTN) // delete mode
			{
				if (prog->click.x || prog->click.y)
				{
					if (mouse_over(grid->box, sdl->mouse))
					{
						int m = -1;
						grid->active[0] = find_node(sdl->mouse.x, sdl->mouse.y, grid);
						if (grid->nodes[grid->active[0].x][grid->active[0].y] == NODE_FULL)
							m = find_vector(media->worlds[media->world_id].vertices, grid->active[0], media->worlds[media->world_id].n_vectors);
						delete_vector(m, &media->worlds[media->world_id]);
						grid->nodes[grid->active[0].x][grid->active[0].y] = NODE_EMPTY;
						prog->click = (t_vec2d){ 0, 0 };
						prog->features[F_REDRAW] = 1;
					}
					else
						prog->click = (t_vec2d){ 0, 0 };
				}
			}
		}

	}
	update_sector_status(media->worlds[media->world_id].sec, media->worlds[media->world_id].walls,
			media->worlds[media->world_id].vertices, media->worlds[media->world_id].n_sec);
	return (SUCCESS);
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
            else if (event.key.keysym.sym == SDLK_UP)
                prog->move.y -= 7;
            else if (event.key.keysym.sym == SDLK_DOWN)
                prog->move.y += 7;
            else if (event.key.keysym.sym == SDLK_LEFT)
                prog->move.x -= 7;
            else if (event.key.keysym.sym == SDLK_RIGHT)
                prog->move.x += 7;
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
				prog->click = sdl->mouse;
			if(event.type == SDL_MOUSEBUTTONUP)
			{
                if (selected_item(1, STATE_SELECT, -1) == NORMAL && prog->button_lit == BACK_BTN)
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
                prog->click = (t_vec2d){ 0, 0 };
			}
		}
	}
	return (quit);
}