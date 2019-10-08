/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   useful.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mshpakov <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/09/28 20:09:24 by mshpakov          #+#    #+#             */
/*   Updated: 2019/09/28 20:09:26 by mshpakov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "builder.h"

unsigned short			within(int value, int min, int max)
{
	if (value <= min)
		return (FALSE);
	if (value >= max)
		return (FALSE);
	return (TRUE);
}

void					bounding_box(t_vec *min, t_vec *max, t_vec *p, \
																		int n_p)
{
	int					i;

	if (!min || !max)
		return ;
	*min = (t_vec){ 0, 0 };
	*max = (t_vec){ W_W, W_H };
	if (!p)
		return ;
	*min = (t_vec){ W_W, W_H };
	*max = (t_vec){ 0, 0 };
	i = -1;
	while (++i < n_p)
		update_min_max(min, max, p[i]);
	min->x = clamp(min->x, 0, W_W);
	max->x = clamp(max->x, 0, W_W);
	min->y = clamp(min->y, 0, W_H);
	max->y = clamp(max->y, 0, W_H);
}

unsigned short			already_in_sector(int id, int *vecs, int n_v)
{
	int					i;

	if (!vecs)
		return (FALSE);
	i = 0;
	while (i < n_v)
	{
		if (vecs[i++] == id)
			return (TRUE);
	}
	return (FALSE);
}

unsigned short			can_place_vec(t_vec v, t_world world, t_grid *grid, \
int wall)
{
	if (!within(v.x, -1, GRID_SIZE) || !within(v.y, -1, GRID_SIZE))
		return (FALSE);
	if (grid->nod[v.x][v.y] != NODE_FULL && nod_in_sec(v, &world) != -1
	&& wall == -1)
		return (FALSE);
	return (TRUE);
}

unsigned short			mouse_over(t_rec box, t_vec mouse)
{
	if (mouse.x <= box.x || mouse.x >= box.x + box.w)
		return (FALSE);
	if (mouse.y <= box.y || mouse.y >= box.y + box.h)
		return (FALSE);
	return (TRUE);
}

void					highlight_sec_nod(t_vec *p, int n_w, t_grid *grid)
{
	int                 i;

	i = -1;
	if (!p && !grid)
        return ;
	while (++i < n_w)
	{
		if (ingrid(p[i]))
			grid->nod[p[i].x][p[i].y] = NODE_SEC;
	}
}

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

void					set_false(char *one, char *two)
{
	if (one)
		*one = FALSE;
	if (two)
		*two = FALSE;
}

unsigned short			circle_inter_line(t_vec mouse, int radius, t_line line)
{
	t_vec_f				d;
	float				a;
	float				b;
	float				c;
	float				det;

	d.x = line.p1.x - line.p0.x;
	d.y = line.p1.y - line.p0.y;

	a = d.x * d.x + d.y * d.y;
	b = 2 * (d.x * (line.p0.x - mouse.x) + d.y * (line.p0.y - mouse.y));
	c = (line.p0.x - mouse.x) * (line.p0.x - mouse.x) +
		(line.p0.y - mouse.y) * (line.p0.y - mouse.y) -
		radius * radius;
	det = b * b - 4 * a * c;
	if (a <= 0.0000001 || det < 0 || (!within(mouse.x, \
	get_min(line.p0.x, line.p1.x), get_max(line.p0.x, line.p1.x)) &&
									  !within(mouse.y, get_min(line.p0.y, line.p1.y), \
	get_max(line.p0.y, line.p1.y))))
		return (0);
	else if (det == 0)
		return (1);
	else
		return (2);
}

void				    swap_ints(int *one, int *two)
{
	int				swap;

	if (!one || !two)
		return ;
	swap = *one;
	*one = *two;
	*two = swap;
}

void				    get_rgb(unsigned char *r, unsigned char *g, \
unsigned char *b, int color)
{
	if (!r || !g || !b)
		return ;
	*r = (unsigned char)((color >> 16) & 0xFF);
	*g = (unsigned char)((color >> 8) & 0xFF);
	*b = (unsigned char)(color & 0xFF);
}