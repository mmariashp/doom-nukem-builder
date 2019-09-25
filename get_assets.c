
#include "builder.h"

unsigned 				close_file(int fd)
{
	if (close(fd) == -1)
		return (FAIL);
	ft_putstr("Closed the file.\n");
	return (SUCCESS);
}

unsigned 				open_for_read(const char *path, int *fd)
{
	if (!path)
		return (FAIL);
	*fd = open(path, O_RDONLY);
	if (*fd == -1)
		return (FAIL);
	ft_putstr("Opened the file.\n");
	return (SUCCESS);
}

char 					*identify_extension(const char *line)
{
	char 				*extension;
	size_t 				i;
	size_t 				len;

	if (!line)
		return (NULL);
	while (*line && *line != '.')
		line++;
	len = ft_strlen(line);
	extension = ft_strdup(line);
	if (!extension)
		return (NULL);
	i = 1;
	while (i < len)
	{
		if (!ft_isalpha(extension[i]))
			return (NULL);
		i++;
	}
	ft_putstr("identified extension as ");
	ft_putstr(extension);
	ft_putchar('\n');
	return (extension);
}

char 					*identify_path(const char *line)
{
	char 				*path;
	size_t 				i;
	size_t 				len;

	if (!line)
		return (NULL);
	while (*line && *line != '.')
		line++;
	len = ft_strlen(line);
	path = ft_strdup(line);
	if (!path)
		return (NULL);
	i = 1;
	while (i < len)
	{
		if (!ft_isalnum(path[i]) && path[i] != '.' &&  path[i] != '/')
			return (NULL);
		i++;
	}
	ft_putstr("identified path as ");
	ft_putstr(path);
	ft_putchar('\n');
	return (path);
}

short 					identify_section(const char *line)
{
	static const char 	sections[TOTAL_SECTIONS][16] = { LEVELS, TEXTURES, ITEMS, SOUNDS, FONTS};
	short 				id;
	short 				i;

	id = -1;
	i = 0;
	line++;
	while (id == -1 && i < TOTAL_SECTIONS)
	{
		if (!(ft_strcasecmp(line, sections[i])))
			id = i;
		i++;
	}
	ft_putstr("identified section as ");
	ft_putnbr(id);
	ft_putchar('\n');
	return (id);
}

void					free_tab(char **tab, int size)
{
	int					i;

	if(!tab)
		return ;
	i = 0;
	while(i < size && tab[i] != NULL)
		free(tab[i++]);
	free(tab);
}

char					**realloc_tab(char **tab, size_t n)
{
	char				**new;
	size_t				j;

	j = 0;
	if (n == 0)
		return (NULL);
	if (!(new = (char **)malloc(sizeof(char *) * n)))
		return (NULL);
	while (j < n - 1 && tab[j])
	{
		new[j] = ft_strdup(tab[j]);
		j++;
	}
	free_tab(tab, n - 1);
	new[j] = NULL;
	return (new);
}

int 					*realloc_int_array(int *tab, size_t n)
{
	int 				*new;
	size_t				j;

	j = 0;
	if (n == 0)
		return (NULL);
	if (!(new = (int *)malloc(sizeof(int) * n)))
		return (NULL);
	while (j < n - 1)
	{
		new[j] = tab[j];
		j++;
	}
	if (tab)
		free(tab);
	new[j] = 0;
	return (new);
}

int						read_item_type(char *line)
{
	static char 		types[TOTAL_TYPES][12] = { "coin", "key", "object",\
						"enemy", "super_bonus", "health", "ammo", "light" };
	int 				res;

	res = 0;
	while (*line && !ft_isalpha(*line))
		line++;
	if (*line == '\0')
		return (res);
	while (res < TOTAL_TYPES)
	{
		if (*line == types[res][0])
			return (res);
		res++;
	}
	return (0);
}

