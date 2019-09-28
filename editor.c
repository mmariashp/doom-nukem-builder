/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   editor.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mshpakov <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/09/26 12:04:56 by mshpakov          #+#    #+#             */
/*   Updated: 2019/09/26 12:04:58 by mshpakov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


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
	med->worlds[med->w_id].n_s) && within((id = select_it(1, I_SELECT, \
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
		if ((tmp = ft_strjoin("Name: \"", med->itemfull[id].name)))
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

void					render_cursor(SDL_Renderer *rend, t_vec2d mouse, SDL_Texture **t, int txtr_id)
{
	SDL_Rect			rect;

	if (rend && t && within(txtr_id, -1, TOTAL_TXTRS))
	{
		rect = (SDL_Rect){ mouse.x , mouse.y, 30, 30 };
		SDL_RenderCopy(rend, t[txtr_id], NULL, &rect);
	}
}



void					r_editor(t_sdl *sdl, t_grid *grid, t_media *media, t_prog *prog)
{
	int					state;

	if (!sdl || !media || !grid)
		return ;
	if (prog->redraw)
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
		if (state == SEC_EDIT || state == WALL_EDIT)
		{
			if (state == SEC_EDIT)
			{
				render_items(sdl->rend, &media->worlds[media->w_id], media->itemfull, media->n_itemfull, grid, prog->t);
				render_item_info(media, sdl, &prog->modes[prog->m_id], prog->t);
			}
			render_edit_menu(sdl, media->txtrs, &media->worlds[media->w_id], state, media->n_txtrs, prog->t);
		}
		render_btns(prog, sdl);
		if (state == NORMAL && prog->btn_on == PLAYER_BTN)
			place_player_icons(media->worlds[media->w_id], grid, sdl);
		if (state == WALL_SEARCH && prog->btn_on == DOOR_ADD_BTN && prog->btn_lit == -1)
		{
			render_cursor(sdl->rend, sdl->mouse, prog->t, TXTR_DOOR);
		}
		SDL_RenderPresent(sdl->rend);
		prog->redraw = 0;
	}
}

unsigned short			save_media(t_media *media, t_prog *prog)
{
	if (!media || !prog || !prog->modes || !prog->modes[prog->m_id].btn\
	|| rewrite_media(media) == FAIL)
		return (FAIL);
	prog->btn_on = DRAG_BTN;
	prog->modes[prog->m_id].btn[prog->btn_on].vis_lit_on[2] = TRUE;
	prog->modes[prog->m_id].btn[SAVE_BTN].vis_lit_on[2] = FALSE;
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
	else if (media->worlds[w].n_s > 0)
		zoom_to_map(media->worlds[w].n_v, media->worlds[w].vecs, grid);
	fill_grid(media->worlds[w].n_v, media->worlds[w].vecs, grid);
	upd_sec(media->worlds[w].sec, media->worlds[w].walls, \
	media->worlds[w].vecs, media->worlds[w].n_s);
	prog->redraw = 1;
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
		sec[s].fl += f_shift;
		sec[s].ceiling += c_shift;
		sec[s].fl = clamp(sec[s].fl, MIN_HEIGHT, MAX_HEIGHT);
		sec[s].ceiling = clamp(sec[s].ceiling, MIN_HEIGHT, MAX_HEIGHT);
	}
}

unsigned short			edit_texture(int fl_ceil, int n_txtrs, t_texture *txtrs, t_world *world)
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
		if (state == SEC_EDIT && within(sector, -1, world->n_s))
		{
			if (fl_ceil == 0)
				world->sec[sector].fl_txtr = texture ;
			else
				world->sec[sector].ceil_txtr = texture ;
		}
		else if (state == WALL_EDIT && within(wall, -1, world->n_w))
			world->walls[wall].txtr = texture ;
	}
	return (SUCCESS);
}

void					edit_wall_type(int btn_on, t_world *world)
{
	int 				wall;

	if (!world || \
	!within((wall = select_it(1, W_SELECT, -1)), -1, world->n_w))
		return ;
	if (btn_on == W_PORTAL_BTN)
	{
		if (world->walls[wall].type != WALL_FILLED)
			world->walls[wall].type = WALL_FILLED;
		else if (world->walls[wall].type == WALL_FILLED)
			world->walls[wall].type = WALL_EMPTY;
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
	if (within(sector, -1, world->n_s) && \
		within(item_i, -1, world->sec[sector].n_items) && \
		within(item_sel, -1, n_itemfull))
		world->sec[sector].items[item_i].id = item_sel;
}

unsigned short			mode_change(t_prog *prog, t_media *media, t_grid *grid, int fl_ceil)
{
	if (!prog || !media || !grid)
		return (FAIL);
	select_it(0, LAST_ST_SELECT, prog->last == MODE_LEVELS ? -2 :
	select_it(1, LAST_ST_SELECT, 1));
	if (prog->last == MODE_LEVELS)
	{
		prog->last = prog->m_id;
		prog->btn_on = -1;
		prog->btn_lit = -1;
		return(open_level(media, prog, grid));
	}
	if (prog->last == MODE_TEXTURES)
		edit_texture(fl_ceil, media->n_txtrs, media->txtrs, &media->worlds[media->w_id]);
	if (prog->last == MODE_SEL_ITEM)
		edit_item_name(media->n_itemfull, media->itemfull, &media->worlds[media->w_id]);
	prog->last = prog->m_id;
	return (SUCCESS);
}

unsigned short			btn_refresh(t_prog *prog, int state)
{
	int 				on;

	if (!prog)
		return (FAIL);
	on = prog->btn_on;
	on = select_it(1, LAST_ST_SELECT, -1) == WALL_EDIT ? WALL_BTN : on;
	get_btn(state, &prog->modes[prog->m_id]);
	turn_btns_off(prog);
	if (state == SEC_SEARCH)
		prog->btn_on = SEC_BTN;
	else if (state == WALL_SEARCH)
		prog->btn_on = on;
	else if (state == NORMAL)
		prog->btn_on = DRAG_BTN;
	if (within(prog->btn_on, -1, prog->modes[prog->m_id].n_btn))
		prog->modes[prog->m_id].btn[prog->btn_on].vis_lit_on[2] = TRUE;
	select_it(0, LAST_ST_SELECT, state);
	prog->redraw = 1;
	return (SUCCESS);
}

void 					drawing(t_world *world, t_prog *prog, t_grid *grid, t_vec2d mouse)
{
	int 				i;
	unsigned short		vis;

	if ((prog->click.x || prog->click.y) && mouse_over(grid->box, mouse))
	{
		if (grid->p[0].x == -1)
			grid->p[0] = find_node(mouse.x, mouse.y, grid);
		else if (grid->p[1].x == -1)
			grid->p[1] = find_node(mouse.x, mouse.y, grid);
		if (grid->p[0].x != -1)
			add_to_media(grid, world);
		vis = grid->p[0].x != -1 ? FALSE : TRUE;
		i = 0;
		while (i < prog->modes[prog->m_id].n_btn)
			prog->modes[prog->m_id].btn[i++].vis_lit_on[0] = vis;
		prog->click = (t_vec2d){ 0, 0 };
	}
	prog->redraw = 1;
}

void					prep_texture_edit(t_world *world, t_prog *prog,
															int n_txtrs)
{
	int                 txtr;
	int 				fc;
	int 				sec;
	int 				wall;

	if (!world || !prog)
		return ;
	txtr = -1;
	if (select_it(1, ST_SELECT, -1) == SEC_EDIT && within((sec = \
	select_it(1, S_SELECT, -1)), -1, world->n_s))
	{
		fc = prog->btn_on == FT_EDIT_BTN ? 0 : 1;
		txtr = !select_it(0, FC_SELECT, fc) ? world->sec[sec].fl_txtr :
			   world->sec[sec].ceil_txtr;
	}
	else if (within((wall = select_it(1, W_SELECT, 0)), -1, world->n_w))
		txtr = world->walls[wall].txtr;
	turn_btns_off(prog);
	prog->btn_lit = -1;
	prog->m_id = MODE_TEXTURES;
	if (within(txtr, -1, get_min(n_txtrs, prog->modes[prog->m_id].n_btn)))
	{
		prog->modes[prog->m_id].btn[(prog->btn_on = txtr)].vis_lit_on[2] = TRUE;
		select_it(0, T_SELECT, txtr);
	}
}

void					normal_st(t_prog *prog, t_vec2d mouse, t_grid *grid, \
																t_world *world)
{
	if (!prog)
		return ;
	if (prog->btn_on == DRAW_BTN)
	{
		drawing(world, prog, grid, mouse);
		upd_sec(world->sec, world->walls, world->vecs, world->n_s);
	}
	else if (prog->btn_on == DISTORT_BTN )
	{
		move_vector(prog, mouse, grid, world);
		upd_sec(world->sec, world->walls, world->vecs, world->n_s);
	}
	else if (prog->btn_on == PLAYER_BTN )
		move_player(prog, mouse, grid, world);
	else if (prog->btn_on == DRAG_BTN && (prog->click.x || prog->click.y))
		move_grid_drag(prog, mouse, grid);
	else if (prog->btn_on == ISO_BTN && (prog->click.x || prog->click.y))
		move_grid_drag(prog, mouse, grid);
}
void					wall_search_st(t_prog *prog, t_vec2d node, \
											t_grid *grid, t_world *world)
{
	int					v1;
	int					v2;
	int 				w;
	
	if (!prog || !world || !grid)
		return ;
	if (within(node.x, -1, GRID_SIZE) && within(node.y, -1, GRID_SIZE))
	{
		w = lit_it(0, W_SELECT, grid->nodes[node.x][node.y]);
		if ((prog->click.x || prog->click.y) && within(w, -1, world->n_w))
		{
			select_it(0, W_SELECT, w);
			if (prog->btn_on == DOOR_ADD_BTN)
				add_door(world, w, grid);
			else
			{
				select_it(0, ST_SELECT, WALL_EDIT);
				if (within((v1 = world->walls[w].v1), -1, world->n_v) &&
					within((v2 = world->walls[w].v2), -1, world->n_v))
					zoom_to_wall(world->vecs[v1], world->vecs[v2], grid, prog);
			}
		}
		prog->redraw = 1;
	}
}
void					sec_search_st(t_prog *prog, t_vec2d mouse, \
											t_grid *grid, t_world *world)
{
	int 				sec;

	if (!prog || !world)
		return ;
	sec = lit_it(0, S_SELECT, mouse_in_stor(mouse, world, grid));
	if ((prog->click.x || prog->click.y) && within(sec, -1, world->n_s))
	{
		select_it(0, S_SELECT, sec);
		select_it(0, ST_SELECT, SEC_EDIT);
		zoom_to_sector(&world->sec[sec], world->vecs, grid, prog);
	}
	prog->redraw = 1;
}
void					sec_edit_st(t_prog *prog, t_vec2d mouse, \
												t_grid *grid, t_media *media)
{
	int 				sector;

	if (!prog)
		return ;
	sector = select_it(1, S_SELECT, -1);
	if (mouse_in_stor(mouse, &media->worlds[media->w_id], grid) == sector)
	{
		if (prog->btn_on == -1)
			move_item(prog, mouse, grid, \
			&media->worlds[media->w_id].sec[sector]);
		else if (prog->click.x || prog->click.y)
			add_item(find_def_item(prog->btn_on - B_COIN, media->itemfull, \
			media->n_itemfull), mouse, grid, \
			&media->worlds[media->w_id].sec[sector]);
	}
	if (prog->btn_on != -1 && (prog->click.x || prog->click.y))
	{
		turn_btns_off(prog);
		prog->redraw = 1;
		prog->click = (t_vec2d){ 0, 0 };
	}
	upd_sec(media->worlds[media->w_id].sec, media->worlds[media->w_id].walls,\
	media->worlds[media->w_id].vecs, media->worlds[media->w_id].n_s);
}

unsigned short			u_editor(t_sdl *sdl, t_grid *grid, t_media *m,
																t_prog *prog)
{
	int					state;
	int 				tmp;

	if (!sdl || !m || !grid || !prog)
		return (FAIL);
	if (prog->last != prog->m_id)
		return (mode_change(prog, m, grid, select_it(1, FC_SELECT, -1)));
	if (prog->zoom != 0)
		zoom_grid(prog, sdl->mouse, grid);
	if (prog->move.x || prog->move.y)
		return (move_grid_keys(prog, grid));
	if (select_it(1, ST_SELECT, -1) == NORMAL && prog->btn_on == SAVE_BTN)
		return (save_media(m, prog));
	if ((tmp = manage_btn(m, prog, grid, sdl->mouse)) < 2)
		return (tmp);
	if ((state = select_it(1, ST_SELECT, -1)) == SEC_SEARCH)
		sec_search_st(prog, sdl->mouse, grid, &m->worlds[m->w_id]);
	else if (state == WALL_SEARCH && mouse_over(grid->box, sdl->mouse))
		wall_search_st(prog, find_node(sdl->mouse.x, sdl->mouse.y, grid), \
		grid, &m->worlds[m->w_id]);
	else if (state == SEC_EDIT)
		sec_edit_st(prog, sdl->mouse, grid, m);
	else
		normal_st(prog, sdl->mouse, grid, &m->worlds[m->w_id]);
	return (SUCCESS);
}

t_vec2d					get_arrow_input(SDL_Keycode key, t_vec2d old)
{
	if (key == SDLK_UP)
		old.y -= 7;
	else if (key == SDLK_DOWN)
		old.y += 7;
	else if (key == SDLK_LEFT)
		old.x -= 7;
	else if (key == SDLK_RIGHT)
		old.x += 7;
	return (old);
}

unsigned short			return_to_levels(t_prog *prog, t_media *media)
{
	if (prog && media && prog->modes)
	{
		prog->last = prog->m_id;
		prog->m_id = MODE_LEVELS;
		turn_btns_off(prog);
		prog->btn_lit = -1;
		media->w_id = -1;
		refresh_level_list(media, &prog->modes[MODE_LEVELS]);
		return (FALSE);
	}
	return (TRUE);
}

int						i_editor(t_sdl *sdl, t_grid *grid, t_media *media,
		t_prog *prog)
{
	SDL_Event			e;

	if (!sdl || !media || !grid || !prog)
		return (TRUE);
	while(SDL_PollEvent(&e))
	{
		SDL_GetMouseState(&sdl->mouse.x, &sdl->mouse.y);
		if (e.type == SDL_QUIT || e.key.keysym.sym == SDLK_ESCAPE)
			return (TRUE);
		else if (e.type == SDL_KEYUP || e.type == SDL_KEYDOWN)
			prog->move = get_arrow_input(e.key.keysym.sym, prog->move);
		else if(e.type == SDL_MOUSEWHEEL && e.wheel.y)
			prog->zoom += e.wheel.y > 0 ? SCROLL_UP : SCROLL_DOWN;
		else if(e.type == SDL_MOUSEBUTTONDOWN)
		{
			prog->click = sdl->mouse;
			break ;
		}
		else if (e.type == SDL_MOUSEBUTTONUP && select_it(1, ST_SELECT, -1)\
		== NORMAL && prog->btn_lit == BACK_BTN)
			return (return_to_levels(prog, media));
		else if (e.type == SDL_MOUSEBUTTONUP)
			prog->click = (t_vec2d){ 0, 0 };
	}
	return (FALSE);
}
