
#include "builder.h"

//t_button				*set_get_free_buttons(char set_get_free, int *n, int state)
//{
//	static int 			n_sec = 4;
//	static int 			n_wall = 4;
//	static t_button		*sec = NULL;
//	static t_button		*wall = NULL;
//	int 				nb;
//
//	if (state == SECTOR_EDIT)
//		nb = n_sec;
//	else if (state == WALL_EDIT)
//		nb = n_wall;
//	else
//		nb = 0;
//	if (set_get_free == 0)
//	{
//		if (state == SECTOR_EDIT && !sec)
//		{
//			if ((sec = init_buttons(n_sec)))
//				*n = nb;
//		}
//		else if (state == WALL_EDIT && !wall)
//		{
//			if ((wall = init_buttons(n_wall)))
//				*n = nb;
//		}
//	}
//	else if (set_get_free == 1)
//	{
//		*n = nb;
//		if (state == SECTOR_EDIT && sec)
//			return (sec);
//		if (state == WALL_EDIT && wall)
//			return (wall);
//	}
//	else if (set_get_free == 2)
//	{
//		if (state == SECTOR_EDIT)
//			free_buttons(sec, n_sec);
//		else if (state == WALL_EDIT)
//			free_buttons(wall, n_wall);
//	}
//	return (NULL);
//}

# define B_EDIT			0
# define B_UP			1
# define B_DOWN			2
# define B_CLOSE		3
# define B_NB			4

//void					standard_buttons(t_button *button, unsigned type, unsigned short free, SDL_Renderer *rend)
//{
//	static char			reg[B_NB][15] = { "edit.png", "up2.png", "down2.png", "cross2.png" };
//	static char			lit[B_NB][15] = { "editlit.png", "up3.png", "down3.png", "cross3.png" };
//
//	if (!button)
//		return ;
//	if (free == TRUE)
//	{
//		if (button->txtr)
//			SDL_DestroyTexture(button->txtr);
//		if (button->lit)
//			SDL_DestroyTexture(button->lit);
//		return ;
//	}
//	if (!button->txtr && type < B_NB)
//		button->txtr = load_texture(reg[type], rend, 0);
//	if (!button->lit && type < B_NB)
//		button->lit = load_texture(lit[type], rend, 0);
//}
//
//void					render_edit_buttons(SDL_Renderer *rend, int state)
//{
//	t_button			*buttons;
//	int 				n;
//	static int 			init = 0;
////	unsigned			i;
//
//	if (init == 0)
//	{
//		set_get_free_buttons(0, &n, SECTOR_EDIT);
//		set_get_free_buttons(0, &n, WALL_EDIT);
//		init = 1;
//	}
////	i = 0;
//	if (!(buttons = set_get_free_buttons(1, &n, state)))
//		return ;
//	standard_buttons(&buttons[0], B_CLOSE, FALSE, rend);
//	t_rec box = layout_menu(0, 0);
//	buttons[0].box =  (t_rec){ box.x + box.w,         box.y,          30, 30 };
//	render_buttons(buttons, rend, n);
//	standard_buttons(&buttons[0], B_CLOSE, TRUE, rend);
//}

t_mode					*init_sector_buttons(t_mode *mode, SDL_Renderer *rend)
{
	t_rec				other;

	if (!mode || !rend)
		return (NULL);
	mode->n_buttons = 7;
	mode->buttons = init_buttons(mode->n_buttons);
	if (!mode->buttons)
		return (NULL);
	mode->buttons[DESELECT_BUTTON].txtr =    load_texture("cross2.png", rend, 0);
	mode->buttons[F_UP_BUTTON].txtr =            load_texture("up2.png", rend, 0);
	mode->buttons[F_DOWN_BUTTON].txtr =          load_texture("down2.png", rend, 0);
	mode->buttons[C_UP_BUTTON].txtr =            load_texture("up2.png", rend, 0);
	mode->buttons[C_DOWN_BUTTON].txtr =          load_texture("down2.png", rend, 0);
	mode->buttons[FT_EDIT_BUTTON].txtr =         load_texture("edit.png", rend, 0);
	mode->buttons[CT_EDIT_BUTTON].txtr =         load_texture("edit.png", rend, 0);

	mode->buttons[DESELECT_BUTTON].lit =      load_texture("cross3.png", rend, 0);
	mode->buttons[F_UP_BUTTON].lit =              load_texture("up3.png", rend, 0);
	mode->buttons[F_DOWN_BUTTON].lit =            load_texture("down3.png", rend, 0);
	mode->buttons[C_UP_BUTTON].lit =              load_texture("up3.png", rend, 0);
	mode->buttons[C_DOWN_BUTTON].lit =            load_texture("down3.png", rend, 0);
	mode->buttons[FT_EDIT_BUTTON].lit =           load_texture("editlit.png", rend, 0);
	mode->buttons[CT_EDIT_BUTTON].lit =           load_texture("editlit.png", rend, 0);

	other = layout_menu(0, 0);
	mode->buttons[DESELECT_BUTTON].box =  (t_rec){ other.x + other.w, other.y, 30, 30 };
	mode->buttons[F_UP_BUTTON].box = layout_menu(3, 1);
	mode->buttons[F_DOWN_BUTTON].box = layout_menu(4, 1);
	mode->buttons[C_UP_BUTTON].box = layout_menu(3, 2);
	mode->buttons[C_DOWN_BUTTON].box = layout_menu(4, 2);
	mode->buttons[FT_EDIT_BUTTON].box = layout_menu(6, 3);
	mode->buttons[CT_EDIT_BUTTON].box = layout_menu(6, 4);
	return (mode);
}