void					read_section_filenames(char *line, t_section *section)
{
	char 				*tmp;

	if (!section || !line)
		return ;
	if (section->id == 2)
	{
		while (*line && !ft_isalpha(*line) && *line != '(')
			line++;
		if (!(section->extra = realloc_int_array(section->extra, section->n_files + 1)))
			return ;
		section->extra[section->n_files] = read_item_type(line);
		while (*line && *line != ')')
			line++;
	}
	while (*line && !ft_isalpha(*line))
		line++;
	if (!(section->names = realloc_tab(section->names, section->n_files + 1)) || \
	!(section->names[section->n_files] = ft_strdup(line)))
		return ;
	tmp = ft_strjoin(section->path, line);
	if (!(section->tab = realloc_tab(section->tab, section->n_files + 1)) || \
	!(section->tab[section->n_files] = ft_strjoin(tmp, section->extension)))
	{
		free(tmp);
		return ;
	}
	free(tmp);
	section->n_files++;

}

void 					refresh_section(t_section *section)
{
	if (!section)
		return ;
	if (section->path)
		ft_memdel((void **)&section->path);
	if (section->extension)
		ft_memdel((void **)&section->extension);
	if (section->tab)
		free_tab(section->tab, section->n_files);
	if (section->names)
		free_tab(section->names, section->n_files);
	if (section->extra)
		free(section->extra);
	section->extra = NULL;
	section->tab = NULL;
	section->names = NULL;
	section->id = -1;
	section->path = NULL;
	section->extension = NULL;
	section->n_files = 0;
}

unsigned short			get_n(char *line, unsigned short min, unsigned short max)
{
	int 				tmp;

	tmp = 0;
	while (*line && ft_isdigit(*line) == FALSE)
		line++;
	if (*line)
		tmp = ft_atoi(line);
	if (tmp >= min && tmp < max)
		return((unsigned)tmp);
	else
		return (0);
}

short					get_wall_txtr(char *line, unsigned short min, unsigned short max)
{
	int 				tmp;

	tmp = 0;
	while (*line && ft_isdigit(*line) == FALSE)
		line++;
	if (*line)
		tmp = ft_atoi(line);
//	else
//		return (-1);
	if (tmp >= min && tmp < max)
		return((short)tmp);
	else
		return (-1);
}

int 					get_wall_type(char c)
{
	if (c == 'e' || c == 'E')
		return (WALL_EMPTY);
	if (c == 'f' || c == 'F')
		return (WALL_FILLED);
	if (c == 'd' || c == 'D')
		return (WALL_DOOR);
	return (-1);
}

unsigned				get_fl_ceil(char *line, int *h, short *txtr, unsigned short max_txtr)
{
	unsigned short		height_found;
	int 				tmp;

	if (!line || !h || !txtr)
		return (FAIL);
	height_found = FALSE;
	while (*line)
	{
		if (ft_isdigit(*line) || *line == '-')
		{
			if (height_found == FALSE)
			{
				*h = clamp(ft_atoi(line), MIN_HEIGHT, MAX_HEIGHT);
				height_found = TRUE;
				if (*line == '-')
					line++;
				while (*line && ft_isdigit(*line))
					line++;
				continue ;
			}
			else
			{
				tmp = ft_atoi(line);
				if (tmp < 0 || tmp >= max_txtr)
					return (FAIL);
				*txtr = (short)tmp;
				break ;
			}
		}
		line++;
	}
	return (SUCCESS);
}

unsigned short			count_walls(char const *s)
{
	unsigned short		count;

	count = 0;
	s++;
	while (*s && *s != '\'')
	{
		if (ft_isdigit(*s))
		{
			count++;
			while (*s && ft_isdigit(*s))
				s++;
		}
		else if (*s != ' ')
			return (0);
		else
			s++;
	}
	return (count);
}

unsigned short			count_items(char const *s)
{
	unsigned short		count;
	unsigned short		comma;
	unsigned short		space;
	unsigned short		nb;

	count = 0;
	s++;
	while (*s && *s != '\'')
	{
		if (*s == '(')
		{
			s++;
			comma = 0;
			space = 0;
			nb = 0;
			while (*s && *s != ')')
			{
				if (*s == ',')
					comma++;
				else if (*s == ' ')
					space++;
				else if (ft_isdigit(*s))
				{
					nb++;
					while (*s && ft_isdigit(*s))
						s++;
					continue ;
				}
				s++;
			}
			if (*s != ')' || comma != 1 || space != 1 || nb != 3)
				return (0);
			count++;
			s++;
		}
		else
			s++;
	}
	return (count);
}

