
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

void					write_text(char *str, t_sdl *sdl, t_rec rec, int color, char h_center)
{
	unsigned char		r;
	unsigned char		g;
	unsigned char		b;
	int					w;
	float 				ratio;
    SDL_Rect            renderQuad;

	get_rgb(&r, &g, &b, color);
	SDL_Color textColor = { r, g, b, 0 };
	SDL_Surface* textSurface = TTF_RenderText_Solid(sdl->font, str, textColor);
	SDL_Texture* text = SDL_CreateTextureFromSurface(sdl->rend, textSurface);
	ratio = textSurface->h ? (float)textSurface->w / textSurface->h : 1.f;
	w = clamp(textSurface->h * ratio, 0, rec.w);
	SDL_FreeSurface(textSurface);
    renderQuad = (SDL_Rect){ rec.x, rec.y, w, rec.h };
	if (h_center == TRUE)
        renderQuad.x = rec.x + (rec.w - w) / 2;
	else
        renderQuad.x = rec.x + rec.w * 0.05;
	SDL_RenderCopy(sdl->rend, text, NULL, &renderQuad);
	SDL_DestroyTexture(text);
}


t_rec                    sector_menu(char i, char n)
{
    static t_rec        outer =  { .x = WIN_W * 0.6,  .y = WIN_H * 0.1  , .w = WIN_W * 0.34, .h = WIN_W * 0.34 };
    static t_rec        title = { .x = WIN_W * 0.62, .y = WIN_H * 0.12 , .w = WIN_W * 0.3,  .h = WIN_H * 0.05 };
    static t_rec        line =  { .x = WIN_W * 0.62, .y = WIN_H * 0.2  , .w = WIN_W * 0.24, .h = WIN_H * 0.03 };
    static t_rec        value = { .x = WIN_W * 0.87, .y = WIN_H * 0.2  , .w = WIN_W * 0.04, .h = WIN_H * 0.03 };


    if (i == 0)
        return (outer);
    if (i == 1)
        return (title);
    if (i == 2) //lines like height
        return ((t_rec){ line.x, line.y + n * (line.h + 20), line.w, line.h });
    if (i == 3) //arrows up
        return ((t_rec){ line.x + title.w * 0.95, (line.y + n * (line.h + 20)) - 10, line.h * 0.9, line.h * 0.9 });
    if (i == 4) // arrows down
        return ((t_rec){ line.x + title.w * 0.95, (line.y + n * (line.h + 20)) + 10, line.h * 0.9, line.h * 0.9 });
    if (i == 5) // values column
        return ((t_rec){ value.x, (value.y + n * (value.h + 20)), value.w, value.h });
    if (i == 6) // values edit
        return ((t_rec){ line.x + title.w * 0.95, (value.y + n * (value.h + 20)), line.h * 0.9, line.h * 0.9 });
    return (line);
}

