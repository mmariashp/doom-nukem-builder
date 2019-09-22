
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

void					write_text(char *str, SDL_Renderer *rend, t_rec rec, int color, char h_center)
{
	unsigned char		r;
	unsigned char		g;
	unsigned char		b;
	int					w;
	float 				ratio;
    SDL_Rect            renderQuad;
	SDL_Color			textColor;
    TTF_Font			*font;

    if (!str || !(font = set_get_free_font(1)))
		return ;
	get_rgb(&r, &g, &b, color);
	textColor = (SDL_Color){ r, g, b, 0 };
	SDL_Surface* textSurface = TTF_RenderText_Solid(font, str, textColor);
	SDL_Texture* text = SDL_CreateTextureFromSurface(rend, textSurface);
	ratio = textSurface->h ? (float)textSurface->w / textSurface->h : 1.f;
	w = clamp(textSurface->h * ratio, 0, rec.w);
	SDL_FreeSurface(textSurface);
    renderQuad = (SDL_Rect){ rec.x, rec.y, w, rec.h };
	if (h_center == TRUE)
        renderQuad.x = rec.x + (rec.w - w) / 2;
	else
        renderQuad.x = rec.x + rec.w * 0.05;
	SDL_RenderCopy(rend, text, NULL, &renderQuad);
	SDL_DestroyTexture(text);
}

void					render_box(t_rec box, SDL_Texture *t, SDL_Renderer *rend)
{
	SDL_Rect			rect;

	if (!t || !rend || within(box.x, -1, WIN_W) == FALSE ||
	within(box.y, -1, WIN_H) == FALSE || box.w < 1 || box.y < 1)
		return ;
	rect = (SDL_Rect){ box.x, box.y, box.w, box.h };
	SDL_RenderCopy(rend, t, NULL, &rect);
}

void					render_screen(SDL_Renderer *rend, int **screen)
{
	int 				x;
	int					y;

	x = 0;
	while (x < WIN_W)
	{
		y = 0;
		while (y < WIN_H)
		{
			if (screen[x][y] != 0)
			    draw_dot(x, y, screen[x][y], rend);
			y++;
		}
		x++;
	}
}

void					render_screen_iso(SDL_Renderer *rend, int **screen)
{
    int 				x;
    int					y;
    t_vec2d p;

    x = 0;
    while (x < WIN_W)
    {
        y = 0;
        while (y < WIN_H)
        {
            if (screen[x][y] != 0)
            {
                p = make_iso(x, y, 0);
                draw_dot(p.x, p.y, screen[x][y], rend);
            }
            y++;
        }
        x++;
    }
}

char 					sector_status(t_sector sector, t_wall *walls, t_vec2d *vecs, int n)
{
	int					i;
	int					j;
	int 				tmp[n];
	int                 *w;
	char                status;

	if (!walls || !vecs)
		return (SEC_OPEN);
	ft_memset(tmp, -1, sizeof(int) * n);
	i = -1;
	j = 0;
	while (++i < sector.n_walls)
	{
	    if (walls[sector.sec_walls[i]].type != WALL_DOOR)
        {
            tmp[j++] = walls[sector.sec_walls[i]].v1;
            tmp[j++] = walls[sector.sec_walls[i]].v2;
        }
	}
    i = -1;
    j = 0;
    while (++i < n)
    {
        if (tmp[i] >= 0)
            tmp[j++] = tmp[i];
    }
    if (!(w = (int *)ft_memalloc(sizeof(int) * j)))
        return (SEC_OPEN);
    i = 0;
    while (i < j && i < n)
    {
        w[i] = tmp[i];
        i++;
    }
	pair_sort(w, j);
    if (sector_closed(w, j) == FALSE)
        status = SEC_OPEN;
	else if (sec_is_convex(vecs, sector.v, sector.n_v) == FALSE)
	    status = SEC_CONCAVE_CLOSED;
	else
	    status = SEC_CONVEX_CLOSED;
    free(w);
	return(status);
}

void					update_sector_status(t_sector *sec, t_wall *walls, t_vec2d *vecs, int n_sec)
{
	int 				i;

	i = 0;
	if (!sec)
		return ;
	while (i < n_sec)
	{
		sec[i].status = sector_status(sec[i], walls, vecs, sec[i].n_walls * 2);
		i++;
	}
}