int 					exists_in_array(int const *array, int n, int number)
{
	int 				i;

	i = 0;
	while (i < n)
	{
		if (array[i++] == number)
			return (TRUE);
	}
	return (FALSE);
}

void					pair_sort(int *a, int n)
{
	int					one;
	int					prev;
	int					next;
	int 				count;

	count = -1;
	while (++count < n)
	{
		one = 0;
		prev = n - 1;
		next = 2;
		while (one < n - 1)
		{
			if (next >= n)
				next = 0;
			if (a[one] != a[prev] && a[one + 1] != a[next])
				swap_ints(&a[one], &a[one + 1]);
			if (next == 0)
				break ;
			prev = one + 1;
			one = next;
			next = one + 2;
		}
	}
}

int 					sector_closed(int *tmp, int n)
{
	int					i;

	if (!tmp)
		return (FALSE);
	if (tmp[0] != tmp[n - 1])
		return (FALSE);
	i = 1;
	while (i < n - 2)
	{
		if (tmp[i] != tmp[i + 1])
			return (FALSE);
		i += 2;
	}
	return (TRUE);
}

int 					fill_sector_v(t_sec *sector, t_wall *walls, int n)
{
	int					i;
	int					j;
	int 				tmp[n];

	if (!sector || !walls)
		return (FAIL);
	ft_memset(tmp, -1, sizeof(int) * n);
	i = -1;
	j = 0;
	while (++i < sector->n_walls)
	{
		if (j < n)
			tmp[j++] = walls[sector->sec_walls[i]].v1;
		if (j < n)
			tmp[j++] = walls[sector->sec_walls[i]].v2;
	}
	pair_sort(tmp, n);
	sector->status = sector_closed(tmp, n) == FALSE ? SEC_OPEN : SEC_CONCAVE_CLOSED;
	i = -1;
	j = 0;
	while (++i < n)
	{
		if (exists_in_array(tmp, j, tmp[i]) == FALSE)
			tmp[j++] = tmp[i];
	}
    if (!(sector->v = (int *)ft_memalloc(sizeof(int) * j)))
        return (FAIL);
    sector->n_v = j;
    ft_memset(sector->v, -1, sizeof(int) * sector->n_v);
    i = 0;
    while (i < sector->n_v)
	{
    	sector->v[i] = tmp[i];
    	i++;
	}
	return (SUCCESS);
}

int 					get_sec_v(t_sec *sector, t_wall *walls)
{
	if (!sector || !walls)
		return (FAIL);
	return (fill_sector_v(sector, walls, sector->n_walls * 2));
}

int 					get_sec_walls(t_sec *sector, char *line)
{
	int 				*walls;
	unsigned  short		count;
	int					i;

	if (!sector || !line)
		return (FAIL);
	line = ft_strchr(line, '\'');
	if (!line)
		return (FAIL);
	count = count_walls(line);
	if (count < MIN_SECTOR_WALLS || count > MAX_SECTOR_WALLS)
		return (FAIL);
	sector->n_walls = count;
	walls = (int *)ft_memalloc(sizeof(int) * count);
	if (!walls)
		return (FAIL);
	ft_bzero(walls,sizeof(int) * count);
	line++;
	i = 0;
	while (*line && *line != '\'' && i < count)
	{
		if (ft_isdigit(*line))
		{
			walls[i++] = ft_atoi(line);
			line = ft_strchr(line, ' ');
			if (*line == '\0')
				break ;
		}
		else if (*line != ' ')
			return (FAIL);
		line++;
	}
	sector->sec_walls = walls;
	return (SUCCESS);
}

