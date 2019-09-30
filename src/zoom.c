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

void					zoom_displace(t_vec *grid_p, t_vec p, float old_scl,\
float new_scl)
{
	float				mapx;
	float				mapy;

	mapx = (float)(p.x - grid_p->x) / old_scl;
	mapy = (float)(p.y - grid_p->y) / old_scl;
	grid_p->x = (int)(p.x - (mapx * new_scl));
	grid_p->y = (int)(p.y - (mapy * new_scl));
}

void					zoom_grid(t_prog *prog, t_vec mouse, t_grid *grid)
{
	float				new;
	static float		min_scl = 1;
	float 				old_scl;
	t_vec				p;

	if (!prog || !grid)
		return ;
	if (mouse_over(grid->box, mouse))
	{
		old_scl = grid->scl;
		if (min_scl <= 1)
			get_min_scl(&min_scl);
		new = grid->scl + grid->scl * 0.3f * prog->zoom;
		grid->scl = clamp_f(new, min_scl, 100);
		p = (t_vec){ grid->box.x, grid->box.y };
		zoom_displace(&p, mouse, old_scl, grid->scl);
		grid->box.x = p.x;
		grid->box.y = p.y;
		prog->redraw = 1;
	}
	prog->zoom = 0;
}

void                    highlight_sec_nod(t_vec *p, int n_w, t_grid *grid)
{
    int                 i;

    i = -1;
    while (++i < n_w)
    {
        if (ingrid(p[i]))
            grid->nod[p[i].x][p[i].y] = NODE_SEC;
    }
}



void                    zoom_to_sector(t_sec *sector, t_vec *vecs, t_grid \
*grid, t_prog *prog)
{
    int					i;
    t_vec				p[sector->n_w];
    t_vec             min;
    t_vec             max;
    t_vec             center;
    static t_vec      desired = { W_W * 0.3, W_H * 0.5 };
    int                 n;
	t_vec				tmp;

    if (!prog || !vecs)
        return ;
    i = -1;
	while (++i < sector->n_v)
		p[i] = vecs[sector->v[i]];
    bounding_box(&min, &max, p, sector->n_v);
    highlight_sec_nod((t_vec *)p, sector->n_w, grid);
    n = get_max(max.x - min.x, max.y - min.y);
    min = transform_to_screen(min, grid);
	max = transform_to_screen(max, grid);
    center = (t_vec){ min.x + (max.x - min.x) / 2, min.y + (max.y - min.y) / 2 };
    grid->box.x = (grid->box.x - center.x) + desired.x;
    grid->box.y = (grid->box.y - center.y) + desired.y;
	tmp = (t_vec){ grid->box.x, grid->box.y };
    zoom_displace(&tmp, desired, grid->scl, get_scl_to_sector(n, W_W * 0.6, W_H * 0.6) );
	grid->box.x = tmp.x;
	grid->box.y = tmp.y;
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
	t_vec				tmp;

	if (!prog || !grid)
		return ;
	max.x = 0;
	min.x = GRID_SIZE;
	max.y = 0;
	min.y = GRID_SIZE;
	draw_node((p[0] = v1), 15, BABY_PINK, prog->screen);
	update_min_max(&min, &max, p[0]);
	draw_node((p[1] = v2), 15, BABY_PINK, prog->screen);
	update_min_max(&min, &max, p[1]);
	n = get_max(max.x - min.x, max.y - min.y);
	min = transform_to_screen(min, grid);
	max = transform_to_screen(max, grid);
	center = (t_vec){ min.x + (max.x - min.x) / 2, min.y + (max.y - min.y) / 2 };
	grid->box.x = (grid->box.x - center.x) + desired.x;
	grid->box.y = (grid->box.y - center.y) + desired.y;
	tmp = (t_vec){ grid->box.x, grid->box.y };
	zoom_displace(&tmp, desired, grid->scl, get_scl_to_sector(n, W_W * 0.6, W_H * 0.6) );
	grid->box.x = tmp.x;
	grid->box.y = tmp.y;
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
	t_vec				tmp;

    i = 0;
    max.x = 0;
    min.x = GRID_SIZE;
    max.y = 0;
    min.y = GRID_SIZE;
    while (i < n_v)
		update_min_max(&min, &max, v[i++]);
    n = get_max(max.x - min.x, max.y - min.y);
	min = transform_to_screen(min, grid);
	max = transform_to_screen(max, grid);
    center = (t_vec){ min.x + (max.x - min.x) / 2, min.y + (max.y - min.y) / 2 };
    grid->box.x = (grid->box.x - center.x) + desired.x;
    grid->box.y = (grid->box.y - center.y) + desired.y;
	tmp = (t_vec){ grid->box.x, grid->box.y };
    zoom_displace(&tmp, desired, grid->scl, get_scl_to_sector(n, W_W * 0.9, W_H * 0.9) );
	grid->box.x = tmp.x;
	grid->box.y = tmp.y;
    grid->scl = get_scl_to_sector(n, W_W * 0.9, W_H * 0.9);
}
