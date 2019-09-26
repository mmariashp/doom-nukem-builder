
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
	static int 			select[8] = { -1, -1, -1, -1, -1, -1, -1, -1 };

	if (id >= 8)
		return (-1);
	else if (set_get_unset == 0)
		select[id] = value;
	else if (set_get_unset == 2)
		select[id] = -1;
	return (select[id]);
}

int 					lit_it(char set_get_unset, unsigned short id, int value)
{
	static int 			select[7] = { -1, -1, -1, -1, -1, -1, -1 };

	if (id >= 7)
		return (-1);
	else if (set_get_unset == 0)
		select[id] = value;
	else if (set_get_unset == 2)
		select[id] = -1;
	return (select[id]);
}

char 					*get_full_path(char *filename, char *ext, char *path)
{
	char 				*with_path;
	char 				*res;

	if (!path || !ext || !filename)
		return (NULL);
	with_path = ft_strjoin(path, filename);
	if (with_path)
	{
		res = ft_strjoin(with_path, ext);
		free(with_path);
		if (res)
			return (res);
	}
	return (NULL);
}
