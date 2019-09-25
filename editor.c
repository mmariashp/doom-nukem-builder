
#include "builder.h"

void					place_player_icons(t_world world, t_grid *grid, t_sdl *sdl)
{
	t_vec2d				node;

	node.x = (int)(grid->box.x + world.p_start.x * grid->scale);
	node.y = (int)(grid->box.y + world.p_start.y * grid->scale);
	write_text("START", sdl, (t_rec){ node.x - 120, node.y + 5, 240, 50 }, WHITE, TRUE);
	node.x = (int)(grid->box.x + world.p_end.x * grid->scale);
	node.y = (int)(grid->box.y + world.p_end.y * grid->scale);
	write_text("END", sdl, (t_rec){ node.x - 120, node.y + 5, 240, 50 }, WHITE, TRUE);
}

char 					*write_type(int type)
{
	static char 		name[TOTAL_TYPES][25] = {	"TYPE: coin", \
													"TYPE: key", \
													"TYPE: object", \
													"TYPE: enemy", \
													"TYPE: super_bonus", \
													"TYPE: health", \
													"TYPE: ammo", \
													"TYPE: light" };
	if (within(type, -1, TOTAL_TYPES))
	{
		return (name[type]);
	}
	return (NULL);
}

void					render_item_info(t_media *med, t_sdl *sdl, t_mode *mode, SDL_Texture **t)
{
	int					s;
	int					id;
	t_rec				box;
	char				*tmp;
	char				*tmp2;

	if (!sdl || !med || !mode || !mode->btn || !med->itemfull)
		return ;
	if (within((s = select_it(1, S_SELECT, 1)), -1, \
	med->worlds[med->w_id].n_sec) && within((id = select_it(1, I_SELECT, \
	1)), -1, med->worlds[med->w_id].sec[s].n_items) && within((id = \
	med->worlds[med->w_id].sec[s].items[id].id), -1, med->n_itemfull))
	{
		box = (t_rec){ 20, WIN_H - 120, 350, 48 };
		if (within(TXTR_RECT_Y, -1, TOTAL_TXTRS) && t[TXTR_RECT_Y])
			render_box(box, t[TXTR_RECT_Y], sdl->rend);
		write_text(write_type(med->itemfull[id].type), sdl, box, EDIT_TEXT_COLOR, FALSE);
		box = (t_rec){ 20, WIN_H - 60, 350, 50 };
		if (within(TXTR_RECT_Y, -1, TOTAL_TXTRS) && t[TXTR_RECT_Y])
			render_box(box, t[TXTR_RECT_Y], sdl->rend);
		if ((tmp = ft_strjoin("Name: \"", med->itemfull[id].filename)))
		{
			tmp2 = ft_strjoin(tmp,"\"");
			free(tmp);
			write_text(tmp2, sdl, box, EDIT_TEXT_COLOR, FALSE);
			if (tmp2)
				free(tmp2);
		}
		mode->btn[B_ITEM_EDIT].box = (t_rec){ box.x + box.w , box.y, box.h, box.h };
		mode->btn[B_ITEM_DEL] .box = (t_rec){ box.x + box.w  + box.h, box.y, box.h, box.h };
		mode->btn[B_ITEM_EDIT].vis_lit_on[0] = TRUE;
		mode->btn[B_ITEM_DEL].vis_lit_on[0] = TRUE;
	}
	else
	{
		mode->btn[B_ITEM_EDIT].vis_lit_on[0] = FALSE;
		mode->btn[B_ITEM_DEL].vis_lit_on[0] = FALSE;
	}
}

//void					render_cursor(SDL_Renderer *rend, t_vec2d mouse)
//{
//	SDL_Texture	*t = load_texture("./buttons/interact.png", rend, 0);
//	SDL_Rect			rect;
//
//	if (rend && t)
//	{
//		rect = (SDL_Rect){ mouse.x , mouse.y,
//						   30, 30 };
//		SDL_RenderCopy(rend, t, NULL, &rect);
//		SDL_DestroyTexture(t);
//	}
//}