void					delete_vector(int id, t_world *world)
{
	t_vec2d				*new;
	int 				i;
	int 				j;

	if (!world || id < 0 || id >= world->n_vecs)
		return ;
	new = (t_vec2d *)ft_memalloc(sizeof(t_vec2d) * (world->n_vecs - 1));
	if (!new)
		return ;
	i = 0;
	j = 0;
	while (j < world->n_vecs)
	{
		if (j == id)
			j++;
		if (j < world->n_vecs)
			new[i] = world->vecs[j];
		i++;
		j++;
	}
	world->n_vecs--;
	free(world->vecs);
	world->vecs = new;
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
	while (i < world->n_sec)
	{
		j = 0;
		while (j < world->sec[i].n_walls)
		{
			if (world->sec[i].v[j] > id)
				world->sec[i].v[j]--;
			j++;
		}
		i++;
	}
}

void					delete_sector(int id, t_world *world)
{
	t_sector			*new;
	int 				i;
	int 				j;

	if (!world || id < 0 || id >= world->n_sec)
		return ;
	new = (t_sector *)ft_memalloc(sizeof(t_sector) * (world->n_sec - 1));
	if (!new)
		return ;
	i = 0;
	j = 0;
	while (j < world->n_sec)
	{
		if (j == id)
		{
			if (world->sec[j].sec_walls)
				free(world->sec[j].sec_walls);
			if (world->sec[j].v)
				free(world->sec[j].v);
			if (world->sec[j].items)
				free(world->sec[j].items);
			j++;
		}
		if (j < world->n_sec)
		{
			new[i].sec_walls = ft_memalloc(sizeof(int) *  world->sec[j].n_walls);
			if (new[i].sec_walls)
				new[i].sec_walls = ft_memcpy(new[i].sec_walls, world->sec[j].sec_walls, sizeof(int) *  world->sec[j].n_walls);
			new[i].v = ft_memalloc(sizeof(int) *  world->sec[j].n_v);
			if (new[i].v)
				new[i].v = ft_memcpy(new[i].v, world->sec[j].v, sizeof(int) *  world->sec[j].n_v);
			new[i].items = ft_memalloc(sizeof(t_item *) *  world->sec[j].n_items);
			if (new[i].items)
				new[i].items = ft_memcpy(new[i].items, world->sec[j].items, sizeof(t_item) *  world->sec[j].n_items);
			new[i].floor      = world->sec[j].floor;
			new[i].ceiling    = world->sec[j].ceiling;
			new[i].floor_txtr = world->sec[j].floor_txtr;
			new[i].ceil_txtr  = world->sec[j].ceil_txtr;
			new[i].n_items    = world->sec[j].n_items;
			new[i].n_walls    = world->sec[j].n_walls;
			new[i].n_v        = world->sec[j].n_v;
			new[i].status     = world->sec[j].status;

			if (world->sec[j].sec_walls)
				free(world->sec[j].sec_walls);
			if (world->sec[j].v)
				free(world->sec[j].v);
			if (world->sec[j].items)
				free(world->sec[j].items);
		}
		i++;
		j++;
	}
	if (world->sec)
		free(world->sec);
	world->sec = new;
	world->n_sec--;
}

void					move_vector(t_prog *prog, t_vec2d mouse, t_grid *grid, t_world *world)
{
	static int			id = -1;
	static t_vec2d		to_erase = { -1, -1 };

	if (!prog || !grid || !world)
		return ;
	if (prog->click.x || prog->click.y)
	{
		if (mouse_over(grid->box, mouse))
		{
			if (grid->active[0].x == -1)
			{
				grid->active[0] = find_node(mouse.x, mouse.y, grid);
				if (grid->active[0].x >= 0 && grid->active[0].y >= 0 &&
				grid->nodes[grid->active[0].x][grid->active[0].y] == NODE_FULL)
					id = find_vector(world->vecs, grid->active[0], world->n_vecs);
				if (id == -1)
				{
					grid->active[0] = (t_vec2d){ -1, -1 };
					move_grid_drag(prog, mouse, grid);
				}
				to_erase = grid->active[0];
			}
			else if (id >= 0)
			{

				grid->active[1] = find_node(mouse.x, mouse.y, grid);
				if (grid->nodes[grid->active[1].x][grid->active[1].y] != NODE_FULL)
				{
					if (to_erase.x != -1)
						grid->nodes[to_erase.x][to_erase.y] = NODE_EMPTY;
					world->vecs[id] = grid->active[1];
					grid->nodes[grid->active[1].x][grid->active[1].y] = NODE_FULL;
					to_erase = grid->active[1];
				}

			}
			prog->click = mouse;
			prog->features[F_REDRAW] = 1;
		}
		else
			prog->click = (t_vec2d){ 0, 0 };
	}
	else
	{
		grid->active[0] = (t_vec2d){ -1, -1 };
		id = -1;
	}
}

