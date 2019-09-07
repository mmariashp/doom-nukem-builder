
#include "builder.h"

//void					end(char *reason)
//{
//	ft_putendl("\x1b[32mSuccessful Exit\x1b[33m");
//	if (reason)
//		ft_putendl(reason);
//	ft_putstr("\x1b[0m");
//	exit(EXIT_SUCCESS);
//}
//
//void					my_error(char *reason)
//{
//	ft_putstr("\x1b[31mError\x1b[33m");
//	if (reason)
//	{
//		ft_putstr("\x1b[31m: \x1b[33m");
//		ft_putstr(reason);
//	}
//	ft_putchar('\n');
//	exit(EXIT_FAILURE);
//}
//
//void					error(char *reason)
//{
//	ft_putstr("\x1b[31mError\x1b[33m");
//	if (reason)
//	{
//		ft_putstr("\x1b[31m: \x1b[33m");
//		ft_putstr(reason);
//	}
//	ft_putchar('\n');
//	ft_putendl(strerror(errno));
//	exit(EXIT_FAILURE);
//}

void					write_text(char *str, t_sdl *sdl, int max_w, int h, t_vec2d pos, int color)
{
	unsigned char		r;
	unsigned char		g;
	unsigned char		b;
	int					w;
	float 				ratio;

	get_rgb(&r, &g, &b, color);
	SDL_Color textColor = { r, g, b, 0 };
	SDL_Surface* textSurface = TTF_RenderText_Solid(sdl->font, str, textColor);
	SDL_Texture* text = SDL_CreateTextureFromSurface(sdl->rend, textSurface);
	ratio = textSurface->h ? (float)textSurface->w / textSurface->h : 1.f;
	w = clamp(textSurface->h * ratio, 0, max_w);
	SDL_FreeSurface(textSurface);
	SDL_Rect renderQuad = { pos.x, pos.y, w, h };
	SDL_RenderCopy(sdl->rend, text, NULL, &renderQuad);
	SDL_DestroyTexture(text);
}

void					editor_clamp(float *res_min_scale)
{
	static int 			min_size = -1;
	static float		min_scale = 1;

	if (min_size == -1)
	{
		if (WIN_H < WIN_W)
			min_size = WIN_H / 2;
		else
			min_size = WIN_W / 2;
		min_scale = (float)min_size / GRID_SIZE;
	}
	*res_min_scale = min_scale;
}

void					render_button(t_button *button, t_sdl *sdl)
{
	SDL_Rect			rect;
	SDL_Color			sdl_color;
	SDL_Surface			*surf;
	SDL_Texture			*txtr;
	unsigned char		r;
	unsigned char		g;
	unsigned char		b;

	if (!button || !sdl->rend)
		return ;
	if (button->background != -1)
	{
		int x = 0;
		int y = button->box.y;
		while (y < button->box.y + button->box.h)
		{
			x = button->box.x;
			while (x < button->box.x + button->box.w)
				draw_dot(x++, y, button->background, sdl->rend);
			y++;
		}
	}
	if (button->vis_lit_on[1] == FALSE && button->vis_lit_on[2] == FALSE && button->back)
	{
		rect = (SDL_Rect){ button->box.x, button->box.y,
						   button->box.w, button->box.h };
		SDL_RenderCopy(sdl->rend, button->back, NULL, &rect);
	}
	else if ((button->vis_lit_on[1] == TRUE || button->vis_lit_on[2] == TRUE) && button->lit_back)
	{
		rect = (SDL_Rect){ button->box.x, button->box.y,
						   button->box.w, button->box.h };
		SDL_RenderCopy(sdl->rend, button->lit_back, NULL, &rect);
	}
	if (button->front)
	{
		rect = (SDL_Rect){ button->box.x + button->box.w * 0.1, button->box.y + button->box.h * 0.1,
						   button->box.w * 0.8, button->box.h * 0.8 };
		SDL_RenderCopy(sdl->rend, button->front, NULL, &rect);
	}
	if (button->text)
	{
		rect = (SDL_Rect){ button->box.x + button->box.w * 0.05, button->box.y + button->box.h * 0.05,
						   button->box.w * 0.9, button->box.h * 0.8 };
		if (button->vis_lit_on[1] == TRUE)
			get_rgb(&r, &g, &b, LIT_COLOR);
		else
			get_rgb(&r, &g, &b, button->text_color);
		sdl_color = (SDL_Color){ r, g, b, 0 };
		surf = TTF_RenderText_Solid(sdl->font, button->text, sdl_color);
		txtr = SDL_CreateTextureFromSurface(sdl->rend, surf);
		SDL_FreeSurface(surf);
		SDL_RenderCopy(sdl->rend, txtr, NULL, &rect);
	}
}

