

# include "builder.h"

void				bounding_box(t_vec2d *min, t_vec2d *max, t_vec2d *p, int n_p)
{
	int 			i;
	
	*min = (t_vec2d){ 0, 0 };
	*max = (t_vec2d){ WIN_W, WIN_H };
	if (!p || n_p < 3)
		return ;
	*min = (t_vec2d){ WIN_W, WIN_H };
	*max = (t_vec2d){ 0, 0 };
	i = 0;
	while (i < n_p)
	{
		if (p[i].x < min->x)
			min->x = p[i].x;
		if (p[i].x > max->x)
			max->x = p[i].x;
		if (p[i].y < min->y)
			min->y = p[i].y;
		if (p[i].y > max->y)
			max->y = p[i].y;
		i++;
	}
	min->x = clamp(min->x, 0, WIN_W);
	max->x = clamp(max->x, 0, WIN_W);
	min->y = clamp(min->y, 0, WIN_H);
	max->y = clamp(max->y, 0, WIN_H);
}

void				bubble_sort(int *tab, int n)
{
	int 			i;
	int 			lim;
	int 			swap;

	lim = n - 1;
	i = 0;
	while (i < lim)
	{
		if (tab[i] > tab[i + 1])
		{
			swap = tab[i];
			tab[i] = tab[i + 1];
			tab[i + 1] = swap;
			if (i)
				i--;
		}
		else
			i++;
	}
}

void				replace_color(int **screen, int to_replace, int new)
{
	int 			i;
	int 			j;

	i = -1;
	while (++i < WIN_W)
	{
		j = -1;
		while (++j < WIN_H)
			screen[i][j] = screen[i][j] == to_replace ? new : screen[i][j];
	}
}

unsigned short		fill_row(int x_from, int x_to, int y, int **screen)
{
	unsigned short	overlay;

	overlay = FALSE;
	while (x_from < x_to)
	{
		if (screen[x_from][y] == 0)
			screen[x_from][y] = TMP_COLOR;
		else if (screen[x_from][y] == CONVEX_COLOR || \
		screen[x_from][y] == CONCAVE_COLOR || \
		screen[x_from][y] == ACTIVE_SECTOR_COLOR)
		{
			screen[x_from][y] = YELLOW;
			overlay = TRUE;
		}
		x_from++;
	}
	return (overlay);
}

int 				get_inter_x(int *inter_x, t_vec2d *p, int n_p, int y)
{
	int				n_interx;
	int 			j;
	int 			i;

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
	return (n_interx);
}

unsigned short		fill_polygon(t_vec2d *p, int n_p, int **screen, int color)
{
	int				n_interx;
	int				inter_x[MAX_SEC_WALLS];
	int 			i;
	t_vec2d			min_max[2];
	t_vec2d			pix;
	unsigned short	overlay;

	overlay = FALSE;
	bounding_box(&min_max[0], &min_max[1], p, n_p);
	pix = (t_vec2d){ 0, min_max[0].y - 1 };
	while (++pix.y < min_max[1].y)
	{
		n_interx = get_inter_x(inter_x, p, n_p, pix.y);
		bubble_sort((int *)inter_x, n_interx);
		i = 0;
		while (i < n_interx && inter_x[i] < min_max[1].x)
		{
			if (inter_x[i + 1] > min_max[0].x)
				overlay = fill_row(clamp(inter_x[i], min_max[0].x,\
				inter_x[i]), clamp(inter_x[i + 1],\
				inter_x[i + 1], min_max[1].x), pix.y, screen) == TRUE ? TRUE : overlay;
			i += 2;
		}
	}
	replace_color(screen, TMP_COLOR, color);
	return (overlay);
}