t_mode					*init_wall_buttons(t_mode *mode, SDL_Renderer *rend)
{
	t_rec				other;

	if (!mode || !rend)
		return (NULL);
	mode->n_buttons = 4;
	mode->buttons = init_buttons(mode->n_buttons);
	if (!mode->buttons)
		return (NULL);
	mode->buttons[DESELECT_BUTTON].txtr =    	load_texture("cross2.png", rend, 0);
	mode->buttons[WT_EDIT_BUTTON].txtr =        load_texture("edit.png", rend, 0);
	mode->buttons[W_PORTAL_BUTTON].txtr =        load_texture("edit.png", rend, 0);
	mode->buttons[W_DOOR_BUTTON].txtr =        load_texture("edit.png", rend, 0);

	mode->buttons[DESELECT_BUTTON].lit =      load_texture("cross3.png", rend, 0);
	mode->buttons[WT_EDIT_BUTTON].lit =        load_texture("editlit.png", rend, 0);
	mode->buttons[W_PORTAL_BUTTON].lit =        load_texture("editlit.png", rend, 0);
	mode->buttons[W_DOOR_BUTTON].lit =        load_texture("editlit.png", rend, 0);

	other = layout_menu(0, 0);
	mode->buttons[DESELECT_BUTTON].box =  (t_rec){ other.x + other.w, other.y, 30, 30 };
	mode->buttons[WT_EDIT_BUTTON].box =  layout_menu(6, 1);
	mode->buttons[W_PORTAL_BUTTON].box = layout_menu(6, 2);
	mode->buttons[W_DOOR_BUTTON].box = layout_menu(6, 3);
	return (mode);
}

t_mode					*init_regular_buttons(t_mode *mode, SDL_Renderer *rend)
{
	static t_rec		box = { 10, 0, WIN_H * 0.07 * 8, WIN_H * 0.07};

	if (!mode || !rend)
		return (NULL);
	mode->n_buttons = 8;
	mode->buttons = init_buttons(mode->n_buttons);
	if (!mode->buttons)
		return (NULL);
	distribute_buttons_h(mode->buttons, 0,  mode->n_buttons, box, 3);
	mode->buttons[DRAG_BUTTON].txtr = load_texture("move2.png", rend, 0);
	mode->buttons[DRAW_BUTTON].txtr = load_texture("add2.png", rend, 0);
	mode->buttons[DISTORT_BUTTON].txtr = load_texture("distort2.png", rend, 0);
	mode->buttons[DELETE_BUTTON].txtr = load_texture("delete2.png", rend, 0);
	mode->buttons[BACK_BUTTON].txtr = load_texture("back22.png", rend, 0);
	mode->buttons[SAVE_BUTTON].txtr = load_texture("save2.png", rend, 0);
	mode->buttons[SECTOR_BUTTON].txtr = load_texture("sector22.png", rend, 0);
	mode->buttons[WALL_BUTTON].txtr = load_texture("wall2.png", rend, 0);

	mode->buttons[DRAG_BUTTON].lit = load_texture("move3.png", rend, 0);
	mode->buttons[DRAW_BUTTON].lit = load_texture("add3.png", rend, 0);
	mode->buttons[DISTORT_BUTTON].lit = load_texture("distort3.png", rend, 0);
	mode->buttons[DELETE_BUTTON].lit = load_texture("delete3.png", rend, 0);
	mode->buttons[BACK_BUTTON].lit = load_texture("back3.png", rend, 0);
	mode->buttons[SAVE_BUTTON].lit = load_texture("save3.png", rend, 0);
	mode->buttons[SECTOR_BUTTON].lit = load_texture("sector3.png", rend, 0);
	mode->buttons[WALL_BUTTON].lit = load_texture("wall3.png", rend, 0);
	return (mode);
}