int 					get_sec_items(t_sec *sector, char *line)
{
	t_item				*items;
	unsigned  short		count;
	int					i;

	if (!sector || !line)
		return (FAIL);
	line = ft_strchr(line, '\'');
	if (!line)
		return (FAIL);
	count = count_items(line);
	if (count > MAX_SECTOR_ITEMS)
		return (FAIL);
	sector->n_items = count;
	items = (t_item *)ft_memalloc(sizeof(t_item) * count);
	if (!items)
		return (FAIL);
	ft_bzero(items, sizeof(t_item) * count);
	line++;
	i = 0;
	while (*line && *line != '\'' && i < count)
	{
		if (*line == '(')
		{
			line++;
			if (!line)
				return (FAIL);
			items[i].p.x = ft_atoi(line);
			if (items[i].p.x < 0 || items[i].p.x > GRID_SIZE)
				return (FAIL);
			if (!(line = ft_strchr(line, ',')))
				return (FAIL);
			line++;
			if (!line)
				return (FAIL);
			items[i].p.y = ft_atoi(line);
			if (items[i].p.y < 0 || items[i].p.y > GRID_SIZE)
				return (FAIL);
			if (!(line = ft_strchr(line, ' ')))
				return (FAIL);
			line++;
			if (!line)
				return (FAIL);
			items[i].id = ft_atoi(line);
			if (items[i].id < 0 || items[i].id > MAX_ITEMFULL)
				return (FAIL);
			if (!(line = ft_strchr(line, ')')))
				return (FAIL);
			i++;
		}
		line++;
	}
	sector->items = items;
	return (SUCCESS);
}

int 					get_sec_fl_ceil(t_sec *sector, char *line)
{

	if (!sector || !line)
		return (FAIL);
	sector->floor = 0;
	sector->floor_txtr = 0;
	sector->ceiling = 0;
	sector->ceil_txtr = 0;
	line = ft_strchr(line, 'f');
	if (!line)
		return (FAIL);
	if (get_fl_ceil(line, &sector->floor, &sector->floor_txtr, MAX_N_TXTRS) == FAIL)
		return (FAIL);
	line = ft_strchr(line, 'c');
	if (!line)
		return (FAIL);
	if (get_fl_ceil(line, &sector->ceiling, &sector->ceil_txtr, MAX_N_TXTRS) == FAIL)
		return (FAIL);
	return (SUCCESS);
}

void 					get_walls(t_wall *wall, char *line, t_vec2d p)
{
	if (!wall || !line)
		return ;
	wall->v1 = p.x;
	wall->v2 = p.y;
	while (*line && !ft_isalpha(*line))
		line++;
	wall->type = get_wall_type(*line);
	wall->txtr = get_wall_txtr(line, 0, MAX_N_TXTRS);
	wall->door = -1;
}