void					render_buttons(t_button *buttons, t_sdl *sdl)
{
	short 				i;

	i = 0;

	while (i < sdl->modes[sdl->mode_id].n_buttons)
	{
		if (buttons[i].vis_lit_on[0] == TRUE)
			render_button(&buttons[i], sdl);
		i++;
	}

}

void					render_sector_menu(t_sdl *sdl, t_t *t, t_sector *sector)
{
	SDL_Texture			*back;
	SDL_Rect			rect;
	t_rec				box;
	int 				text_color = DARK_GRAY;

	if (!sdl || !t)
		return ;
	box.w = WIN_W * 0.35;
	box.h = WIN_H * 0.8;
	box.x = WIN_W * 0.6;
	box.y = WIN_H * 0.1;
	back = load_texture("grey_panel.png", sdl);
	rect = (SDL_Rect){ box.x, box.y,
					   box.w, box.h };
	SDL_RenderCopy(sdl->rend, back, NULL, &rect);

	write_text(ft_strjoin("Sector ", ft_itoa(t->active[0].x)), sdl, box.w, box.h * 0.07,
			(t_vec2d){ box.x + box.w * 0.05, box.y + box.h * 0.05 }, text_color);
	write_text(ft_strjoin("Floor height ", ft_itoa(sector->floor)), sdl, box.w, box.h * 0.07,
			   (t_vec2d){ box.x + box.w * 0.05, box.y + box.h * 0.15 }, text_color);
	write_text(ft_strjoin("Ceiling height ", ft_itoa(sector->ceiling)), sdl, box.w, box.h * 0.07,
			   (t_vec2d){ box.x + box.w * 0.05, box.y + box.h * 0.25 }, text_color);
	write_text(ft_strjoin("Floor texture ", ft_itoa(sector->floor_txtr)), sdl, box.w, box.h * 0.07,
			   (t_vec2d){ box.x + box.w * 0.05, box.y + box.h * 0.35 }, text_color);
	write_text(ft_strjoin("Ceiling texture ", ft_itoa(sector->ceil_txtr)), sdl, box.w, box.h * 0.07,
			   (t_vec2d){ box.x + box.w * 0.05, box.y + box.h * 0.45 }, text_color);
	write_text(ft_strjoin("Wall texture ", ft_itoa(sector->floor_txtr)), sdl, box.w, box.h * 0.07,
			   (t_vec2d){ box.x + box.w * 0.05, box.y + box.h * 0.55 }, text_color);

	sdl->modes[sdl->mode_id].buttons[DESELECT_SEC_BUTTON].box = (t_rec){ box.x + box.w, box.y, 30, 30 };
}

void					render_screen(t_sdl *sdl)
{
	int 				x;
	int					y;

	x = 0;
	while (x < WIN_W)
	{
		y = 0;
		while (y < WIN_H)
		{
			if (sdl->screen[x][y] != 0)
				draw_dot(x, y, sdl->screen[x][y], sdl->rend);
			y++;
		}
		x++;
	}
}

void					render_editor(t_sdl *sdl, t_t *t, t_media *media)
{
	if (!sdl || !media || !t || sdl->features[F_REDRAW] == 0)
		return ;
	SDL_SetRenderDrawColor(sdl->rend, 55, 55, 55, 255);

	SDL_RenderClear(sdl->rend);

	render_grid(media->worlds[media->world_id], t, sdl);
	render_screen(sdl);
	if (t->active[0].y >= 0 && t->active[0].y < media->worlds[media->world_id].n_sectors)
		render_sector_menu(sdl, t, &media->worlds[media->world_id].sectors[t->active[0].y]);
	render_buttons(sdl->modes[sdl->mode_id].buttons, sdl);
	SDL_RenderPresent(sdl->rend);
	sdl->features[F_REDRAW] = 0;
}

t_vec2d					find_node(int p_x, int p_y, t_t *t)
{
	float				mapx;
	float				mapy;

	mapx = (float)(p_x - t->grid.box.x) / t->grid.scale;
	mapy = (float)(p_y - t->grid.box.y) / t->grid.scale;
	return ((t_vec2d){ round(mapx), round(mapy) });
}

void					zoom_displace(int *gridx, int *gridy, t_vec2d p, float old_scale, float new_scale)
{
	float				mapx;
	float				mapy;

	mapx = (float)(p.x - *gridx) / old_scale;
	mapy = (float)(p.y - *gridy) / old_scale;
	*gridx = (int)(p.x - (mapx * new_scale));
	*gridy = (int)(p.y - (mapy * new_scale));
}