void					get_buttons(int state, t_mode *mode, SDL_Renderer *rend)
{
	if (!mode)
		return ;
	if (state == SECTOR_EDIT)
	{
		mode = init_sector_buttons(mode, rend);
	}
	else if (state == WALL_EDIT)
	{
		mode = init_wall_buttons(mode, rend);
	}
	else
	{
		mode = init_regular_buttons(mode, rend);
	}
}

void					render_editor(t_sdl *sdl, t_grid *grid, t_media *media, t_prog *prog)
{
	int			state = selected_item(1, STATE_SELECT, -1);

	if (!sdl || !media || !grid || prog->features[F_REDRAW] == 0)
		return ;


	SDL_SetRenderDrawColor(sdl->rend, 55, 55, 55, 255);
	SDL_RenderClear(sdl->rend);

	render_grid(media->worlds[media->world_id], grid, prog, sdl->mouse);
	render_screen(sdl->rend, prog->screen);

	if (state == SECTOR_EDIT || state == WALL_EDIT)
		render_edit_menu(sdl->rend, media->txtrs, &media->worlds[media->world_id], state);
	render_buttons(prog->modes[prog->mode_id].buttons, sdl->rend, prog->modes[prog->mode_id].n_buttons);

	write_text(ft_strjoin("STATE is ", ft_itoa(selected_item(1, STATE_SELECT, -1))), sdl->rend,
			(t_rec){ 400, 10, 250, 50 }, WHITE, TRUE);
	write_text(ft_strjoin("SECT is ", ft_itoa(selected_item(1, S_SELECT, -1))), sdl->rend,
			   (t_rec){ 400, 50, 250, 50 }, WHITE, TRUE);
	write_text(ft_strjoin("VECT is ", ft_itoa(selected_item(1, V_SELECT, -1))), sdl->rend,
			   (t_rec){ 400, 90, 250, 50 }, WHITE, TRUE);
	write_text(ft_strjoin("WALL is ", ft_itoa(selected_item(1, W_SELECT, -1))), sdl->rend,
			   (t_rec){ 400, 130, 250, 50 }, WHITE, TRUE);
	SDL_RenderPresent(sdl->rend);
	prog->features[F_REDRAW] = 0;

}