unsigned				read_line(char *str, unsigned short status, t_world *world, unsigned short world_no)
{
	static unsigned		v_count = 0;
	static unsigned		w_count = 0;
	static unsigned		s_count = 0;
//	static unsigned		d_count = 0;
	static unsigned		p_count = 0;
	static unsigned short		current_world = 0;
	t_vec2d				p;
	char 				sep;

	char *line;
	line = str;

	if (current_world != world_no)
	{
		v_count = 0;
		w_count = 0;
		s_count = 0;
//		d_count = 0;
		p_count = 0;
		current_world = world_no;
	}
	if (!line || !world || !status)
		return (FAIL);
	if (!(line = ft_strchr(line, ')')))
		return (FAIL) ;
	line++;
	if (status != R_SECTORS)
	{
		sep = status == R_WALLS ? '-' : ',';
		p.x = ft_atoi(line);
		if (!(line = ft_strchr(line, sep)))
			return (FAIL) ;
		line++;
		p.y = ft_atoi(line);
	}
	if (status == R_VECTORS && v_count < world->n_vecs)
	{
		p = (t_vec2d){ clamp(p.x, 0, GRID_SIZE), clamp(p.y, 0, GRID_SIZE) };
		world->vecs[v_count] = p;
		v_count++;
	}
	if (status == R_WALLS && w_count < world->n_walls)
	{
		if (p.x < MIN_VERTEX_ID || p.x > MAX_VERTEX_ID || p.x > (int)v_count ||
			p.y < MIN_VERTEX_ID || p.y > MAX_VERTEX_ID || p.y > (int)v_count) {
			return (FAIL);
		}
		get_walls(&world->walls[w_count], line, p);
		if (*line == '\0' || world->walls[w_count].type == -1)
		{
			ft_putstr("Incorrect wall type for wall ");
			ft_putnbr(w_count);
			ft_putchar('\n');
			return (FAIL);
		}
		if (world->walls[w_count].type == WALL_FILLED && world->walls[w_count].txtr == -1)
		{
			ft_putstr("Incorrect wall texture for wall ");
			ft_putnbr(w_count);
			ft_putchar('\n');
			return (FAIL);
		}
		w_count++;
	}
	if (status == R_SECTORS && s_count < world->n_sec)
	{
		world->sec[s_count].items = NULL;
		world->sec[s_count].sec_walls = NULL;
		world->sec[s_count].v = NULL;
		world->sec[s_count].floor = 0;
		world->sec[s_count].ceiling = 0;
		world->sec[s_count].floor_txtr = 0;
		world->sec[s_count].ceil_txtr = 0;
		world->sec[s_count].n_items = 0;
		world->sec[s_count].n_walls = 0;
        world->sec[s_count].n_v = 0;
		world->sec[s_count].status = 0;
		if (get_sec_fl_ceil(&world->sec[s_count], line) == FAIL)
		{
			ft_putstr("Incorrect floor or ceiling for sector ");
			ft_putnbr(s_count);
			ft_putchar('\n');
			return (FAIL);
		}
		line = ft_strchr(line, 'w');
		if (!line)
			return (FAIL);
		if (get_sec_walls(&world->sec[s_count], line) == FAIL)
		{
			ft_putstr("Incorrect walls for sector ");
			ft_putnbr(s_count);
			ft_putchar('\n');
			return (FAIL);
		}
		line = ft_strchr(line, 'i');
		if (!line)
			return (FAIL);
		if (get_sec_items(&world->sec[s_count], line) == FAIL)
		{
			ft_putstr("Incorrect items for sector ");
			ft_putnbr(s_count);
			ft_putchar('\n');
			return (FAIL);
		}
		s_count++;
	}
	if (status == R_PLAYER && p_count < 2)
	{
		if (p_count == 0)
			world->p_start = p;
		else
			world->p_end = p;
		p_count++;
	}
	return (SUCCESS);
}

unsigned 				read_map(int fd, t_world *world, unsigned short world_no)
{
	int					ret;
	char				*line;
	char 				*tmp;
	unsigned short		i;
	unsigned short		j;
	unsigned short		status;
	char 				tab[5];

	if (!world || fd < 1)
		return (FAIL);
	status = 0;
	ft_strcpy(tab, "0VWSP");
	line = NULL;
	i = -1;
	while ((ret = get_next_line(fd, &(line))) == 1)
	{
		tmp = line;
		if (++i == 0)
		{
			world->n_vecs = get_n(line, 0, MAX_VERTEX_ID);
			if (world->n_vecs == 0)
				world->vecs = NULL;
			else
			{
				if (!(world->vecs = (t_vec2d *)malloc(sizeof(t_vec2d) * world->n_vecs)))
					return (FAIL);
				ft_bzero(world->vecs, sizeof(t_vec2d) * world->n_vecs);
			}
		}
		else if (i == 1)
		{
			if (!(world->n_walls = get_n(line, MIN_N_WALLS, MAX_N_WALLS)))
				world->walls = NULL;
			else
			{
				if (!(world->walls = (t_wall *)malloc(sizeof(t_wall) * world->n_walls)))
					return (FAIL);
				ft_bzero(world->walls, sizeof(t_wall) * world->n_walls);
			}
		}
		else if (i == 2)
		{
			if (!(world->n_sec = get_n(line, MIN_N_SECTORS, MAX_N_SECTORS)))
				world->sec = NULL;
			else
			{
				if (!(world->sec = (t_sec *)malloc(sizeof(t_sec) * world->n_sec)))
					return (FAIL);
				ft_bzero(world->sec, sizeof(t_sec) * world->n_sec);
			}
		}
		else
		{
			if (ft_isdigit(line[0]) && status > 0 && status < 6)
			{
				if (read_line(tmp, status, world, world_no) == FAIL)
					return (FAIL);
			}
			else
			{
				j = 0;
				while (j < 5 && tab[j] && line[0])
				{
					if (tab[j] == line[0])
					{
						status = j;
						break ;
					}
					j++;
				}
			}
		}
		ft_strdel(&line);
	}
	ft_strdel(&line);
	if (ret == -1)
		return (FAIL);
	i = 0;
	while (i < world->n_sec)
		get_sec_v(&world->sec[i++], world->walls);
	return (SUCCESS);
}

