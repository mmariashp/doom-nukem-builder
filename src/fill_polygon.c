/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fill_polygon.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mshpakov <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/09/28 16:42:09 by mshpakov          #+#    #+#             */
/*   Updated: 2019/09/28 16:42:10 by mshpakov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "builder.h"

void					replace_color(int **screen, int to_replace, int new)
{
	int					i;
	int					j;

	i = -1;
	if (!screen)
		return ;
	while (++i < W_W)
	{
		j = -1;
		while (++j < W_H)
			screen[i][j] = screen[i][j] == to_replace ? new : screen[i][j];
	}
}

unsigned short			fill_row(int x_from, int x_to, int y, int **screen)
{
	unsigned short		overlay;

	overlay = FALSE;
	if (!screen)
		return (overlay);
	while (x_from < x_to)
	{
		if (screen[x_from][y] == 0)
			screen[x_from][y] = TMP_COLOR;
		else if (screen[x_from][y] == CONVEX_COLOR || \
		screen[x_from][y] == CONCAVE_COLOR || \
		screen[x_from][y] == ACTIVE_SEC_COLOR)
		{
			screen[x_from][y] = YELLOW;
			overlay = TRUE;
		}
		x_from++;
	}
	return (overlay);
}

int						get_inter_x(int *inter_x, t_vec *p, int n_p, int y)
{
	int					n_interx;
	int					j;
	int					i;

	if (!p || !inter_x)
		return (0);
	n_interx = 0;
	j = n_p - 1;
	i = 0;
	while (i < n_p)
	{
		if ((p[i].y < y && p[j].y >= y) || (p[j].y < y && p[i].y >= y))
			inter_x[n_interx++] = (int)(p[i].x + (double)(y - p[i].y) / \
			(p[j].y - p[i].y) * (p[j].x - p[i].x));
		j = i++;
	}
	bubble_sort((int *)inter_x, n_interx);
	return (n_interx);
}

unsigned short			fill_polygon(t_vec *p, int n_p, int **screen,
																	int color)
{
	int					n_ix;
	int					ix[MAX_SEC_WALLS];
	int					i;
	t_vec				minmax_p[3];
	unsigned short		o;

	if (!p || !screen)
		return (FALSE);
	o = FALSE;
	bounding_box(&minmax_p[0], &minmax_p[1], p, n_p);
	minmax_p[2] = (t_vec){ 0, minmax_p[0].y - 1 };
	while (++minmax_p[2].y < minmax_p[1].y)
	{
		n_ix = get_inter_x(ix, p, n_p, minmax_p[2].y);
		i = 0;
		while (i < n_ix && ix[i] < minmax_p[1].x)
		{
			if (ix[i + 1] > minmax_p[0].x)
				o = fill_row(clamp(ix[i], minmax_p[0].x, ix[i]), clamp(ix[i + 1\
				], ix[i + 1], minmax_p[1].x), minmax_p[2].y, screen) ? TRUE : o;
			i += 2;
		}
	}
	replace_color(screen, TMP_COLOR, color);
	return (o);
}