int						texture_in_world(int id, t_world world)
{
    int                 i;

    i = 0;
    while (i < world.n_txtrs)
    {
        if (id == world.textures[i])
            return (i);
        i++;
    }
    return (-1);
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

unsigned short			add_texture(int **textures, short n_txtrs, int id)
{
    *textures = realloc_textures(*textures, n_txtrs + 1);
    if (!*textures)
        return (FAIL);
    (*textures)[n_txtrs] = id;
    return (SUCCESS);
}

unsigned short			save_media(t_media *media, t_prog *prog)
{
	if (!media || !prog || !prog->modes || !prog->modes[prog->mode_id].buttons)
		return (FAIL);
	if (rewrite_media(media) == FAIL)
		return (FAIL);
	prog->save = 0;
	prog->button_on = DRAG_BUTTON;
	prog->modes[prog->mode_id].buttons[prog->button_on].vis_lit_on[2] = TRUE;
	prog->modes[prog->mode_id].buttons[SAVE_BUTTON].vis_lit_on[2] = FALSE;
	return (SUCCESS);
}

unsigned short			open_level(t_media *media, t_prog* prog, t_grid *grid)
{
	if (!media || !prog || !grid || within(media->world_id, -1, media->n_worlds + 1) == FALSE)
		return (FAIL);
	clean_grid(grid);
	if (media->world_id == media->n_worlds) // if ADD WORLD chosen
	{
		if (add_world(&media->worlds, media->n_worlds) == FAIL)
			return (FAIL);
		media->n_worlds++;
	}
	else
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
	if (b == F_UP_BUTTON) // sector mode
		f_shift++;
	else if (b == F_DOWN_BUTTON) // sector mode
		f_shift--;
	else if (b == C_UP_BUTTON) // sector mode
		c_shift++;
	else if (b == C_DOWN_BUTTON) // sector mode
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
	int 				i;
	int 				texture;
	int 				state;

	if (!txtrs)
		return (FAIL);
	i = selected_item(1, T_SELECT, -1);
	state = selected_item(1, STATE_SELECT, -1);
	sector = selected_item(1, S_SELECT, -1);
	wall = selected_item(1, W_SELECT, -1);
	if (within(i, -1, n_txtrs))
	{
		if ((texture = texture_in_world(i, *world)) == -1)
		{
			if (add_texture(&world->textures, world->n_txtrs, i) == FAIL)
				return (FAIL);
			texture = world->n_txtrs++;
		}
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

unsigned short			update_editor(t_sdl *sdl, t_grid *grid, t_media *media, t_prog *prog)
{
	int                 texture;
	static int			last = -2;
	static int			floor_ceil = 0;
	int					sector;
	int					vector;
	int 				wall;
	t_vec2d				grid_node;

	int state = selected_item(1, STATE_SELECT, -1);

	if (!sdl || !media || !grid || !prog)
		return (FAIL);
	if (prog->last_mode_id != prog->mode_id) // coming from other modes
	{
		if (prog->last_mode_id == MODE_SUMMARY) // when opening a map
		{
			prog->last_mode_id = prog->mode_id;
			last = -2;
			prog->button_on = -1;
			prog->button_lit = -1;
			return(open_level(media, prog, grid));
		}
		if (prog->last_mode_id == MODE_TEXTURES)
		{
			edit_texture(floor_ceil, media->n_txtrs, media->txtrs, &media->worlds[media->world_id]);

			prog->last_mode_id = prog->mode_id;
		}
	}
	if (prog->zoom != 0)
		zoom_grid(prog, sdl->mouse, grid);
	if (prog->move.x || prog->move.y)
	{
		move_grid_keys(prog, grid);
		return (SUCCESS);
	}
	if (prog->save == 1 || prog->button_on == SAVE_BUTTON) // when saving
		return (save_media(media, prog));
	if (last != state)
	{
		printf("change of buttons\n");
		free_buttons(prog->modes[prog->mode_id].buttons, prog->modes[prog->mode_id].n_buttons);
		get_buttons(state, &prog->modes[prog->mode_id], sdl->rend);
		prog->button_on = -1;
		prog->button_lit = -1;
		if (state == SECTOR_SEARCH)
			prog->button_on = SECTOR_BUTTON;
		else if (state == WALL_SEARCH)
			prog->button_on = WALL_BUTTON;
		else if (state == NORMAL)
			prog->button_on = DRAG_BUTTON;
		if (within(prog->button_on, -1, prog->modes[prog->mode_id].n_buttons))
			prog->modes[prog->mode_id].buttons[prog->button_on].vis_lit_on[2] = TRUE;
		last = state;
		prog->features[F_REDRAW] = 1;
	}
	if (prog->button_lit != -1 && (prog->click.x || prog->click.y)) // when pressing an on screen button
	{
		if (within(prog->button_on, -1, prog->modes[prog->mode_id].n_buttons) == TRUE)
			prog->modes[prog->mode_id].buttons[prog->button_on].vis_lit_on[2] = FALSE;
		prog->button_on = prog->button_lit;
		prog->modes[prog->mode_id].buttons[prog->button_on].vis_lit_on[2] = TRUE;

		if (state == NORMAL || state == SECTOR_SEARCH || state == WALL_SEARCH)
		{
			clean_grid(grid);
			fill_grid(media->worlds[media->world_id].n_vectors, media->worlds[media->world_id].vertices, grid);
			if (prog->button_on == SECTOR_BUTTON)
				selected_item(0, STATE_SELECT, SECTOR_SEARCH);
			else if (prog->button_on == WALL_BUTTON)
			{
				selected_item(0, STATE_SELECT, WALL_SEARCH);
				fill_grid_walls(media->worlds[media->world_id].n_walls, media->worlds[media->world_id].walls,
								media->worlds[media->world_id].n_vectors, media->worlds[media->world_id].vertices, grid);
			}
			else
			{
				printf("back to normal\n");
				selected_item(0, STATE_SELECT, NORMAL);
			}
		}
		else if (state == SECTOR_EDIT || state == WALL_EDIT)
		{
			if (prog->button_on == DESELECT_BUTTON)
			{
				clean_grid(grid);
				fill_grid(media->worlds[media->world_id].n_vectors, media->worlds[media->world_id].vertices, grid);
				zoom_to_map(media->worlds[media->world_id].n_vectors, media->worlds[media->world_id].vertices, grid);
				if (state == SECTOR_EDIT)
				{
					selected_item(0, STATE_SELECT, SECTOR_SEARCH);
				}
				else
				{
					selected_item(0, STATE_SELECT, WALL_SEARCH);
					selected_item(0, W_SELECT, -1);
					fill_grid_walls(media->worlds[media->world_id].n_walls, media->worlds[media->world_id].walls,
									media->worlds[media->world_id].n_vectors, media->worlds[media->world_id].vertices, grid);
				}
			}
			else if ((state == SECTOR_EDIT && (prog->button_on == FT_EDIT_BUTTON || prog->button_on == CT_EDIT_BUTTON)) ||
					(state == WALL_EDIT && prog->button_on == WT_EDIT_BUTTON))
			{
				texture = -1;
				if (state == SECTOR_EDIT && within((sector = selected_item(1, S_SELECT, -1)), -1, media->worlds[media->world_id].n_sec))
				{
					floor_ceil = prog->button_on == FT_EDIT_BUTTON ? 0 : 1;
					texture = floor_ceil == 0 ? media->worlds[media->world_id].sec[sector].floor_txtr :
							  media->worlds[media->world_id].sec[sector].ceil_txtr;
					texture = media->worlds[media->world_id].textures[texture];
				}
				else if (state == WALL_EDIT &&
				within((wall = selected_item(1, W_SELECT, -1)), -1, media->worlds[media->world_id].n_walls))
					texture = media->worlds[media->world_id].textures[media->worlds[media->world_id].walls[wall].txtr];
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
			else if (state == SECTOR_EDIT && within(prog->button_on, F_UP_BUTTON - 1, C_DOWN_BUTTON + 1))
			{
				change_heights(prog->button_on, media->worlds[media->world_id].sec);
				prog->modes[prog->mode_id].buttons[prog->button_on].vis_lit_on[2] = FALSE;
				prog->button_on = -1;
			}
		}
		prog->features[F_REDRAW] = 1;
		return (SUCCESS);
	}

	if (light_button(sdl, prog->modes[prog->mode_id].buttons, prog->modes[prog->mode_id].n_buttons, prog) == SUCCESS) // when mouse is over a button
		prog->features[F_REDRAW] = 1;

	if (state == SECTOR_SEARCH)
	{
		sector = lit_item(0, S_SELECT, in_sector(sdl->mouse, &media->worlds[media->world_id], grid));
		if ((prog->click.x || prog->click.y) && within(sector, -1, media->worlds[media->world_id].n_sec) == TRUE)
		{
			selected_item(0, S_SELECT, sector);
			selected_item(0, STATE_SELECT, SECTOR_EDIT);
			clean_grid(grid);
			fill_grid(media->worlds[media->world_id].n_vectors, media->worlds[media->world_id].vertices, grid);
			zoom_to_sector(&media->worlds[media->world_id].sec[sector], media->worlds[media->world_id].vertices, grid, prog);
		}
		prog->features[F_REDRAW] = 1;
	}
	else if (state == SECTOR_EDIT)
	{

	}
	else if (state == VECTOR_SEARCH)
	{
		if (mouse_over(grid->box, sdl->mouse) == TRUE)
		{
			grid_node = find_node(sdl->mouse.x, sdl->mouse.y, grid);
			vector = lit_item(0, V_SELECT, find_vector(media->worlds[media->world_id].vertices, grid_node, media->worlds[media->world_id].n_vectors));
			if ((prog->click.x || prog->click.y) && within(vector, -1, media->worlds[media->world_id].n_vectors) == TRUE)
			{
				selected_item(0, V_SELECT, vector);
				selected_item(0, STATE_SELECT, VECTOR_EDIT);
			}
			prog->features[F_REDRAW] = 1;
		}
	}
	else if (state == VECTOR_EDIT)
	{

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
				}
				prog->features[F_REDRAW] = 1;
			}
		}
	}
	else if (state == WALL_EDIT)
	{

	}


	if (prog->button_on == DRAW_BUTTON) // draw mode
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
			prog->click.x = 0;
			prog->click.y = 0;
		}
		else if (grid->active[0].x != -1 && grid->active[1].x == -1)
			prog->features[F_REDRAW] = 1;
	}
	else if (prog->button_on == DISTORT_BUTTON ) // move mode
	{
		move_vector(prog, sdl->mouse, grid, &media->worlds[media->world_id]);
	}
	else if (prog->button_on == DRAG_BUTTON && (prog->click.x || prog->click.y)) // view mode
		move_grid_drag(prog, sdl->mouse, grid);
	else if (prog->button_on == DELETE_BUTTON) // delete mode
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
                prog->click = (t_vec2d){ 0, 0 };
			}
		}
	}
	return (quit);
}