unsigned short			light_button(t_sdl *sdl)
{
	unsigned short		res;
	unsigned short		i;

	if (!sdl)
		return (FAIL);
	i = 0;
	res = FAIL;
	while (i < sdl->modes[sdl->mode_id].n_buttons)
	{
		if (mouse_over(sdl->modes[sdl->mode_id].buttons[i].box, sdl->mouse) == TRUE)
		{
			sdl->modes[sdl->mode_id].buttons[i].vis_lit_on[1] = TRUE;
			sdl->button_lit = i;
			sdl->features[F_REDRAW] = 1;
			res = SUCCESS;
		}
		else
		{
			if (sdl->modes[sdl->mode_id].buttons[i].vis_lit_on[1] == TRUE)
				sdl->features[F_REDRAW] = 1;
			sdl->modes[sdl->mode_id].buttons[i].vis_lit_on[1] = FALSE;
		}
		i++;
	}
	if (res == FAIL)
		sdl->button_lit = -1;
	return (res);
}

t_world					*realloc_worlds(t_world *world, int n)
{
	t_world			*new;
	int					j;

	j = 0;
	if (n == 0)
		return (NULL);
	if (!(new = (t_world *)malloc(sizeof(t_world) * n)))
		return (NULL);
	while (j < n - 1)
	{
		new[j] = world[j];
		j++;
	}
	free(world);
	return (new);
}

t_sector				*realloc_sectors(t_sector *sectors, int n)
{
	t_sector			*new;
	int					j;

	j = 0;
	if (n == 0)
		return (NULL);
	if (!(new = (t_sector *)malloc(sizeof(t_sector) * n)))
		return (NULL);
	while (j < n - 1)
	{
		new[j] = sectors[j];
		j++;
	}
	free(sectors);
	return (new);
}

int						*realloc_sector_v(int *v, int n)
{
	int					*new;
	int					j;

	j = 0;
	if (n == 0)
		return (NULL);
	if (!(new = (int *)malloc(sizeof(int) * n)))
		return (NULL);
	while (j < n - 1)
	{
		new[j] = v[j];
		j++;
	}
	if (n > 1 && v)
		free(v);
	return (new);
}

int						*realloc_secwalls(int *secwalls, int n)
{
	int					*new;
	int					j;

	j = 0;
	if (n == 0)
		return (NULL);
	if (!(new = (int *)malloc(sizeof(int) * n)))
		return (NULL);
	while (j < n - 1)
	{
		new[j] = secwalls[j];
		j++;
	}
	if (secwalls)
		free(secwalls);
	return (new);
}

t_wall					*realloc_walls(t_wall *walls, int n)
{
	t_wall				*new;
	int					j;

	j = 0;
	if (n == 0)
		return (NULL);
	if (!(new = (t_wall *)malloc(sizeof(t_wall) * n)))
		return (NULL);
	while (j < n - 1)
	{
		new[j] = walls[j];
		j++;
	}
	free(walls);
	return (new);
}

t_vec2d					*realloc_vertices(t_vec2d *vertices, int n)
{
	t_vec2d				*new;
	int					j;

	j = 0;
	if (n == 0)
		return (NULL);
	if (!(new = (t_vec2d *)malloc(sizeof(t_vec2d) * n)))
		return (NULL);
	while (j < n - 1)
	{
		new[j] = vertices[j];
		j++;
	}
	free(vertices);
	return (new);
}

short 					find_vector(t_vec2d *vertices, t_vec2d p, int n)
{
	int 				i;

	if (!vertices)
		return (-1);
	i = 0;
	while (i < n)
	{
		if (vertices[i].x == p.x && vertices[i].y == p.y)
			return (i);
		i++;
	}
	return (-1);
}

short 					find_wall(short one, short two, t_wall *walls, short n_walls)
{
	short 				i;

	i = 0;
	if (!walls)
		return (-1);
	while (i < n_walls)
	{
		if (one == walls[i].v1 && two == walls[i].v2)
			return (i);
		if (two == walls[i].v1 && one == walls[i].v2)
			return (i);
		i++;
	}
	return (-1);
}

unsigned short			add_vector(t_vec2d **vertices, short n_vectors, t_t *t, short i)
{
	if (!t)
		return (FAIL);
	*vertices = realloc_vertices(*vertices, n_vectors + 1);
	if (!*vertices)
		return (FAIL);
	(*vertices)[n_vectors] = t->active[i];
	t->grid.nodes[t->active[i].x][t->active[i].y] = NODE_FULL;
	return (SUCCESS);
}

unsigned short			add_sector_v(int **v, short n_v, int id)
{
	*v = realloc_sector_v(*v, n_v + 1);
	if (!*v)
		return (FAIL);
	(*v)[n_v] = id;
	return (SUCCESS);
}