void					r_editor(t_sdl *sdl, t_grid *grid, t_media *media, t_prog *prog)
{
	int					state;

	if (!sdl || !media || !grid)
		return ;

	if (prog->features[F_REDRAW])
	{
		SDL_SetRenderDrawColor(sdl->rend, 55, 55, 55, 255);
		SDL_RenderClear(sdl->rend);
		state = select_it(1, ST_SELECT, -1);
		grid_refresh(grid, media, state, select_it(1, S_SELECT, -1));

		if (state == NORMAL && prog->btn_on == ISO_BTN)
			render_grid_iso(media->worlds[media->w_id], grid, prog);
		else
			render_grid(media->worlds[media->w_id], grid, prog, sdl->mouse);
		render_screen(sdl->rend, prog->screen);
		if (state == SECTOR_EDIT || state == WALL_EDIT)
		{
			if (state == SECTOR_EDIT)
			{
				render_items(sdl->rend, &media->worlds[media->w_id], media->itemfull, media->n_itemfull, grid, prog->t);
				render_item_info(media, sdl, &prog->modes[prog->mode_id], prog->t);
			}
			render_edit_menu(sdl, media->txtrs, &media->worlds[media->w_id], state, media->n_txtrs, prog->t);
		}
		render_btn(prog->modes[prog->mode_id].btn, sdl, prog->modes[prog->mode_id].n_btn, prog->mode_id, prog->t);
		if (state == NORMAL && prog->btn_on == PLAYER_BTN)
			place_player_icons(media->worlds[media->w_id], grid, sdl);
		SDL_RenderPresent(sdl->rend);
		prog->features[F_REDRAW] = 0;
	}

//	if (!sdl || !media || !grid)
//		return ;
//	SDL_SetRenderDrawColor(sdl->rend, 55, 55, 55, 255);
//	SDL_RenderClear(sdl->rend);
//	state = select_it(1, ST_SELECT, -1);
//	if (prog->features[F_REDRAW])
//	{
//		grid_refresh(grid, media, state, select_it(1, S_SELECT, -1));
//		render_grid(media->worlds[media->w_id], grid, prog, sdl->mouse);
//	}
//	render_screen(sdl->rend, prog->screen);
//	if (state == SECTOR_EDIT || state == WALL_EDIT)
//	{
//		if (state == SECTOR_EDIT)
//		{
//			render_items(sdl->rend, &media->worlds[media->w_id], media->itemfull, media->n_itemfull, grid);
//			render_item_info(media, sdl->rend, &prog->modes[prog->mode_id]);
//		}
//		render_edit_menu(sdl->rend, media->txtrs, &media->worlds[media->w_id], state, media->n_txtrs);
//	}
//	render_btn(prog->modes[prog->mode_id].btn, sdl->rend, prog->modes[prog->mode_id].n_btn, prog->mode_id);
//	if (state == NORMAL && prog->btn_on == PLAYER_BTN)
//		place_player_icons(media->worlds[media->w_id], grid, sdl->rend);
//	render_cursor(sdl->rend, sdl->mouse);
//	SDL_RenderPresent(sdl->rend);
//	prog->features[F_REDRAW] = 0;
}

unsigned short			save_media(t_media *media, t_prog *prog)
{
	if (!media || !prog || !prog->modes || !prog->modes[prog->mode_id].btn\
	|| rewrite_media(media) == FAIL)
		return (FAIL);
	prog->btn_on = DRAG_BTN;
	prog->modes[prog->mode_id].btn[prog->btn_on].vis_lit_on[2] = TRUE;
	prog->modes[prog->mode_id].btn[SAVE_BTN].vis_lit_on[2] = FALSE;
	return (SUCCESS);
}

unsigned short			open_level(t_media *media, t_prog* prog, t_grid *grid)
{
	int 				w;

	if (!media || !prog || !grid || \
	!within(media->w_id, -1, media->n_worlds + 1))
		return (FAIL);
	w = media->w_id;
	clean_grid(grid);
	if (w == media->n_worlds)
	{
		if (add_world(&media->worlds, media->n_worlds, media->extensions[0], \
		media->paths[0]) == FAIL)
			return (FAIL);
		media->n_worlds++;
	}
	else if (media->worlds[w].n_sec > 0)
		zoom_to_map(media->worlds[w].n_vecs, media->worlds[w].vecs, grid);
	fill_grid(media->worlds[w].n_vecs, media->worlds[w].vecs, grid);
	update_sector_status(media->worlds[w].sec, media->worlds[w].walls, \
	media->worlds[w].vecs, media->worlds[w].n_sec);
	prog->features[F_REDRAW] = 1;
	return (SUCCESS);
}

