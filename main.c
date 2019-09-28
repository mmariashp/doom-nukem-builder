/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mshpakov <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/09/26 12:04:32 by mshpakov          #+#    #+#             */
/*   Updated: 2019/09/26 12:04:35 by mshpakov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "builder.h"

void					write_text(char *str, t_sdl *sdl, t_rec rec, int color, char h_center)
{
	unsigned char		r;
	unsigned char		g;
	unsigned char		b;
	int					w;
	float 				ratio;
    SDL_Rect            renderQuad;
	SDL_Color			textColor;

    if (!str || !sdl || !sdl->rend || !sdl->font)
		return ;
	get_rgb(&r, &g, &b, color);
	textColor = (SDL_Color){ r, g, b, 0 };
	SDL_Surface* textSurface = TTF_RenderText_Solid(sdl->font, str, textColor);
	if (textSurface)
	{
		SDL_Texture* text = SDL_CreateTextureFromSurface(sdl->rend, textSurface);
		if (text)
		{
			ratio = textSurface->h ? (float)textSurface->w / textSurface->h : 1.f;
			w = clamp(textSurface->h * ratio, 0, rec.w);
			renderQuad = (SDL_Rect){ rec.x, rec.y, w, rec.h };
			if (h_center == TRUE)
				renderQuad.x = rec.x + (rec.w - w) / 2;
			else
				renderQuad.x = rec.x + rec.w * 0.05;
			SDL_RenderCopy(sdl->rend, text, NULL, &renderQuad);
			SDL_DestroyTexture(text);
		}
		SDL_FreeSurface(textSurface);
	}
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

char 					sector_status(t_sec sector, t_wall *walls, t_vec2d *vecs, int n)
{
	int					i;
	int					j;
	int 				*tmp;
	int                 *w;
	char                status;

	if (!walls || !vecs)
		return (SEC_OPEN);
	if (!(tmp = (int *)ft_memalloc(sizeof(int) * n)))
		return (SEC_OPEN);
	ft_memset(tmp, -1, sizeof(int) * n);
	i = -1;
	j = 0;
	while (++i < sector.n_w && j + 1 < n)
	{
		tmp[j++] = walls[sector.s_walls[i]].v1;
		tmp[j++] = walls[sector.s_walls[i]].v2;
	}
    i = -1;
    j = 0;
    while (++i < n && j < n)
    {
        if (tmp[i] >= 0)
            tmp[j++] = tmp[i];
    }
    if (!(w = (int *)ft_memalloc(sizeof(int) * j)))
	{
    	free(tmp);
    	return (SEC_OPEN);
	}
    i = 0;
    while (i < j && i < n)
    {
        w[i] = tmp[i];
        i++;
    }
	free(tmp);
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

void					upd_sec(t_sec *sec, t_wall *walls, t_vec2d *vecs, int n_s)
{
	int 				i;

	i = -1;
	if (!sec || !walls || !vecs)
		return ;
	while (++i < n_s)
		sec[i].status = sector_status(sec[i], walls, vecs, sec[i].n_w * 2);
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
			if (grid->p[0].x == -1)
			{
				grid->p[0] = find_node(mouse.x, mouse.y, grid);
				if (grid->p[0].x >= 0 && grid->p[0].y >= 0 &&
				grid->nodes[grid->p[0].x][grid->p[0].y] == NODE_FULL)
					id = find_vec(world->vecs, grid->p[0], world->n_v);
				if (id == -1)
				{
					grid->p[0] = (t_vec2d){ -1, -1 };
					move_grid_drag(prog, mouse, grid);
				}
				to_erase = grid->p[0];
			}
			else if (id >= 0)
			{

				grid->p[1] = find_node(mouse.x, mouse.y, grid);
				if (grid->nodes[grid->p[1].x][grid->p[1].y] != NODE_FULL)
				{
					if (to_erase.x != -1)
						grid->nodes[to_erase.x][to_erase.y] = NODE_EMPTY;
					world->vecs[id] = grid->p[1];
					grid->nodes[grid->p[1].x][grid->p[1].y] = NODE_FULL;
					to_erase = grid->p[1];
				}

			}
			prog->click = mouse;
			prog->redraw = 1;
		}
		else
			prog->click = (t_vec2d){ 0, 0 };
	}
	else
	{
		grid->p[0] = (t_vec2d){ -1, -1 };
		id = -1;
	}
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
			if (grid->p[0].x == -1)
			{
				grid->p[0] = find_node(mouse.x, mouse.y, grid);
				if (vec_same(grid->p[0], world->p_start))
					id = 0;
				else if (vec_same(grid->p[0], world->p_end))
					id = 1;
				else
				{
					grid->p[0] = (t_vec2d){ -1, -1};
					move_grid_drag(prog, mouse, grid);
				}
			}
			else if (id >= 0)
			{
				grid->p[1] = find_node(mouse.x, mouse.y, grid);
				if (grid->nodes[grid->p[1].x][grid->p[1].y] == NODE_EMPTY)
				{
					if (id == 0)
						world->p_start = grid->p[1];
					else
						world->p_end = grid->p[1];
				}
			}
			prog->click = mouse;
			prog->redraw = 1;
		}
		else
			prog->click = (t_vec2d){ 0, 0 };
	}
	else
	{
		grid->p[0] = (t_vec2d){ -1, -1 };
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

void					game_loop(t_sdl *sdl, t_media *media, t_prog *prog)
{
	t_grid				*grid;

    grid = NULL;
	select_it(0, FC_SELECT, 0);
	if (init_modes(media, prog) == FAIL || !(grid = get_grid()))
	{
		ft_putstr("\x1b[32mReturning fail from game loop.\x1b[0m\n");
		return ;
	}
	while (prog->modes[prog->m_id].input(sdl, grid, media, prog) == FALSE)
	{
		if (prog->modes[prog->m_id].update(sdl, grid,  media, prog) == FAIL)
			return ;
		prog->modes[prog->m_id].render(sdl, grid, media, prog);
		SDL_Delay(10);
	}
	free(grid);
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

unsigned short			mouse_over(t_rec box, t_vec2d mouse)
{
	if (mouse.x <= box.x || mouse.x >= box.x + box.w)
		return (FALSE);
	if (mouse.y <= box.y || mouse.y >= box.y + box.h)
		return (FALSE);
	return (TRUE);
}

int						main(void)
{
	t_sdl				*sdl;
	t_media				*media;
	t_prog				*prog;

	prog = NULL;
	if (!(media = get_assets()))
	{
		ft_putstr("\x1b[32mMedia is NULL, Returning fail from main function.\x1b[0m\n");
		system("leaks -q builder");
		return (FAIL);
	}
	if (!(sdl = get_sdl()))
	{
		ft_putstr("\x1b[32mSdl is NULL, Returning fail from main function.\x1b[0m\n");
		free_media(media);
		system("leaks -q builder");
		return (FAIL);
	}
	if (!(prog = get_prog(sdl->rend)) || load_sdl_media(media, sdl) == FAIL)
	{
		ft_putstr("\x1b[32mCouldn't load sdl media, Returning fail from main function.\x1b[0m\n");
		free_media(media);
		free_prog(prog);
		free_sdl(sdl);
		system("leaks -q builder");
		return (FAIL);
	}
	game_loop(sdl, media, prog);
	free_prog(prog);
	rewrite_media(media);
	free_media(media);
	free_sdl(sdl);
	ft_putstr("\x1b[32mReturning success from main function.\x1b[0m\n");
	system("leaks -q builder");
	return (SUCCESS);
}