unsigned				get_map(t_world *world, unsigned short world_no)
{
	int 				fd;

	if (!world || !world->full_path)
		return (FAIL);
	if (open_for_read(world->full_path, &fd) == FAIL)
	{
		ft_putstr("Couldn't open the file ");
		ft_putstr(world->full_path);
		ft_putstr(". Must be in the specified folder and have read permissions.\n");
		return (FAIL);
	}
	if (read_map(fd, world, world_no) == FAIL)
		return (FAIL);
	if (close_file(fd) == FAIL)
	{
		ft_putstr("Couldn't close the file");
		ft_putstr(world->full_path);
		ft_putstr(".\n");
		return (FAIL);
	}
	return (SUCCESS);
}

unsigned				read_fonts(t_media *media, t_section *section)
{
	short 				i;

	if (!media || !section || !section->tab || media->fonts)
		return (FAIL);
	i = 0;
	if (!(media->fonts = (char **)ft_memalloc(sizeof(char *) * section->n_files)))
		return (FAIL);
	media->n_fonts = section->n_files;
	ft_bzero(media->fonts, sizeof(char *) * section->n_files);
	while (i < section->n_files)
	{
		if (!(media->fonts[i] = ft_strdup(section->tab[i])))
			return (FAIL);
		ft_bzero(media->fonts[i], sizeof(char *) * section->n_files);
		i++;
	}
	return (SUCCESS);
}

unsigned				read_sounds(t_media *media, t_section *section)
{
	short 				i;

	if (!media || !section || !section->tab || media->sounds)
		return (FAIL);
	if (!(media->sounds = (char **)ft_memalloc(sizeof(char *) * section->n_files)))
		return (FAIL);
	media->n_sounds = section->n_files;
	ft_bzero(media->sounds, sizeof(char *) * section->n_files);
	i = -1;
	while (++i < section->n_files)
	{
		if (!(media->sounds[i] = ft_strdup(section->tab[i])))
			return (FAIL);
		ft_bzero(media->sounds[i], sizeof(media->sounds[i]));
	}
	return (SUCCESS);
}

unsigned				read_textures(t_media *media, t_section *section)
{
	short 				i;

	if (!media || !section || !section->tab ||!(media->txtrs = \
	(t_texture *)ft_memalloc(sizeof(t_texture) * section->n_files)))
		return (FAIL);
	media->n_txtrs = section->n_files;
	ft_bzero(media->txtrs, sizeof(t_texture) * section->n_files);
	i = -1;
	while (++i < section->n_files)
	{
		media->txtrs[i].full_path = NULL;
		media->txtrs[i].name = NULL;
		if (!(media->txtrs[i].full_path = ft_strdup(section->tab[i])))
			return (FAIL);
		if (!(media->txtrs[i].name = ft_strdup(section->names[i])))
			return (FAIL);
		media->txtrs[i].sdl_t = NULL;
	}
	return (SUCCESS);
}

unsigned				read_itemfull(t_media *media, t_section *section)
{
	short 				i;

	if (!media || !section || !section->tab || section->n_files > MAX_ITEMFULL)
		return (FAIL);
	if (!(media->itemfull = (t_itemfull *)ft_memalloc(sizeof(t_itemfull) * section->n_files)))
		return (FAIL);
	media->n_itemfull = section->n_files;
	ft_bzero(media->itemfull, sizeof(t_itemfull) * section->n_files);
	i = -1;
	while (++i < section->n_files)
	{
		media->itemfull[i].full_path = NULL;
		media->itemfull[i].filename = NULL;
		if (section->tab[i] && !(media->itemfull[i].full_path = ft_strdup(section->tab[i])))
			return (FAIL);
		if (section->names[i] && !(media->itemfull[i].filename = ft_strdup(section->names[i])))
			return (FAIL);
		if (section->extra)
			media->itemfull[i].type = section->extra[i];
		else
			media->itemfull[i].type = 0;
	}
	return (SUCCESS);
}