unsigned short			add_sector(t_sector **sectors, short n_sectors)
{
	*sectors = realloc_sectors(*sectors, n_sectors + 1);
	if (!*sectors)
		return (FAIL);
	(*sectors)[n_sectors].sec_walls = NULL;
	(*sectors)[n_sectors].floor = 0;
	(*sectors)[n_sectors].ceiling = 0;
	(*sectors)[n_sectors].floor_txtr = 0;
	(*sectors)[n_sectors].ceil_txtr = 0;
	(*sectors)[n_sectors].n_walls = 0;
	return (SUCCESS);
}

unsigned short			add_wall(t_wall **walls, short n_walls, int one, int two)
{
	*walls = realloc_walls(*walls, n_walls + 1);
	if (!*walls)
		return (FAIL);
	(*walls)[n_walls].type = WALL_FILLED;
	(*walls)[n_walls].v1 = one;
	(*walls)[n_walls].v2 = two;
	return (SUCCESS);
}

unsigned short			add_secwall(int **secwalls, short n_secwalls, int wall)
{
	if (n_secwalls > 0 && !*secwalls)
		return (FAIL);
	*secwalls = realloc_secwalls(*secwalls, n_secwalls + 1);
	if (!*secwalls)
		return (FAIL);
	(*secwalls)[n_secwalls] = wall;
	return (SUCCESS);
}

unsigned short			add_world(t_world **worlds, short n_worlds)
{
	*worlds = realloc_worlds(*worlds, n_worlds + 1);
	if (!*worlds)
		return (FAIL);
	(*worlds)[n_worlds].filename = ft_strdup("F.txt");
	(*worlds)[n_worlds].full_path = ft_strdup("./media/maps/");
	(*worlds)[n_worlds].textures = NULL;
	(*worlds)[n_worlds].n_textures = 0;
	(*worlds)[n_worlds].sectors = NULL;
	(*worlds)[n_worlds].n_sectors = 0;
	(*worlds)[n_worlds].walls = NULL;
	(*worlds)[n_worlds].n_walls = 0;
	(*worlds)[n_worlds].vertices = NULL;
	(*worlds)[n_worlds].n_vectors = 0;
	return (SUCCESS);
}

void					add_to_media(t_t *t, t_media *media)
{
	static short 		first_vector = -1;
	static short 		sector = -1;
	static short 		last_id = -1;
	short 				wall;
	short 				id;
	short 				done;

	if (!media || !t)
		return ;
	id = -1;
	done = FALSE;
	wall = -1;
	if (t->active[1].x == -1)
	{
		if (t->grid.nodes[t->active[0].x][t->active[0].y] == NODE_FULL)
			id = find_vector(media->worlds[media->world_id].vertices, t->active[0], media->worlds[media->world_id].n_vectors);
		if (id == -1)
		{
			if (add_vector(&media->worlds[media->world_id].vertices, media->worlds[media->world_id].n_vectors, t, 0) == FAIL)
				return ;
			id = media->worlds[media->world_id].n_vectors++;
		}
		if (add_sector(&media->worlds[media->world_id].sectors, media->worlds[media->world_id].n_sectors) == FAIL)
			return ;
		sector = media->worlds[media->world_id].n_sectors++;
		if (add_sector_v(&media->worlds[media->world_id].sectors[sector].v, media->worlds[media->world_id].sectors[sector].n_walls, id) == FAIL)
			return ;
		last_id = id;
		first_vector = id;
	}
	else
	{
		if (t->grid.nodes[t->active[1].x][t->active[1].y] == NODE_FULL)
			id = find_vector(media->worlds[media->world_id].vertices, t->active[1], media->worlds[media->world_id].n_vectors);
		if (id == -1)
		{
			if (add_vector(&media->worlds[media->world_id].vertices, media->worlds[media->world_id].n_vectors, t, 1) == FAIL)
				return ;
			id = media->worlds[media->world_id].n_vectors++;
		}
		else
		{
			if (id == first_vector)
				done = TRUE;
			else if (t->grid.nodes[media->worlds[media->world_id].vertices[last_id].x][media->worlds[media->world_id].vertices[last_id].y] == NODE_FULL &&
				t->grid.nodes[media->worlds[media->world_id].vertices[id].x][media->worlds[media->world_id].vertices[id].y] == NODE_FULL)
				wall = find_wall(last_id, id, media->worlds[media->world_id].walls, media->worlds[media->world_id].n_walls);
		}
		if (wall == -1)
		{
			if (add_wall(&media->worlds[media->world_id].walls, media->worlds[media->world_id].n_walls, last_id, id) == FAIL)
				return ;
			wall = media->worlds[media->world_id].n_walls++;
		}
		if (add_secwall(&media->worlds[media->world_id].sectors[sector].sec_walls, media->worlds[media->world_id].sectors[sector].n_walls, wall) == FAIL)
			return ;
		media->worlds[media->world_id].sectors[sector].n_walls++;
		if (add_sector_v(&media->worlds[media->world_id].sectors[sector].v, media->worlds[media->world_id].sectors[sector].n_walls, id) == FAIL)
			return ;
		t->active[0] = t->active[1];
		t->active[1] = (t_vec2d){ -1, -1 };
		last_id = id;
	}
	if (done == TRUE)
	{
		t->active[0] = (t_vec2d){ -1, -1 };
		t->active[1] = (t_vec2d){ -1, -1 };
		last_id = -1;
	}
}