unsigned short          vec_same(t_vec2d one, t_vec2d two)
{
	if (one.x == two.x && one.y == two.y)
		return (TRUE);
	return (FALSE);
}

void					move_player(t_prog *prog, t_vec2d mouse, t_grid *grid, t_world *world)
{
	static int			id = -1;

	if (!prog || !grid || !world)
		return ;
	if (prog->click.x || prog->click.y)
	{
		if (mouse_over(grid->box, mouse))
		{
			if (grid->active[0].x == -1)
			{
				grid->active[0] = find_node(mouse.x, mouse.y, grid);
				if (vec_same(grid->active[0], world->p_start))
					id = 0;
				else if (vec_same(grid->active[0], world->p_end))
					id = 1;
				else
				{
					grid->active[0] = (t_vec2d){ -1, -1};
					move_grid_drag(prog, mouse, grid);
				}
			}
			else if (id >= 0)
			{
				grid->active[1] = find_node(mouse.x, mouse.y, grid);
				if (grid->nodes[grid->active[1].x][grid->active[1].y] == NODE_EMPTY)
				{
					if (id == 0)
						world->p_start = grid->active[1];
					else
						world->p_end = grid->active[1];
				}
			}
			prog->click = mouse;
			prog->features[F_REDRAW] = 1;
		}
		else
			prog->click = (t_vec2d){ 0, 0 };
	}
	else
	{
		grid->active[0] = (t_vec2d){ -1, -1 };
		id = -1;
	}
}

t_vec2d					find_node(int p_x, int p_y, t_grid *grid)
{
	float				mapx;
	float				mapy;

	mapx = (float)(p_x - grid->box.x) / grid->scale;
	mapy = (float)(p_y - grid->box.y) / grid->scale;
	return ((t_vec2d){ round(mapx), round(mapy) });
}

void					game_loop(t_sdl *sdl, t_media *media)
{
	t_grid				*grid;
	t_prog				*prog;

	prog = NULL;
    grid = NULL;
	set_get_free_font(0);
	if (!(prog = get_prog()) || init_modes(sdl, media, prog) == FAIL || !(grid = get_grid()))
	{
		ft_putstr("\x1b[32mReturning fail from game loop.\x1b[0m\n");
		return ;
	}
	while (prog->modes[prog->mode_id].input(sdl, grid, media, prog) == FALSE)
	{
		if (prog->modes[prog->mode_id].update(sdl, grid,  media, prog) == FAIL)
			return ;
		prog->modes[prog->mode_id].render(sdl, grid, media, prog);
		SDL_Delay(10);
	}
	free(grid);
	free_prog(prog, sdl);
	set_get_free_font(2);
	draw_items_or_free(1, 0, (t_rec){ 100, 100, 100, 100 }, sdl->rend);
}

unsigned 				load_sdl_media(t_media *media, t_sdl *sdl)
{
	unsigned 			i;

	if (!media || !sdl)
		return (FAIL);
	i = 0;
	while (i < media->n_txtrs)
	{
		media->txtrs[i].sdl_t = load_texture(media->txtrs[i].full_path, sdl->rend, &media->txtrs[i].size);
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

	if (!(media = get_assets()))
	{
		ft_putstr("\x1b[32mMedia is NULL, Returning fail from main function.\x1b[0m\n");
		return (FAIL);
	}
	if (!(sdl = get_sdl()))
	{
		ft_putstr("\x1b[32mSdl is NULL, Returning fail from main function.\x1b[0m\n");
		free_media(media);
		return (FAIL);
	}
	if (load_sdl_media(media, sdl) == FAIL)
	{
		ft_putstr("\x1b[32mCouldn't load sdl media, Returning fail from main function.\x1b[0m\n");
		free_media(media);
		free_sdl(sdl);
		return (FAIL);
	}
	game_loop(sdl, media);
	rewrite_media(media);
	free_media(media);
	free_sdl(sdl);
	ft_putstr("\x1b[32mReturning success from main function.\x1b[0m\n");
//	system("leaks -q builder");
	return (SUCCESS);
}
