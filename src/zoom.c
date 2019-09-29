/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   zoom.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mshpakov <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/09/26 12:04:41 by mshpakov          #+#    #+#             */
/*   Updated: 2019/09/26 12:04:43 by mshpakov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "builder.h"

void					get_min_scl(float *res_min_scl)
{
	static int 			min_size = -1;
	static float		min_scl = 1;

	if (min_size == -1)
	{
		if (W_H < W_W)
			min_size = W_H / 2;
		else
			min_size = W_W / 2;
		min_scl = (float)min_size / GRID_SIZE;
	}
	*res_min_scl = min_scl;
}

float					get_scl_to_sector(int size, int max_w, int max_h)
{
    float               scl;
    int                 box;

    box = get_min(max_w, max_h);
    scl = (float)box / size;
    return (scl);
}

void					zoom_displace(int *gridx, int *gridy, t_vec p, float old_scl, float new_scl)
{
	float				mapx;
	float				mapy;

	mapx = (float)(p.x - *gridx) / old_scl;
	mapy = (float)(p.y - *gridy) / old_scl;
	*gridx = (int)(p.x - (mapx * new_scl));
	*gridy = (int)(p.y - (mapy * new_scl));
}

void					zoom_grid(t_prog *prog, t_vec mouse, t_grid *grid)
{
	float				new;
	static float		min_scl = 1;
	float 				old_scl;

	if (!prog || !grid)
		return ;
	if (mouse_over(grid->box, mouse))
	{
		old_scl = grid->scl;
		if (min_scl <= 1)
			get_min_scl(&min_scl);
		new = grid->scl + grid->scl * 0.3f * prog->zoom;
		grid->scl = clamp_f(new, min_scl, 100);
		zoom_displace(&grid->box.x, &grid->box.y, mouse, old_scl, grid->scl );
		prog->redraw = 1;
	}
	prog->zoom = 0;
}

void                    highlight_sec_nod(t_vec *p, int n_w, t_grid *grid)
{
    int                 i;

    i = 0;
    while (i < n_w)
    {
        if (p[i].x >= 0 && p[i].x < GRID_SIZE && p[i].y >= 0 && p[i].y < GRID_SIZE)
            grid->nod[p[i].x][p[i].y] = NODE_SEC;
        i++;
    }
}

void                    zoom_to_sector(t_sec *sector, t_vec *vecs, t_grid *grid, t_prog *prog)
{
    int					i;
    t_vec				p[sector->n_w];
    t_vec             min;
    t_vec             max;
    t_vec             center;
    static t_vec      desired = { W_W * 0.3, W_H * 0.5 };
    int                 n;

    if (!prog || !vecs)
        return ;
    max.x = 0;
    min.x = GRID_SIZE;
    max.y = 0;
    min.y = GRID_SIZE;
    i = -1;
    while (++i < sector->n_v)
    {
        p[i] = vecs[sector->v[i]];
        draw_node(p[i], 15, BABY_PINK, prog->screen);
        if (p[i].x < min.x)
            min.x = p[i].x;
        if (p[i].x > max.x)
            max.x = p[i].x;
        if (p[i].y < min.y)
            min.y = p[i].y;
        if (p[i].y > max.y)
            max.y = p[i].y;
    }
    highlight_sec_nod((t_vec *)p, sector->n_w, grid);
    n = get_max(max.x - min.x, max.y - min.y);
    min.x = (int)(grid->box.x + min.x * grid->scl);
    min.y = (int)(grid->box.y + min.y * grid->scl);
    max.x = (int)(grid->box.x + max.x * grid->scl);
    max.y = (int)(grid->box.y + max.y * grid->scl);
    center = (t_vec){ min.x + (max.x - min.x) / 2, min.y + (max.y - min.y) / 2 };
    grid->box.x = (grid->box.x - center.x) + desired.x;
    grid->box.y = (grid->box.y - center.y) + desired.y;
    zoom_displace(&grid->box.x, &grid->box.y, desired, grid->scl, get_scl_to_sector(n, W_W * 0.6, W_H * 0.6) );
    grid->scl = get_scl_to_sector(n, W_W * 0.6, W_H * 0.6);
}

void                    zoom_to_wall(t_vec v1, t_vec v2, t_grid *grid, t_prog *prog)
{
	t_vec				p[2];
	t_vec             min;
	t_vec             max;
	t_vec             center;
	static t_vec      desired = { W_W * 0.3, W_H * 0.5 };
	int                 n;

	if (!prog || !grid)
		return ;
	max.x = 0;
	min.x = GRID_SIZE;
	max.y = 0;
	min.y = GRID_SIZE;

		p[0] = v1;
		draw_node(p[0], 15, BABY_PINK, prog->screen);
		if (p[0].x < min.x)
			min.x = p[0].x;
		if (p[0].x > max.x)
			max.x = p[0].x;
		if (p[0].y < min.y)
			min.y = p[0].y;
		if (p[0].y > max.y)
			max.y = p[0].y;

	p[1] = v2;
	draw_node(p[1], 15, BABY_PINK, prog->screen);
	if (p[1].x < min.x)
		min.x = p[1].x;
	if (p[1].x > max.x)
		max.x = p[1].x;
	if (p[1].y < min.y)
		min.y = p[1].y;
	if (p[1].y > max.y)
		max.y = p[1].y;

	n = get_max(max.x - min.x, max.y - min.y);
	min.x = (int)(grid->box.x + min.x * grid->scl);
	min.y = (int)(grid->box.y + min.y * grid->scl);
	max.x = (int)(grid->box.x + max.x * grid->scl);
	max.y = (int)(grid->box.y + max.y * grid->scl);
	center = (t_vec){ min.x + (max.x - min.x) / 2, min.y + (max.y - min.y) / 2 };
	grid->box.x = (grid->box.x - center.x) + desired.x;
	grid->box.y = (grid->box.y - center.y) + desired.y;
	zoom_displace(&grid->box.x, &grid->box.y, desired, grid->scl, get_scl_to_sector(n, W_W * 0.6, W_H * 0.6) );
	grid->scl = get_scl_to_sector(n, W_W * 0.6, W_H * 0.6);
}

void                    zoom_to_map(int n_v, t_vec *v, t_grid *grid)
{
    int                 i;
    int                 n;
    t_vec             min;
    t_vec             max;
    t_vec             center;
    static t_vec      desired = { W_W / 2, W_H / 2 };

    i = 0;
    max.x = 0;
    min.x = GRID_SIZE;
    max.y = 0;
    min.y = GRID_SIZE;
    while (i < n_v)
    {
        if (v[i].x < min.x)
            min.x = v[i].x;
        if (v[i].x > max.x)
            max.x = v[i].x;
        if (v[i].y < min.y)
            min.y = v[i].y;
        if (v[i].y > max.y)
            max.y = v[i].y;
        i++;
    }
    n = get_max(max.x - min.x, max.y - min.y);
    min.x = (int)(grid->box.x + min.x * grid->scl);
    min.y = (int)(grid->box.y + min.y * grid->scl);
    max.x = (int)(grid->box.x + max.x * grid->scl);
    max.y = (int)(grid->box.y + max.y * grid->scl);
    center = (t_vec){ min.x + (max.x - min.x) / 2, min.y + (max.y - min.y) / 2 };
    grid->box.x = (grid->box.x - center.x) + desired.x;
    grid->box.y = (grid->box.y - center.y) + desired.y;
    zoom_displace(&grid->box.x, &grid->box.y, desired, grid->scl, get_scl_to_sector(n, W_W * 0.9, W_H * 0.9) );
    grid->scl = get_scl_to_sector(n, W_W * 0.9, W_H * 0.9);
}