unsigned short 			sec_is_convex(t_vec2d *vertices, int *v, int n)
{
	char 			pos = 0;
	char 			neg = 0;
	if (n < 4)
		return (TRUE);

	for(int i = 0; i < n; i++)
	{
		double dx1 = vertices[v[(i + 2) % n]].x - vertices[v[(i + 1) % n]].x;
		double dy1 = vertices[v[(i + 2) % n]].y - vertices[v[(i + 1) % n]].y;
		double dx2 = vertices[v[i]].x - vertices[v[(i + 1) % n]].x;
		double dy2 = vertices[v[i]].y - vertices[v[(i + 1) % n]].y;
		double zcrossproduct = dx1 * dy2 - dy1 * dx2;

		if (zcrossproduct > 0)
			pos = 1;
		if (zcrossproduct < 0)
			neg = 1;

		if (pos && neg)
			return (FALSE);
	}

	return (TRUE);
}

char 					sector_status(t_sector sector, t_wall *walls, t_vec2d *vertices, int n)
{
	int					i;
	int					j;
	int 				tmp[n];

	if (!walls || !vertices)
		return (FAIL);
	ft_memset(tmp, -1, sizeof(int) * n);
	i = -1;
	j = 0;
	while (++i < sector.n_walls)
	{
		tmp[j++] = walls[sector.sec_walls[i]].v1;
		tmp[j++] = walls[sector.sec_walls[i]].v2;
	}
	pair_sort(tmp, n);
	if (sector_closed(tmp, n) == FALSE)
		return (SEC_OPEN);
	else if (sec_is_convex(vertices, sector.v, sector.n_walls) == FALSE)
		return(SEC_CONCAVE_CLOSED);
	return(SEC_CONVEX_CLOSED);
}

void					update_sector_status(t_sector *sectors, t_wall *walls, t_vec2d *vertices, int n_sectors)
{
	int 				i;

	i = 0;
	if (!sectors)
		return ;
	while (i < n_sectors)
	{
		sectors[i].status = sector_status(sectors[i], walls, vertices, sectors[i].n_walls * 2);
		i++;
	}
}

void					delete_vector(int id, t_world *world)
{
	t_vec2d				*new;
	int 				i;
	int 				j;

	if (!world || id < 0 || id >= world->n_vectors)
		return ;
	new = (t_vec2d *)ft_memalloc(sizeof(t_vec2d) * (world->n_vectors - 1));
	if (!new)
		return ;
	i = 0;
	j = 0;
	while (j < world->n_vectors)
	{
		if (j == id)
			j++;
		new[i] = world->vertices[j];
		i++;
		j++;
	}
	world->n_vectors--;
	free(world->vertices);
	world->vertices = new;
	i = 0;
	while (i < world->n_walls)
	{
		if (world->walls[i].v1 > id)
			world->walls[i].v1--;
		if (world->walls[i].v2 > id)
			world->walls[i].v2--;
		i++;
	}
	i = 0;
	while (i < world->n_sectors)
	{
		j = 0;
		while (j < world->sectors[i].n_walls)
		{
			if (world->sectors[i].v[j] > id)
				world->sectors[i].v[j]--;
			j++;
		}
		i++;
	}
}

void					fill_grid(int n_vectors, t_vec2d *vertices, t_grid *grid)
{
	int 				i;

	if (!vertices || !grid || n_vectors < 1)
		return ;
	i = 0;
	while (i < n_vectors)
	{
		grid->nodes[vertices[i].x][vertices[i].y] = NODE_FULL;
		i++;
	}
}

void					clean_grid(t_grid *grid)
{
	int					i;
	int					j;

	if (!grid)
		return ;
	i = 0;
	while (i < GRID_SIZE)
	{
		j = 0;
		while (j < GRID_SIZE)
			grid->nodes[i][j++] = NODE_EMPTY;
		i++;
	}
}