unsigned short          find_doors(int n, t_wall *walls)
{
    int                 i;
    int                 j;
    int                 wall;

    i = -1;
    while (++i < n)
    {
        if (walls[i].type == WALL_DOOR)
        {
            j = -1;
            wall = -1;
            while ( ++j < n )
            {
                if (j != i && ((walls[j].v1 == walls[i].v1 && walls[j].v2 == walls[i].v2) ||
                               (walls[j].v1 == walls[i].v2 && walls[j].v2 == walls[i].v1)) )
                {
                    if (walls[j].type != WALL_EMPTY || walls[j].door != -1 || wall != -1)
                        return (FAIL);
                    walls[j].door = i;
                    wall = j;
                }
            }
        }
    }
    return (SUCCESS);
}

unsigned				read_levels(t_media *media, t_section *section)
{
	short 				i;

	if (!media || !section || !section->tab || media->worlds || section->n_files < 1 || section->n_files > MAX_LEVELS)
		return (FAIL);
	if (!(media->worlds = (t_world *)ft_memalloc(sizeof(t_world) * section->n_files)))
		return (FAIL);
	media->n_worlds = section->n_files;
	ft_bzero(media->worlds, sizeof(t_world) * section->n_files);
	i = -1;
	while (++i < section->n_files)
	{
		if (!section->tab[i] || !section->names[i])
			return (FAIL);
		ft_bzero(&media->worlds[i], sizeof(t_world));
		media->worlds[i].full_path = ft_strdup(section->tab[i]);
		media->worlds[i].filename = ft_strdup(section->names[i]);
		media->worlds[i].sec = NULL;
		media->worlds[i].walls = NULL;
		media->worlds[i].vecs = NULL;
		media->worlds[i].doors = NULL;
		media->worlds[i].n_sec = 0;
		media->worlds[i].n_vecs = 0;
		media->worlds[i].n_walls = 0;
		media->worlds[i].n_doors = 0;
		printf("NAME %s\n", media->worlds[i].filename);
		if (!media->worlds[i].full_path  || !media->worlds[i].filename || get_map(&media->worlds[i], i) == FAIL)
		{
			ft_putendl("failed to get map\n");
			return (FAIL);
		}
		if (find_doors(media->worlds[i].n_walls, media->worlds[i].walls) == FAIL) // will remove
        {
            ft_putstr("Errors in doors in level: ");
            ft_putendl(media->worlds[i].filename);
		    return (FAIL);
        }
	}
	return (SUCCESS);
}

unsigned				update_media(t_media *media, t_section *section)
{
	unsigned			(*update_section[])(t_media*, t_section*) = { read_levels, read_textures, read_itemfull, read_sounds, read_fonts };

	if (!media || !section)
		return (FAIL);
	ft_bzero(&media->paths[section->id], sizeof(char) * 20);
	ft_bzero(&media->extensions[section->id], sizeof(char) * 10);
	if (section->path && ft_strlen(section->path) < 20)
		ft_strcpy(media->paths[section->id], section->path);
	if (section->extension && ft_strlen(section->extension) < 10)
		ft_strcpy(media->extensions[section->id], section->extension);
	return (update_section[section->id](media, section));
}

void					free_media(t_media *media)
{
	int 				i;
	int 				j;

	if (!media)
		return ;
	if (media->worlds)
	{
		j = 0;
		while (j < media->n_worlds )
			free_world(&media->worlds[j++]);
		free(media->worlds);
	}
	if (media->txtrs)
	{
		i = -1;
		while (++i < media->n_txtrs)
		{
			if (media->txtrs[i].full_path)
				free(media->txtrs[i].full_path);
			if (media->txtrs[i].name)
				free(media->txtrs[i].name);
//			if (media->txtrs[i].sdl_t)
//				SDL_DestroyTexture(media->txtrs[i].sdl_t);
		}
		free(media->txtrs);
	}
	if (media->fonts)
	{
		i = -1;
		while (++i < media->n_fonts)
		{
			if (media->fonts[i])
				free(media->fonts[i]);
		}
		free(media->fonts);
	}
	if (media->sounds)
	{
		i = -1;
		while (++i < media->n_sounds)
		{
			if (media->sounds[i])
				free(media->sounds[i]);
		}
		free(media->sounds);
	}
	if (media->itemfull)
	{
		i = -1;
		while (++i < media->n_itemfull)
		{
			if (media->itemfull[i].filename)
			{
				free(media->itemfull[i].filename);
				media->itemfull[i].filename = NULL;
			}
			if (media->itemfull[i].full_path)
			{
				free(media->itemfull[i].full_path);
				media->itemfull[i].full_path = NULL;
			}
		}
		free(media->itemfull);
	}
	ft_memdel((void **)&media);
}