void					change_heights(int b, t_sec *sec)
{
	int					s;
	int					f_shift;
	int					c_shift;

	s = select_it(1, S_SELECT, -1);
	f_shift = 0;
	c_shift = 0;
	if (b == F_UP_BTN)
		f_shift++;
	else if (b == F_DOWN_BTN)
		f_shift--;
	else if (b == C_UP_BTN)
		c_shift++;
	else if (b == C_DOWN_BTN)
		c_shift--;
	if (f_shift || c_shift)
	{
		sec[s].floor += f_shift;
		sec[s].ceiling += c_shift;
		sec[s].floor = clamp(sec[s].floor, MIN_HEIGHT, MAX_HEIGHT);
		sec[s].ceiling = clamp(sec[s].ceiling, MIN_HEIGHT, MAX_HEIGHT);
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
	state = select_it(1, ST_SELECT, -1);
	sector = select_it(1, S_SELECT, -1);
	wall = select_it(1, W_SELECT, -1);
	if (within((texture = select_it(1, T_SELECT, -1)), -1, n_txtrs))
	{
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

	if (!world || \
	!within((wall = select_it(1, W_SELECT, -1)), -1, world->n_walls))
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
			world->walls[wall].type = WALL_EMPTY;
	}
	else if (btn_on == W_DOOR_BTN)
	{
		if (world->walls[wall].type == WALL_FILLED)
			world->walls[wall].type = WALL_EMPTY;
		if (world->walls[wall].door != -1)
			delete_door(world, wall);
		else
			add_door(world, wall);
	}
}

void					edit_item_name(int n_itemfull, t_itemfull *itemfull, t_world *world)
{
	int 				sector;
	int 				item_i;
	int 				item_sel;

	if (!itemfull || !world)
		return ;
	sector = select_it(1, S_SELECT, -1);
	item_i = select_it(1, I_SELECT, -1);
	item_sel = select_it(1, SEL_I_SELECT, -1);
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
	if (prog->last_mode_id == MODE_LEVELS) // when opening a map
	{
		prog->last_mode_id = prog->mode_id;
		prog->btn_on = -1;
		prog->btn_lit = -1;
		return(open_level(media, prog, grid));
	}
	if (prog->last_mode_id == MODE_TEXTURES)
		edit_texture(floor_ceil, media->n_txtrs, media->txtrs, &media->worlds[media->w_id]);
	if (prog->last_mode_id == MODE_SEL_ITEM)
		edit_item_name(media->n_itemfull, media->itemfull, &media->worlds[media->w_id]);
	prog->last_mode_id = prog->mode_id;
	return (SUCCESS);
}

void					btn_refresh(t_prog *prog, int state, int *last)
{
	if (!prog || !last)
		return ;
//	free_btn(prog->modes[prog->mode_id].btn, prog->modes[prog->mode_id].n_btn);
	get_btn(state, &prog->modes[prog->mode_id]);
	prog->btn_on = -1;
	prog->btn_lit = -1;
	if (state == SECTOR_SEARCH)
		prog->btn_on = SECTOR_BTN;
	else if (state == WALL_SEARCH)
		prog->btn_on = WALL_BTN;
	else if (state == NORMAL)
		prog->btn_on = DRAG_BTN;
	if (within(prog->btn_on, -1, prog->modes[prog->mode_id].n_btn))
		prog->modes[prog->mode_id].btn[prog->btn_on].vis_lit_on[2] = TRUE;
	*last = state;
	prog->features[F_REDRAW] = 1;
}

void 					drawing(t_media *media, t_prog *prog, t_grid *grid, t_sdl *sdl)
{
	int 				i;
	unsigned short		vis;

	if ((prog->click.x || prog->click.y) && mouse_over(grid->box, sdl->mouse))
	{
		if (grid->active[0].x == -1)
			grid->active[0] = find_node(sdl->mouse.x, sdl->mouse.y, grid);
		else if (grid->active[1].x == -1)
			grid->active[1] = find_node(sdl->mouse.x, sdl->mouse.y, grid);
		if (grid->active[0].x != -1)
			add_to_media(grid, media);
		vis = grid->active[0].x != -1 ? FALSE : TRUE;
		i = 0;
		while (i < prog->modes[prog->mode_id].n_btn)
			prog->modes[prog->mode_id].btn[i++].vis_lit_on[0] = vis;
		prog->features[F_REDRAW] = 1;
		prog->click = (t_vec2d){ 0, 0 };
	}
	else if (grid->active[0].x != -1 && grid->active[1].x == -1)
		prog->features[F_REDRAW] = 1;
}

unsigned short			u_editor(t_sdl *sdl, t_grid *grid, t_media *media, t_prog *prog)
{
	int                 texture;
	static int			last = -2; // last state
	static int			floor_ceil = 0;
	int					sector;
	int 				wall;
	t_vec2d				grid_node;
	int					state = select_it(1, ST_SELECT, -1);

	if (!sdl || !media || !grid || !prog)
		return (FAIL);
	last = prog->last_mode_id == MODE_LEVELS ? -2 : last; // to unlight btn
	if (prog->last_mode_id != prog->mode_id) // coming from other modes
		return(mode_change(prog, media, grid, floor_ceil));
	if (prog->zoom != 0)
		zoom_grid(prog, sdl->mouse, grid);
	if (prog->move.x || prog->move.y)
		return(move_grid_keys(prog, grid));
	if (state == NORMAL && prog->btn_on == SAVE_BTN) // when saving
		return (save_media(media, prog));
	if (last != state)
		btn_refresh(prog, state, &last);
	if (prog->btn_lit != -1 && (prog->click.x || prog->click.y)) // when pressing an on screen button
	{
		prog->features[F_REDRAW] = 1;
		if (within(prog->btn_on, -1, prog->modes[prog->mode_id].n_btn) == TRUE)
			prog->modes[prog->mode_id].btn[prog->btn_on].vis_lit_on[2] = FALSE;
		prog->btn_on = prog->btn_lit;
		prog->click = (t_vec2d){ 0, 0 };
		prog->modes[prog->mode_id].btn[prog->btn_on].vis_lit_on[2] = TRUE;
		grid->active[0] = (t_vec2d){ -1, -1 };
		grid->active[1] = (t_vec2d){ -1, -1 };

		if (state == NORMAL || state == SECTOR_SEARCH || state == WALL_SEARCH)
		{
			if (prog->btn_on == SECTOR_BTN)
				select_it(0, ST_SELECT, SECTOR_SEARCH);
			else if (prog->btn_on == WALL_BTN)
			{
				select_it(0, ST_SELECT, WALL_SEARCH);
			}
			else
				select_it(0, ST_SELECT, NORMAL);
		}
		else if (state == SECTOR_EDIT || state == WALL_EDIT)
		{
			if (prog->btn_on == DESELECT_BTN)
			{
				zoom_to_map(media->worlds[media->w_id].n_vecs, media->worlds[media->w_id].vecs, grid);
				if (state == SECTOR_EDIT)
					select_it(0, ST_SELECT, SECTOR_SEARCH);
				else
				{
					select_it(0, ST_SELECT, WALL_SEARCH);
					select_it(0, W_SELECT, -1);
				}
			}
			else if ((state == SECTOR_EDIT && (prog->btn_on == FT_EDIT_BTN || prog->btn_on == CT_EDIT_BTN)) ||
					(state == WALL_EDIT && prog->btn_on == WT_EDIT_BTN)) //editing textures
			{
				texture = -1;
				if (state == SECTOR_EDIT && within((sector = select_it(1, S_SELECT, -1)), -1, media->worlds[media->w_id].n_sec))
				{
					floor_ceil = prog->btn_on == FT_EDIT_BTN ? 0 : 1;
					texture = floor_ceil == 0 ? media->worlds[media->w_id].sec[sector].floor_txtr :
							  media->worlds[media->w_id].sec[sector].ceil_txtr;
				}
				else if (state == WALL_EDIT && within((wall = select_it(1, W_SELECT, -1)), -1, media->worlds[media->w_id].n_walls))
					texture = media->worlds[media->w_id].walls[wall].txtr;
				prog->modes[prog->mode_id].btn[prog->btn_on].vis_lit_on[2] = FALSE;
				prog->btn_lit = -1;
				prog->btn_on = -1;
				prog->mode_id = MODE_TEXTURES;
				if (within(texture, -1, media->n_txtrs))
				{
					prog->btn_on = texture;
					prog->modes[prog->mode_id].btn[prog->btn_on].vis_lit_on[2] = TRUE;
					select_it(0, T_SELECT, texture);
				}
			}
			else if (state == WALL_EDIT && (prog->btn_on == W_PORTAL_BTN || prog->btn_on == W_DOOR_BTN))
			{
				edit_wall_type(prog->btn_on, &media->worlds[media->w_id]);
				prog->modes[prog->mode_id].btn[prog->btn_on].vis_lit_on[2] = FALSE;
				prog->btn_on = -1;
			}
			else if (state == SECTOR_EDIT)
			{
				if (prog->btn_on == DEL_SEC_BTN)
				{
					zoom_to_map(media->worlds[media->w_id].n_vecs, media->worlds[media->w_id].vecs, grid);
					select_it(0, ST_SELECT, SECTOR_SEARCH);
					delete_sector(select_it(1, S_SELECT, -1), &media->worlds[media->w_id]);
					select_it(0, S_SELECT, -1);
					return (SUCCESS);
				}
				if (prog->btn_on == B_ITEM_DEL)
				{
					prog->btn_lit = -1;
					prog->modes[prog->mode_id].btn[prog->btn_on].vis_lit_on[2] = FALSE;
					prog->btn_on = -1;
					delete_item(&media->worlds[media->w_id].sec[select_it(1, S_SELECT, -1)], select_it(1, I_SELECT, -1));
					select_it(0, I_SELECT, -1);
				}
				else if (within(prog->btn_on, F_UP_BTN - 1, C_DOWN_BTN + 1))
				{
					change_heights(prog->btn_on, media->worlds[media->w_id].sec);
					prog->btn_lit = -1;
					prog->modes[prog->mode_id].btn[prog->btn_on].vis_lit_on[2] = FALSE;
					prog->btn_on = -1;
				}
				else if (prog->btn_on == B_ITEM_EDIT)
				{
					prog->btn_lit = -1;
					prog->modes[prog->mode_id].btn[prog->btn_on].vis_lit_on[2] = FALSE;
					prog->btn_on = -1;
					prog->last_mode_id = prog->mode_id;
					prog->mode_id = MODE_SEL_ITEM;
					sector = select_it(1, S_SELECT, -1);
					int item_i = select_it(1, I_SELECT, -1);
					if (within(sector, -1, media->worlds[media->w_id].n_sec) && \
					within(item_i, -1, media->worlds[media->w_id].sec[sector].n_items) && \
					within(media->worlds[media->w_id].sec[sector].items[item_i].id, -1, media->n_itemfull))
					{
						select_it(0, SEL_I_SELECT, media->worlds[media->w_id].sec[sector].items[item_i].id);
						prog->btn_on = media->worlds[media->w_id].sec[sector].items[item_i].id;
						prog->modes[prog->mode_id].btn[prog->btn_on].vis_lit_on[2] = TRUE;
					}
					prog->features[F_REDRAW] = 0;
				}

			}
		}
		prog->click = (t_vec2d){ 0, 0 };
		return (SUCCESS);
	}
	if (light_button(sdl, prog->modes[prog->mode_id].btn, prog->modes[prog->mode_id].n_btn, prog) == SUCCESS) // when mouse is over a button
		prog->features[F_REDRAW] = 1;
	else // managing interactions not concerning btn
	{
		if (state == SECTOR_SEARCH)
		{
			sector = lit_item(0, S_SELECT, mouse_in_sector(sdl->mouse, &media->worlds[media->w_id], grid));
			if ((prog->click.x || prog->click.y) && within(sector, -1, media->worlds[media->w_id].n_sec) == TRUE)
			{
				select_it(0, S_SELECT, sector);
				select_it(0, ST_SELECT, SECTOR_EDIT);
				zoom_to_sector(&media->worlds[media->w_id].sec[sector], media->worlds[media->w_id].vecs, grid, prog);
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
					if ((prog->click.x || prog->click.y) && within(wall, -1, media->worlds[media->w_id].n_walls) == TRUE)
					{
						select_it(0, W_SELECT, wall);
						select_it(0, ST_SELECT, WALL_EDIT);
						int v1 = media->worlds[media->w_id].walls[wall].v1;
						int v2 = media->worlds[media->w_id].walls[wall].v2;
						if (within(v1, -1, media->worlds[media->w_id].n_vecs) &&
							within(v2, -1, media->worlds[media->w_id].n_vecs))
							zoom_to_wall(media->worlds[media->w_id].vecs[v1], media->worlds[media->w_id].vecs[v2], grid, prog);

					}
					prog->features[F_REDRAW] = 1;
				}
			}
		}
		else if (state == SECTOR_EDIT)
		{
			sector = select_it(1, S_SELECT, -1);
			if (mouse_in_sector(sdl->mouse, &media->worlds[media->w_id], grid) == sector)
			{
				if (prog->btn_on == -1)
					move_item(prog, sdl->mouse, grid, &media->worlds[media->w_id].sec[sector]);
				else if (prog->click.x || prog->click.y)
					add_item(find_default_item(prog->btn_on - B_COIN, media->itemfull, media->n_itemfull), sdl->mouse, grid, &media->worlds[media->w_id].sec[sector]);
			}
			if (prog->btn_on != -1 && (prog->click.x || prog->click.y))
			{
				prog->modes[prog->mode_id].btn[prog->btn_on].vis_lit_on[2] = FALSE;
				prog->btn_on = -1;
				prog->features[F_REDRAW] = 1;
				prog->click = (t_vec2d){ 0, 0 };
			}
		}
		else if (state == NORMAL)
		{
			if (prog->btn_on == DRAW_BTN)
				drawing(media, prog, grid, sdl);
			else if (prog->btn_on == DISTORT_BTN ) // move mode
				move_vector(prog, sdl->mouse, grid, &media->worlds[media->w_id]);
			else if (prog->btn_on == PLAYER_BTN ) // move mode
				move_player(prog, sdl->mouse, grid, &media->worlds[media->w_id]);
			else if (prog->btn_on == DRAG_BTN && (prog->click.x || prog->click.y)) // view mode
				move_grid_drag(prog, sdl->mouse, grid);
			else if (prog->btn_on == ISO_BTN && (prog->click.x || prog->click.y)) // delete mode
			{
				move_grid_drag(prog, sdl->mouse, grid);
			}
		}
	}
	update_sector_status(media->worlds[media->w_id].sec, media->worlds[media->w_id].walls,
			media->worlds[media->w_id].vecs, media->worlds[media->w_id].n_sec);
	return (SUCCESS);
}

int						i_editor(t_sdl *sdl, t_grid *grid, t_media *media, t_prog *prog)
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
		else if(event.type == SDL_MOUSEWHEEL && event.wheel.y)
			prog->zoom += event.wheel.y > 0 ? SCROLL_UP : SCROLL_DOWN;
		else if( event.type == SDL_MOUSEMOTION || event.type == SDL_MOUSEBUTTONDOWN || event.type == SDL_MOUSEBUTTONUP )
		{
			if(event.type == SDL_MOUSEBUTTONDOWN)
				prog->click = sdl->mouse;
			if(event.type == SDL_MOUSEBUTTONUP)
			{
                if (select_it(1, ST_SELECT, -1) == NORMAL && prog->btn_lit == BACK_BTN)
                {
					prog->last_mode_id = prog->mode_id;
                    prog->mode_id = MODE_LEVELS;
					prog->modes[prog->mode_id].btn[prog->btn_lit].vis_lit_on[2] = FALSE;
					prog->btn_lit = -1;
                    prog->btn_on = -1;
                    media->w_id = -1;
                    refresh_level_list(media, &prog->modes[MODE_LEVELS]);
                    break ;
                }
                prog->click = (t_vec2d){ 0, 0 };
			}
		}
	}
	return (quit);
}