void					zoom_grid(t_sdl *sdl, t_grid *grid)
{
	float				new;
	static float		min_scale = 1;
	float 				old_scale;

	if (!sdl || !grid)
		return ;
	if (mouse_over(grid->box, sdl->mouse))
	{
		old_scale = grid->scale;
		if (min_scale == 1)
			editor_clamp(&min_scale);
		new = grid->scale + grid->scale * 0.3f * sdl->zoom;
		grid->scale = clamp_f(new, min_scale, 100);
		zoom_displace(&grid->box.x, &grid->box.y, sdl->mouse, old_scale, grid->scale );
		sdl->features[F_REDRAW] = 1;
	}
	sdl->zoom = 0;
}

void					move_grid(t_sdl *sdl, t_grid *grid)
{
	if (!sdl || !grid)
		return ;
	if (mouse_over(grid->box, sdl->mouse))
	{
		grid->box.x += sdl->mouse.x - sdl->move.x;
		grid->box.y += sdl->mouse.y - sdl->move.y;
		sdl->move = sdl->mouse;
		sdl->features[F_REDRAW] = 1;
	}
	else
		sdl->move = (t_vec2d){ 0, 0 };
}

void					move_vector(t_sdl *sdl, t_t *t, t_world *world)
{
	static int			id = -1;
	static t_vec2d		to_erase = { -1, -1 };

	if (!sdl || !t || !world)
		return ;
	if (sdl->move.x || sdl->move.y)
	{
		if (mouse_over(t->grid.box, sdl->mouse))
		{
			if (t->active[0].x == -1)
			{
				t->active[0] = find_node(sdl->mouse.x, sdl->mouse.y, t);
				if (t->active[0].x >= 0 && t->active[0].y >= 0 &&
				t->grid.nodes[t->active[0].x][t->active[0].y] == NODE_FULL)
					id = find_vector(world->vertices, t->active[0], world->n_vectors);
				if (id == -1)
					t->active[0] = (t_vec2d){ -1, -1 };
				to_erase = t->active[0];
			}
			else if (id >= 0)
			{
				if (to_erase.x != -1)
					t->grid.nodes[to_erase.x][to_erase.y] = NODE_EMPTY;
				t->active[1] = find_node(sdl->mouse.x, sdl->mouse.y, t);
				world->vertices[id] = t->active[1];
				t->grid.nodes[t->active[1].x][t->active[1].y] = NODE_FULL;
				to_erase = t->active[1];
			}
			sdl->move = sdl->mouse;
			sdl->features[F_REDRAW] = 1;
		}
		else
			sdl->move = (t_vec2d){ 0, 0 };
	}
	else
	{
		t->active[0] = (t_vec2d){ -1, -1 };
		id = -1;
	}
}

unsigned short			dot_inside_sector(int x, int y, t_vec2d *p, int n)
{
	int 				i;
	int 				j;
	unsigned short		odd;

	i = 0;
	j = n - 1;
	odd = FALSE;
	while(i < n)
	{
		if ((p[i].y < y && p[j].y >= y) || (p[j].y < y && p[i].y >= y) )
		{
			if (p[i].x + (y - p[i].y) / (p[j].y - p[i].y) * (p[j].x - p[i].x) < x)
				odd = odd == FALSE ? TRUE : FALSE;
		}
		j = i;
		i++;
	}
	return (odd);
}

t_vec2d					find_in_grid(int p_x, int p_y, t_grid grid)
{
	t_vec2d				res;

	res.x = (float)(p_x - grid.box.x) / grid.scale;
	res.y = (float)(p_y - grid.box.y) / grid.scale;
	return (res);
}

int 					in_sector(t_vec2d p, t_world *world, t_t *t)
{
	int 				id;
	t_vec2d				map_p;
	t_vec2d				*v;
	int 				i;
	int 				j;

	if (!world || !t || mouse_over(t->grid.box, p) == FALSE)
		return (-1);
	id = -1;
	map_p = find_in_grid(p.x, p.y, t->grid);
	i = 0;
	while (i < world->n_sectors)
	{
		if ( world->sectors[i].status != SEC_OPEN)
		{
			if (!(v = ft_memalloc(sizeof(t_vec2d) * world->sectors[i].n_walls)))
				return (-1);
			j = 0;
			while (j < world->sectors[i].n_walls)
			{
				v[j] = world->vertices[world->sectors[i].v[j]];
				j++;
			}
			if (dot_inside_sector(map_p.x, map_p.y, v, world->sectors[i].n_walls) == TRUE)
				id = i;
			free(v);
		}
		i++;
	}
	return (id);
}

