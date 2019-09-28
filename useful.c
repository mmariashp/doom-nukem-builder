
#include "builder.h"

unsigned short			within(int value, int min, int max)
{
	if (value <= min)
		return (FALSE);
	if (value >= max)
		return (FALSE);
	return (TRUE);
}

int 					select_it(char set_get_unset, unsigned short id, int value)
{
	static int 			select[9] = { -1, -1, -1, -1, -1, -1, -1, -1, -1 };

	if (id >= 9)
		return (-1);
	else if (set_get_unset == 0)
		select[id] = value;
	else if (set_get_unset == 2)
		select[id] = -1;
	return (select[id]);
}

int 					lit_it(char set_get_unset, unsigned short id, int value)
{
	static int 			select[9] = { -1, -1, -1, -1, -1, -1, -1, -1, -1 };

	if (id >= 9)
		return (-1);
	else if (set_get_unset == 0)
		select[id] = value;
	else if (set_get_unset == 2)
		select[id] = -1;
	return (select[id]);
}

char 					*get_full_path(char *name, char *ext, char *path)
{
	char 				*with_path;
	char 				*res;

	if (!path || !ext || !name)
		return (NULL);
	with_path = ft_strjoin(path, name);
	if (with_path)
	{
		res = ft_strjoin(with_path, ext);
		free(with_path);
		if (res)
			return (res);
	}
	return (NULL);
}

void					bounding_box(t_vec2d *min, t_vec2d *max, t_vec2d *p, \
																		int n_p)
{
	int 				i;

	if (!min || !max)
		return ;
	*min = (t_vec2d){ 0, 0 };
	*max = (t_vec2d){ WIN_W, WIN_H };
	if (!p || n_p < 3)
		return ;
	*min = (t_vec2d){ WIN_W, WIN_H };
	*max = (t_vec2d){ 0, 0 };
	i = -1;
	while (++i < n_p)
	{
		min->x = p[i].x < min->x ? p[i].x : min->x;
		max->x = p[i].x > max->x ? p[i].x : max->x;
		min->y = p[i].y < min->y ? p[i].y : min->y;
		max->y = p[i].y > max->y ? p[i].y : max->y;
	}
	min->x = clamp(min->x, 0, WIN_W);
	max->x = clamp(max->x, 0, WIN_W);
	min->y = clamp(min->y, 0, WIN_H);
	max->y = clamp(max->y, 0, WIN_H);
}