void					render_sector_menu(t_sdl *sdl, t_grid *grid, t_sector *sector, t_media *media)
{
	SDL_Texture         *back;
	SDL_Texture         *title;
	SDL_Rect			rect;
	t_rec				box;
	t_rec				title_box;
	int 				text_color = DARK_GRAY;
    char                *str;
    int                 i = 0;
	int                 n = 4;
	static char         line[4][20] = { "Floor height ", "Ceiling height ", "Floor texture ", "Ceiling texture " };
    int                 value[4] = { sector->floor, sector->ceiling, sector->floor_txtr, sector->ceil_txtr};

	if (!sdl || !grid || !sector || !media->txtrs || media->n_textures < 0)
		return ;
	back = button_back(2, 1, sdl);
	title = button_back(0, 1, sdl);
	box = sector_menu(0, 0);
    title_box = sector_menu(1, 0);

	rect = (SDL_Rect){ box.x, box.y, box.w, box.h };
	SDL_RenderCopy(sdl->rend, back, NULL, &rect);
    rect = (SDL_Rect){ title_box.x, title_box.y, title_box.w, title_box.h };
	SDL_RenderCopy(sdl->rend, title, NULL, &rect);

	str = ft_strjoin("SECTOR ", ft_itoa(grid->active[0].x));
	write_text(str, sdl, title_box, text_color, FALSE);
	if (str)
	    free(str);
    title = button_back(1, 1, sdl);
    title_box = sector_menu(2, i);
    rect = (SDL_Rect){ title_box.x, title_box.y, title_box.w, title_box.h };
	while (i < n)
    {
        title_box = sector_menu(2, i);
        rect.y = title_box.y;
        SDL_RenderCopy(sdl->rend, title, NULL, &rect);
        write_text(line[i], sdl, title_box, text_color, FALSE);
        title_box = sector_menu(5, i);
        write_text(ft_itoa(value[i]), sdl, title_box, text_color, FALSE);
        if (i > 1 && media->txtrs[value[i]].sdl_t && value[i] >= 0 && value[i] < media->n_textures)
        {
            int k;
            k = media->worlds[media->world_id].textures[value[i]];
            if (k >= 0 && k < media->n_textures)
            {
                SDL_Rect rect2 = (SDL_Rect){ title_box.x, title_box.y, title_box.h, title_box.h };
                SDL_Rect rect3 = (SDL_Rect){ media->txtrs[k].size.x / 4, media->txtrs[k].size.y / 4,
                                             media->txtrs[k].size.x / 3, media->txtrs[k].size.y / 3 };
                SDL_RenderCopy(sdl->rend, media->txtrs[k].sdl_t, &rect3, &rect2);
            }
        }
	    i++;
    }
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

t_vec2d					find_node(int p_x, int p_y, t_grid *grid)
{
	float				mapx;
	float				mapy;

	mapx = (float)(p_x - grid->box.x) / grid->scale;
	mapy = (float)(p_y - grid->box.y) / grid->scale;
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
	char            pos_neg[2];
	t_vec2d_d       diff1;
	t_vec2d_d       diff2;
    int             i;
    int             j;
    int             k;

	if (n < 4)
		return (TRUE);
    pos_neg[0] = FALSE;
    pos_neg[1] = FALSE;
    i = -1;
    while (++i < n)
    {
        if (v[i] != -2)
        {
            j = i + 1;
            diff2.x = vertices[v[i]].x - vertices[v[(j) % n]].x;
            diff2.y = vertices[v[i]].y - vertices[v[(j) % n]].y;
            k = j + 1;
            diff1.x = vertices[v[(k) % n]].x - vertices[v[(j) % n]].x;
            diff1.y = vertices[v[(k) % n]].y - vertices[v[(j) % n]].y;
            diff1.x = diff1.x * diff2.y - diff1.y * diff2.x;
            if (diff1.x > 0)
                pos_neg[0] = TRUE;
            else if (diff1.x < 0)
                pos_neg[1] = TRUE;
            if (pos_neg[0] == TRUE && pos_neg[1] == TRUE)
                return (FALSE);
        }
    }
	return (TRUE);
}

char 					sector_status(t_sector sector, t_wall *walls, t_vec2d *vertices, int n)
{
	int					i;
	int					j;
	int 				tmp[n];
	int                 *w;
	char                status;

	if (!walls || !vertices)
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
	else if (sec_is_convex(vertices, sector.v, sector.n_v) == FALSE)
	    status = SEC_CONCAVE_CLOSED;
	else
	    status = SEC_CONVEX_CLOSED;
    free(w);
	return(status);
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

void                    fill_grid_walls(int n_walls, t_wall *walls, int n_vectors, t_vec2d *vertices, t_grid *grid)
{
    int                 wall_i;
    int                 v1;
    int                 v2;

    if (!walls || !vertices || !grid || n_walls < 1 || n_vectors < 2 || n_walls >= MAX_N_WALLS)
        return ;
    wall_i = 0;
    while (wall_i < n_walls)
    {
        v1 = walls[wall_i].v1;
        v2 = walls[wall_i].v2;
        if (walls[wall_i].type != WALL_DOOR && v1 >= 0 && v1 < n_vectors && v2 >= 0 && v2 < n_vectors)
        {
            draw_line_grid((t_line){ vertices[v1], vertices[v2] }, (signed char)wall_i, grid->nodes);
        }
        wall_i++;
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

void					move_grid_drag(t_prog *prog, t_vec2d mouse, t_grid *grid);
void					move_grid_keys(t_prog *prog, t_grid *grid);


void					move_grid_drag(t_prog *prog, t_vec2d mouse, t_grid *grid)
{
	if (!prog || !grid)
		return ;
	if (mouse_over(grid->box, mouse))
	{
		grid->box.x += mouse.x - prog->click.x;
		grid->box.y += mouse.y - prog->click.y;
		prog->click = mouse;
		prog->features[F_REDRAW] = 1;
	}
	else
		prog->click = (t_vec2d){ 0, 0 };
}

void					move_grid_keys(t_prog *prog, t_grid *grid)
{
    if (!prog || !grid)
        return ;
    if (prog->move.x || prog->move.y)
    {
        grid->box.x += prog->move.x;
        grid->box.y += prog->move.y;
        prog->move = (t_vec2d){ 0, 0 };
        prog->features[F_REDRAW] = 1;
    }
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
					id = find_vector(world->vertices, grid->active[0], world->n_vectors);
				if (id == -1)
					grid->active[0] = (t_vec2d){ -1, -1 };
				to_erase = grid->active[0];
			}
			else if (id >= 0)
			{
				if (to_erase.x != -1)
					grid->nodes[to_erase.x][to_erase.y] = NODE_EMPTY;
				grid->active[1] = find_node(mouse.x, mouse.y, grid);
				world->vertices[id] = grid->active[1];
				grid->nodes[grid->active[1].x][grid->active[1].y] = NODE_FULL;
				to_erase = grid->active[1];
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
			if (p[i].x + (float)(y - p[i].y) / (p[j].y - p[i].y) * (p[j].x - p[i].x) < x)
				odd = odd == FALSE ? TRUE : FALSE;
		}
		j = i;
		i++;
	}
	return (odd);
}

t_vec2d					find_in_grid(int p_x, int p_y, t_grid *grid)
{
	t_vec2d				res;

	res.x = (float)(p_x - grid->box.x) / grid->scale;
	res.y = (float)(p_y - grid->box.y) / grid->scale;
	return (res);
}

int 					in_sector(t_vec2d p, t_world *world, t_grid *grid)
{
	int 				id;
	t_vec2d				map_p;
	t_vec2d				*v;
	int 				i;
	int 				j;

	if (!world || !grid || mouse_over(grid->box, p) == FALSE)
		return (-1);
	id = -1;
	map_p = find_in_grid(p.x, p.y, grid);
	i = 0;
	while (i < world->n_sectors)
	{
		if ( world->sectors[i].status != SEC_OPEN)
		{
			if (!(v = ft_memalloc(sizeof(t_vec2d) * world->sectors[i].n_v)))
				return (-1);
			j = 0;
			while (j < world->sectors[i].n_v)
			{
			    if (world->sectors[i].v[j] < 0 || world->sectors[i].v[j] >= world->n_vectors)
                    return (-1);
				v[j] = world->vertices[world->sectors[i].v[j]];
				j++;
			}
			if (dot_inside_sector(map_p.x, map_p.y, v, world->sectors[i].n_v) == TRUE)
				id = i;
			free(v);
		}
		i++;
	}
	return (id);
}

t_grid                  *get_grid(void)
{
    t_grid					*grid;

    if (!(grid = (t_grid *)ft_memalloc(sizeof(t_grid))))
        return (NULL);
    grid->scale = WIN_H / GRID_SIZE;
    grid->box.w = GRID_SIZE * grid->scale;
    grid->box.h = GRID_SIZE * grid->scale;
    grid->box.x = (WIN_W - grid->box.w) / 2;
    grid->box.y = (WIN_H - grid->box.h) / 2;
    grid->active[0] = (t_vec2d){ -1, -1 };
    grid->active[1] = (t_vec2d){ -1, -1 };
    clean_grid(grid);
    return (grid);
}

void					game_loop(t_sdl *sdl, t_media *media)
{
	t_grid				*grid;
	t_prog				*prog;

	prog = NULL;
    grid = NULL;
	if (!(prog = get_prog()) || init_modes(sdl, media, prog) == FAIL || !(grid = get_grid()))
	{
		ft_putstr("\x1b[32mReturning fail from game loop.\x1b[0m\n");
		return ;
	}
	while (prog->modes[prog->mode_id].input(sdl, grid, media, prog) == FALSE)
	{
		prog->modes[prog->mode_id].update(sdl, grid,  media, prog);
		prog->modes[prog->mode_id].render(sdl, grid, media, prog);
		SDL_Delay(10);
	}
	free(grid);
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


	if (!(sdl = get_sdl()))
	{
		ft_putstr("\x1b[32mSdl is NULL, Returning fail from main function.\x1b[0m\n");
		return (FAIL);
	}
	if (!(media = get_assets()) || load_sdl_media(media, sdl) == FAIL)
	{
		ft_putstr("\x1b[32mMedia is NULL, Returning fail from main function.\x1b[0m\n");
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