void					update_editor(t_sdl *sdl, t_t *t, t_media *media)
{
	static short 		world_id = -1;
	static				int s = -1;

	if (!sdl || !media || !t)
		return;
	if (sdl->save == 1 || sdl->button_on == SAVE_BUTTON) // when saving
	{
		rewrite_media(media);
		sdl->save = 0;
		sdl->button_on = DRAG_BUTTON;
		sdl->modes[sdl->mode_id].buttons[sdl->button_on].vis_lit_on[2] = TRUE;
		sdl->modes[sdl->mode_id].buttons[SAVE_BUTTON].vis_lit_on[2] = FALSE;
		return ;
	}
	if (sdl->button_lit != -1 && (sdl->move.x || sdl->move.y)) // when pressing an on screen button
	{
		if (sdl->button_on != -1)
			sdl->modes[sdl->mode_id].buttons[sdl->button_on].vis_lit_on[2] = FALSE;
		sdl->button_on = sdl->button_lit;
		sdl->modes[sdl->mode_id].buttons[sdl->button_on].vis_lit_on[2] = TRUE;
		t->active[0] = (t_vec2d){ -1, -1 };
		t->active[1] = (t_vec2d){ -1, -1 };
		return ;
	}
	if (light_button(sdl) == SUCCESS) // when mouse is over a button
		return ;
	if (world_id != media->world_id) // when opening a map
	{
		world_id = media->world_id;
		clean_grid(&t->grid);
		if (media->world_id >= media->n_worlds && add_world(&media->worlds, media->n_worlds) == SUCCESS) // if ADD WORLD chosen
			media->n_worlds++;
		fill_grid(media->worlds[world_id].n_vectors, media->worlds[world_id].vertices, &t->grid);
		sdl->features[F_REDRAW] = 1;
		update_sector_status(media->worlds[world_id].sectors, media->worlds[world_id].walls, media->worlds[world_id].vertices, media->worlds[world_id].n_sectors);
		return ;
	}
	if (sdl->button_on == DRAW_BUTTON) // draw mode
	{
		if ((sdl->move.x || sdl->move.y) && mouse_over(t->grid.box, sdl->mouse))
		{
			if (t->active[0].x == -1)
				t->active[0] = find_node(sdl->mouse.x, sdl->mouse.y, t);
			else if (t->active[1].x == -1)
				t->active[1] = find_node(sdl->mouse.x, sdl->mouse.y, t);
			if (t->active[0].x != -1)
				add_to_media(t, media);
			sdl->features[F_REDRAW] = 1;
			sdl->move.x = 0;
			sdl->move.y = 0;
		}
		else if (t->active[0].x != -1 && t->active[1].x == -1)
			sdl->features[F_REDRAW] = 1;
	}
	else if (sdl->button_on == DISTORT_BUTTON ) // move mode
		move_vector(sdl, t, &media->worlds[world_id]);
	else if (sdl->button_on == DRAG_BUTTON) // view mode
	{
		if (sdl->move.x || sdl->move.y)
			move_grid(sdl, &t->grid);
	}
	else if (sdl->button_on == DELETE_BUTTON) // delete mode
	{
		if (sdl->move.x || sdl->move.y)
		{
			if (mouse_over(t->grid.box, sdl->mouse))
			{
				int m = -1;
				t->active[0] = find_node(sdl->mouse.x, sdl->mouse.y, t);
				if (t->grid.nodes[t->active[0].x][t->active[0].y] == NODE_FULL)
					m = find_vector(media->worlds[world_id].vertices, t->active[0], media->worlds[world_id].n_vectors);
				delete_vector(m, &media->worlds[world_id]);
				t->grid.nodes[t->active[0].x][t->active[0].y] = NODE_EMPTY;
				sdl->move = (t_vec2d){ 0, 0 };
				sdl->features[F_REDRAW] = 1;
			}
			else
				sdl->move = (t_vec2d){ 0, 0 };
		}
	}
	else if (sdl->button_on == SECTOR_BUTTON && s == -1) // sector mode
	{
		int k;
		t->active[0] = (t_vec2d){ -1, -1 };
		k = in_sector(sdl->mouse, &media->worlds[world_id], t);
		if (k != -1)
		{
			t->active[0].x = k;
			if (sdl->move.x || sdl->move.y)
			{
				s = k;
				t->active[0].y = s;
			}
		}
		sdl->features[F_REDRAW] = 1;

	}
	else if (sdl->button_on == DESELECT_SEC_BUTTON && s != -1) // sector mode
	{
		s = -1;
		sdl->features[F_REDRAW] = 1;
		sdl->modes[sdl->mode_id].buttons[sdl->button_on].vis_lit_on[2] = FALSE;
		sdl->button_on = SECTOR_BUTTON;
		sdl->modes[sdl->mode_id].buttons[sdl->button_on].vis_lit_on[2] = TRUE;

	}
	sdl->modes[sdl->mode_id].buttons[7].vis_lit_on[0] = (sdl->button_on == SECTOR_BUTTON && s != -1) ? TRUE : FALSE;
	if (sdl->zoom != 0)
		zoom_grid(sdl, &t->grid);
	update_sector_status(media->worlds[world_id].sectors, media->worlds[world_id].walls, media->worlds[world_id].vertices, media->worlds[world_id].n_sectors);
}