void					free_section(t_section * section)
{
	if (section->path)
		free(section->path);
	if (section->extension)
		free(section->extension);
	if (section->tab && section->n_files)
		free_tab(section->tab, section->n_files);
	if (section->names && section->n_files)
		free_tab(section->names, section->n_files);
	section->path = NULL;
	section->extension = NULL;
	section->tab = NULL;
	section->n_files = 0;
	section->names = NULL;
}

void					init_media(t_media *media)
{
	if (!media)
		return ;
	media->w_id = -1;
	media->txtrs = NULL;
	media->itemfull = NULL;
	media->fonts = NULL;
	media->sounds = NULL;
	media->worlds = NULL;
	media->n_itemfull = 0;
	media->n_worlds = 0;
	media->n_txtrs = 0;
	media->n_fonts = 0;
	media->n_sounds = 0;
}

t_media					*read_assets(int fd)
{
	t_section			section;
	int					ret;
	char				*line;
	char 				*tmp;
	char 				**tab;
	char 				**names;
	t_media				*media;

	section.id = -1;
	section.n_files = 0;
	section.path = NULL;
	section.extension = NULL;
	section.tab = NULL;
	section.names = NULL;
	section.extra = NULL;
	if (!(media = (t_media *)ft_memalloc(sizeof(t_media))))
		return (NULL);
	init_media(media);
	while ((ret = get_next_line(fd, &(line))) == 1)
	{
		tmp = line;
		if (ft_strlen(line) > 2)
		{
			if (line[0]== '#')
			{
				if (section.id == -1)
				{
					section.id = identify_section(tmp);
					if (section.tab || !(tab = (char **)ft_memalloc(sizeof(char *) * 1)) ||
					!(names = (char **)ft_memalloc(sizeof(char *) * 1)))
					{
						free_section(&section);
						ft_strdel(&line);
						free_media(media);
						return (NULL);
					}
					section.tab = tab;
					section.names = names;
				}
				else if (!ft_strcmp(line, "###"))
				{
					if (update_media(media, &section) == SUCCESS)
						refresh_section(&section);
					else
					{
						ft_putstr("failed to update media at section ");
						ft_putnbr(section.id);
						ft_putendl(" ");
						free_section(&section);
						ft_strdel(&line);
						free_media(media);
						return (NULL);
					}
				}
			}
			else if (ft_isdigit(line[0]) && section.id >= 0 && section.path && section.extension)
				read_section_filenames(tmp, &section);
			else if (line[0] == 'P' && !section.path)
				section.path = identify_path(tmp);
			else if (line[0] == 'E' && !section.extension)
				section.extension = identify_extension(tmp);
		}
		ft_strdel(&line);
	}
	free_section(&section);
	ft_strdel(&line);
	if (ret == -1)
	{
		free_media(media);
		return (NULL);
	}
	return (media);
}

t_media					*get_assets(void)
{
	int					fd;
	t_media				*media;

	if (open_for_read(ASSET_FILE, &fd) == FAIL)
	{
		ft_putstr("Couldn't open the file");
		ft_putstr(ASSET_FILE);
		ft_putstr(". Must be in the root folder and have read permissions.\n");
		return (NULL);
	}
	media = read_assets(fd);
	if (!media)
	{
		ft_putendl("error in reading assets\n");
		return (NULL);
	}
	ft_putendl("Read assets\n");
	if (close_file(fd) == FAIL)
	{
		ft_putstr("Couldn't close the file");
		ft_putstr(ASSET_FILE);
		ft_putstr(".\n");
		free_media(media);
		return (NULL);
	}
	validate_media(media);
	return (media);
}
