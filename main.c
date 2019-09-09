
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

void					render_sector_menu(t_sdl *sdl, t_t *t, t_sector *sector)
{
	SDL_Texture	*back;
	SDL_Texture	*title;
	SDL_Rect			rect;
	t_rec				box;
	t_rec				title_box;
	int 				text_color = DARK_GRAY;

	if (!sdl || !t)
		return ;
	back = button_back(2, 1, sdl);
	title = button_back(0, 1, sdl);
	box.w = WIN_W * 0.35;
	box.h = box.w;
	box.x = WIN_W * 0.6;
	box.y = WIN_H * 0.1;

	title_box.w = box.w * 0.9;
	title_box.h = box.h * 0.1;
	title_box.x = box.x + (box.w - title_box.w) / 2;
	title_box.y = box.y + 20;

	rect = (SDL_Rect){ box.x, box.y,
					   box.w, box.h };
	SDL_RenderCopy(sdl->rend, back, NULL, &rect);
	rect = (SDL_Rect){ title_box.x, title_box.y,
					   title_box.w, title_box.h };
	SDL_RenderCopy(sdl->rend, title, NULL, &rect);

	char *str;
	str = ft_strjoin("SECTOR ", ft_itoa(t->active[0].x));
	write_text(str, sdl, box.w, box.h * 0.07,
			(t_vec2d){ box.x + box.w * 0.08, box.y + box.h * 0.05 }, text_color);
	if (str)
	    free(str);
    str = ft_strjoin("Floor height ", ft_itoa(sector->floor));
	write_text(str, sdl, box.w, box.h * 0.07,
			   (t_vec2d){ box.x + box.w * 0.05, box.y + box.h * 0.15 }, text_color);
    if (str)
        free(str);
    str = ft_strjoin("Ceiling height ", ft_itoa(sector->ceiling));
	write_text(str, sdl, box.w, box.h * 0.07,
			   (t_vec2d){ box.x + box.w * 0.05, box.y + box.h * 0.25 }, text_color);
    if (str)
        free(str);
    str = ft_strjoin("Floor texture ", ft_itoa(sector->floor_txtr));
	write_text(str, sdl, box.w, box.h * 0.07,
			   (t_vec2d){ box.x + box.w * 0.05, box.y + box.h * 0.35 }, text_color);
    if (str)
        free(str);
    str = ft_strjoin("Ceiling texture ", ft_itoa(sector->ceil_txtr));
	write_text(str, sdl, box.w, box.h * 0.07,
			   (t_vec2d){ box.x + box.w * 0.05, box.y + box.h * 0.45 }, text_color);
    if (str)
        free(str);
    str = ft_strjoin("Wall texture ", ft_itoa(sector->floor_txtr));
	write_text(str, sdl, box.w, box.h * 0.07,
			   (t_vec2d){ box.x + box.w * 0.05, box.y + box.h * 0.55 }, text_color);
    if (str)
        free(str);
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

t_vec2d					find_node(int p_x, int p_y, t_t *t)
{
	float				mapx;
	float				mapy;

	mapx = (float)(p_x - t->grid.box.x) / t->grid.scale;
	mapy = (float)(p_y - t->grid.box.y) / t->grid.scale;
	return ((t_vec2d){ round(mapx), round(mapy) });
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
    int					k;
	int					j;
	int 				tmp[n];

	if (!walls || !vertices)
		return (FAIL);
	ft_memset(tmp, -1, sizeof(int) * n);
	i = 0;
	j = 0;
	k = 0;
	while (k < sector.n_v && i < sector.n_walls)
	{
	    if (walls[sector.sec_walls[i]].type != WALL_DOOR)
        {
	        k++;
            tmp[j++] = walls[sector.sec_walls[i]].v1;
            tmp[j++] = walls[sector.sec_walls[i]].v2;
        }
        i++;
	}
	pair_sort(tmp, n);
	if (sector_closed(tmp, n) == FALSE)
		return (SEC_OPEN);
	else if (sec_is_convex(vertices, sector.v, sector.n_v) == FALSE)
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
		sectors[i].status = sector_status(sectors[i], walls, vertices, sectors[i].n_v * 2);
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

void					move_grid(t_prog *prog, t_vec2d mouse, t_grid *grid)
{
	if (!prog || !grid)
		return ;
	if (mouse_over(grid->box, mouse))
	{
		grid->box.x += mouse.x - prog->move.x;
		grid->box.y += mouse.y - prog->move.y;
		prog->move = mouse;
		prog->features[F_REDRAW] = 1;
	}
	else
		prog->move = (t_vec2d){ 0, 0 };
}

void					move_vector(t_prog *prog, t_vec2d mouse, t_t *t, t_world *world)
{
	static int			id = -1;
	static t_vec2d		to_erase = { -1, -1 };

	if (!prog || !t || !world)
		return ;
	if (prog->move.x || prog->move.y)
	{
		if (mouse_over(t->grid.box, mouse))
		{
			if (t->active[0].x == -1)
			{
				t->active[0] = find_node(mouse.x, mouse.y, t);
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
				t->active[1] = find_node(mouse.x, mouse.y, t);
				world->vertices[id] = t->active[1];
				t->grid.nodes[t->active[1].x][t->active[1].y] = NODE_FULL;
				to_erase = t->active[1];
			}
			prog->move = mouse;
			prog->features[F_REDRAW] = 1;
		}
		else
			prog->move = (t_vec2d){ 0, 0 };
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

void					game_loop(t_sdl *sdl, t_media *media)
{
	if (!sdl || !media)
		return ;
	t_t					*t;
	t_prog				*prog;

	prog = NULL;
	if (!(t = ft_memalloc(sizeof(t_t))))
		return ;
	if (!(prog = get_prog()) ||  init_modes(sdl, media, prog) == FAIL)
	{
		ft_putstr("\x1b[32mProg is NULL, Returning fail from main function.\x1b[0m\n");
		return ;
	}
	t->grid.scale = WIN_H / GRID_SIZE;
	t->grid.box.w = GRID_SIZE * t->grid.scale;
	t->grid.box.h = GRID_SIZE * t->grid.scale;
	t->grid.box.x = (WIN_W - t->grid.box.w) / 2;
	t->grid.box.y = (WIN_H - t->grid.box.h) / 2;
	t->active[0] = (t_vec2d){ -1, -1 };
	t->active[1] = (t_vec2d){ -1, -1 };
	clean_grid(&t->grid);
	while (prog->modes[prog->mode_id].input(sdl, &t->grid.scale, media, prog) == FALSE)
	{
		prog->modes[prog->mode_id].update(sdl, t,  media, prog);
		prog->modes[prog->mode_id].render(sdl, t, media, prog);
	}
	free(t);
	free_prog(prog, sdl);
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
	if (!(media = get_assets()))
	{
		ft_putstr("\x1b[32mMedia is NULL, Returning fail from main function.\x1b[0m\n");
		free_media(media);
		free_sdl(sdl);
		return (FAIL);
	}
	game_loop(sdl, media);
//	rewrite_media(media);
	free_media(media);
	free_sdl(sdl);
	ft_putstr("\x1b[32mReturning success from main function.\x1b[0m\n");
//	system("leaks -q builder");
	return (SUCCESS);
}