int						input_editor(t_sdl *sdl, float *grid_scale, t_media *media)
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
				sdl->save = 1;
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
				sdl->zoom++;
			else if(event.wheel.y < 0) // scroll down
				sdl->zoom--;
		}
		if( event.type == SDL_MOUSEMOTION || event.type == SDL_MOUSEBUTTONDOWN || event.type == SDL_MOUSEBUTTONUP )
		{
			if(event.type == SDL_MOUSEBUTTONDOWN)
			{
				if (sdl->button_lit == BACK_BUTTON)
				{
					sdl->mode_id = MODE_SUMMARY;
					sdl->button_lit = -1;
					sdl->button_on = -1;
					media->world_id = -1;
					return (quit);
				}
				sdl->move.x = sdl->mouse.x;
				sdl->move.y = sdl->mouse.y;
			}
			if(event.type == SDL_MOUSEBUTTONUP)
			{
				sdl->move.x = 0;
				sdl->move.y = 0;
			}
		}
	}
	return (quit);
}

void					game_loop(t_sdl *sdl, t_media *media)
{
	if (!sdl || !media)
		return ;
	t_t					*t;

	if (!(t = ft_memalloc(sizeof(t_t))))
		return ;

	t->grid.scale = WIN_H / GRID_SIZE;
	t->grid.box.w = GRID_SIZE * t->grid.scale;
	t->grid.box.h = GRID_SIZE * t->grid.scale;
	t->grid.box.x = (WIN_W - t->grid.box.w) / 2;
	t->grid.box.y = (WIN_H - t->grid.box.h) / 2;


	sdl->move = (t_vec2d){ 0, 0 };
	sdl->button_on = 0;

	sdl->features[F_ZOOM] = 1;
	sdl->features[F_MOVE_GRID] = 1;
	sdl->features[F_SELECT_NODE] = 0;
	sdl->features[F_REDRAW] = 1;

	t->active[0] = (t_vec2d){ -1, -1 };
	t->active[1] = (t_vec2d){ -1, -1 };


	sdl->screen = (int **)ft_memalloc(sizeof(int *) * WIN_W);
	if (!sdl->screen)
		return;
	int k = 0;
	while (k < WIN_W)
	{
		sdl->screen[k] = (int *)ft_memalloc(sizeof(int) * WIN_H);
		if (!sdl->screen[k])
			return;
		ft_bzero(sdl->screen[k], sizeof(sdl->screen[k]));
		k++;
	}

	sdl->save = 0;
	clean_grid(&t->grid);
	while (sdl->modes[sdl->mode_id].input(sdl, &t->grid.scale, media) == FALSE)
	{
		sdl->modes[sdl->mode_id].update(sdl, t,  media);
		sdl->modes[sdl->mode_id].render(sdl, t, media);
	}
}

unsigned 				load_sdl_media(t_media *media, t_sdl *sdl)
{
	unsigned 			i;

	if (!media || !sdl)
		return (FAIL);
	i = 0;
	while (i < media->n_textures)
	{
		media->txtrs[i].sdl_t = load_texture(media->txtrs[i].full_path, sdl);
		if (!media->txtrs[i].sdl_t)
			return (FAIL);
		i++;
	}
	return (SUCCESS);
}

int						main(void)
{
	t_sdl				*sdl;
	t_media				*media;


	if (!(sdl = get_sdl()))
	{
		ft_putstr("\x1b[32mSdl is NULL, Returning fail from main function.\x1b[0m\n");
		return (FAIL);
	}
	if (!(media = get_assets()) || load_sdl_media(media, sdl) == FAIL || init_modes(sdl, media) == FAIL)
	{
		ft_putstr("\x1b[32mMedia is NULL, Returning fail from main function.\x1b[0m\n");
		return (FAIL);
	}
	game_loop(sdl, media);
	rewrite_media(media);
	free_media(media);
	free_sdl(sdl);
	ft_putstr("\x1b[32mReturning success from main function.\x1b[0m\n");
	return (SUCCESS);
}
