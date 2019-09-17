
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
	static const char 	sections[TOTAL_SECTIONS][16] = { LEVELS, TEXTURES, SOUNDS, FONTS};
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
		ft_memdel((void **)&tab[i++]);
	ft_memdel((void **)&tab);
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

void					read_section_filenames(char *line, t_section *section)
{
	char 				*tmp;

	if (!section || !line)
		return ;
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
	if (tmp >= min && tmp <= max)
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
	unsigned short		level_found;
	int 				tmp;

	if (!line || !h || !txtr)
		return (FAIL);
	level_found = FALSE;
	while (*line)
	{
		if (ft_isdigit(*line) || *line == '-')
		{
			if (level_found == FALSE)
			{
				*h = clamp(ft_atoi(line), MIN_HEIGHT, MAX_HEIGHT);
				level_found = TRUE;
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

int 					exists_in_array(int *array, int n, int number)
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
	int					one = 0;
	int					prev = n - 1;
	int					next = 2;
	int					swap;

	while (one < n - 1)
	{
		if (next >= n)
			next = 0;
		if (a[one] != a[prev] && a[one + 1] != a[next])
		{
			swap = a[one];
			a[one] = a[one + 1];
			a[one + 1] = swap;
		}
		if (next == 0)
			break ;
		prev = one + 1;
		one = next;
		next = one + 2;
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

int 					fill_sector_v(t_sector *sector, t_wall *walls, int n)
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
		tmp[j++] = walls[sector->sec_walls[i]].v1;
		tmp[j++] = walls[sector->sec_walls[i]].v2;
	}
	pair_sort(tmp, n);
	if (sector_closed(tmp, n) == FALSE)
		sector->status = SEC_OPEN;
	else
		sector->status = SEC_CONCAVE_CLOSED;
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

int 					get_sector_v(t_sector *sector, t_wall *walls)
{
	if (!sector || !walls)
		return (FAIL);
	return (fill_sector_v(sector, walls, sector->n_walls * 2));
}

int 					get_sector_walls(t_sector *sector, char *line)
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
	ft_bzero(walls, sizeof(walls));
	line++;
	i = 0;
	while (*line && *line != '\'' && i < count)
	{
		if (ft_isdigit(*line))
		{
			walls[i++] = ft_atoi(line);
			line = ft_strchr(line, ' ');
		}
		else if (*line != ' ')
			return (FAIL);
		line++;
	}
	sector->sec_walls = walls;
	return (SUCCESS);
}

int 					get_sector_fl_ceil(t_sector *sector, char *line, int n_txtrs)
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
	if (get_fl_ceil(line, &sector->floor, &sector->floor_txtr, n_txtrs) == FAIL)
		return (FAIL);
	line = ft_strchr(line, 'c');
	if (!line)
		return (FAIL);
	if (get_fl_ceil(line, &sector->ceiling, &sector->ceil_txtr, n_txtrs) == FAIL)
		return (FAIL);
	return (SUCCESS);
}

void 					get_walls(t_wall *wall, char *line, t_vec2d p, int n_txtrs)
{
	if (!wall || !line)
		return ;
	wall->v1 = p.x;
	wall->v2 = p.y;
	while (*line && !ft_isalpha(*line))
		line++;
	wall->type = get_wall_type(*line);
	wall->txtr = get_wall_txtr(line, 0, n_txtrs - 1);
	wall->door = -1;
}

unsigned				read_line(char *str, unsigned short status, t_world *world, unsigned short world_no)
{
	static unsigned		v_count = 0;
	static unsigned		w_count = 0;
	static unsigned		s_count = 0;
	static unsigned		t_count = 0;
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
		t_count = 0;
		p_count = 0;
		current_world = world_no;
	}
	if (!line || !world || !status)
		return (FAIL);
	while (*line)
	{
		if (*line++ == ')')
			break ;
	}
	if (*line == '\0')
		return (FAIL) ;

	if (status != R_SECTORS)
	{
		sep = status == R_WALLS ? '-' : ',';
		p.x = ft_atoi(line);
		if (status == R_TEXTURES)
			p.y = 0;
		else
		{
			while (*line)
			{
				if (*line++ == sep)
					break ;
			}
			p.y = ft_atoi(line);
		}
	}
	if (status == R_TEXTURES && t_count < world->n_txtrs)
	{
		world->textures[t_count] = p.x;
		t_count++;
	}
	if (status == R_VECTORS && v_count < world->n_vectors)
	{
		p.x = clamp(p.x, MIN_VERTEX_COORD, MAX_VERTEX_COORD);
		p.y = clamp(p.y, MIN_VERTEX_COORD, MAX_VERTEX_COORD);

		world->vertices[v_count] = p;
		v_count++;
	}
	if (status == R_WALLS && w_count < world->n_walls)
	{
		if (p.x < MIN_VERTEX_ID || p.x > MAX_VERTEX_ID || p.x > (int)v_count ||
			p.y < MIN_VERTEX_ID || p.y > MAX_VERTEX_ID || p.y > (int)v_count) {
			return (FAIL);
		}
		get_walls(&world->walls[w_count], line, p, world->n_txtrs);
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
		world->sec[s_count].sec_walls = NULL;
		world->sec[s_count].v = NULL;
		world->sec[s_count].floor = 0;
		world->sec[s_count].ceiling = 0;
		world->sec[s_count].floor_txtr = 0;
		world->sec[s_count].ceil_txtr = 0;
		world->sec[s_count].n_walls = 0;
        world->sec[s_count].n_v = 0;
		world->sec[s_count].status = 0;
		if (get_sector_fl_ceil(&world->sec[s_count], line, world->n_txtrs) == FAIL)
		{
			ft_putstr("Incorrect floor or ceiling for sector ");
			ft_putnbr(s_count);
			ft_putchar('\n');
			return (FAIL);
		}
		line = ft_strchr(line, 'w');
		if (!line)
			return (FAIL);
		if (get_sector_walls(&world->sec[s_count], line) == FAIL)
		{
			ft_putstr("Incorrect walls for sector ");
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
	char 				tab[6];

	if (!world || fd < 1)
		return (FAIL);
	i = 0;
	status = 0;
	ft_strcpy(tab, "0VWSPT");
	world->walls = NULL;
	world->vertices = NULL;
	world->sec = NULL;
	world->textures = NULL;
	line = NULL;
	while ((ret = get_next_line(fd, &(line))) == 1)
	{
		tmp = line;
		if (i == 0)
		{
			world->n_vectors = get_n(line, 0, MAX_VERTEX_ID);
			if (world->n_vectors == 0)
				world->vertices = NULL;
			else
			{
				if (!(world->vertices = (t_vec2d *)malloc(sizeof(t_vec2d) * world->n_vectors)))
					return (FAIL);
				ft_bzero(world->vertices, sizeof(world->vertices));
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
				ft_bzero(world->walls, sizeof(world->walls));
			}
		}
		else if (i == 2)
		{
			if (!(world->n_sec = get_n(line, MIN_N_SECTORS, MAX_N_SECTORS)))
				world->sec = NULL;
			else
			{
				if (!(world->sec = (t_sector *)malloc(sizeof(t_sector) * world->n_sec)))
					return (FAIL);
				ft_bzero(world->sec, sizeof(world->sec));
			}
		}
		else if (i == 3)
		{
			if (!(world->n_txtrs = get_n(line, MIN_N_TXTRS, MAX_N_TXTRS)))
			{
				ft_putendl("Not enough textures.");
				return (FAIL);
			}
			if (!(world->textures = (int *)malloc(sizeof(int) * world->n_txtrs)))
				return (FAIL);
			ft_bzero(world->textures, sizeof(world->textures));
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
				while (j < 6 && tab[j] && line[0])
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
		i++;
	}
	ft_strdel(&line);
	if (ret == -1)
		return (FAIL);
	if (line)
		free(line);
	line = NULL;
	i = 0;
	while (i < world->n_sec)
	{
		get_sector_v(&world->sec[i++], world->walls);
	}
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

	if (!media || !section || !section->tab)
		return (FAIL);
	i = 0;
	if (!(media->fonts = (char **)ft_memalloc(sizeof(char *) * section->n_files)))
		return (FAIL);
	media->n_fonts = section->n_files;
	ft_bzero(media->fonts, sizeof(media->fonts));
	while (i < section->n_files)
	{
		if (!(media->fonts[i] = ft_strdup(section->tab[i])))
			return (FAIL);
		ft_bzero(media->fonts[i], sizeof(media->fonts[i]));
		i++;
	}
	return (SUCCESS);
}

unsigned				read_sounds(t_media *media, t_section *section)
{
	short 				i;

	if (!media || !section || !section->tab)
		return (FAIL);
	i = 0;
	if (!(media->sounds = (char **)ft_memalloc(sizeof(char *) * section->n_files)))
		return (FAIL);
	media->n_sounds = section->n_files;
	ft_bzero(media->sounds, sizeof(media->sounds));
	while (i < section->n_files)
	{
		if (!(media->sounds[i] = ft_strdup(section->tab[i])))
			return (FAIL);
		ft_bzero(media->sounds[i], sizeof(media->sounds[i]));
		i++;
	}
	return (SUCCESS);
}

unsigned				read_textures(t_media *media, t_section *section)
{
	short 				i;

	if (!media || !section || !section->tab)
		return (FAIL);
	i = 0;
	if (!(media->txtrs = (t_texture *)ft_memalloc(sizeof(t_texture) * section->n_files)))
		return (FAIL);
	media->n_txtrs = section->n_files;
	ft_bzero(media->txtrs, sizeof(media->txtrs));
	while (i < section->n_files)
	{
		media->txtrs[i].full_path = NULL;
		media->txtrs[i].name = NULL;
		if (!(media->txtrs[i].full_path = ft_strdup(section->tab[i])))
			return (FAIL);
		if (!(media->txtrs[i].name = ft_strdup(section->names[i])))
			return (FAIL);
		media->txtrs[i].sdl_t = NULL;
		i++;
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
	i = 0;
	ft_putendl("allocated media world\n");
	media->n_worlds = section->n_files;
	ft_bzero(media->worlds, sizeof(media->worlds));
	while (i < section->n_files)
	{
		if (!section->tab[i] || !section->names[i])
			return (FAIL);
		ft_putendl("reading a level file\n");
		ft_bzero(&media->worlds[i], sizeof(media->worlds[i]));
		media->worlds[i].full_path = ft_strdup(section->tab[i]);
		media->worlds[i].filename = ft_strdup(section->names[i]);
		media->worlds[i].textures = NULL;
		media->worlds[i].sec = NULL;
		media->worlds[i].walls = NULL;
		media->worlds[i].vertices = NULL;
		media->worlds[i].n_sec = 0;
		media->worlds[i].n_vectors = 0;
		media->worlds[i].n_walls = 0;
		media->worlds[i].n_txtrs = 0;
		printf("NAME %s\n", media->worlds[i].filename);
		if (get_map(&media->worlds[i], i) == FAIL)
		{
			ft_putendl("failed to get map\n");
			return (FAIL);
		}
		if (find_doors(media->worlds[i].n_walls, media->worlds[i].walls) == FAIL)
        {
            ft_putstr("Errors in doors in level: ");
            ft_putendl(media->worlds[i].filename);
		    return (FAIL);
        }
		i++;
	}
	return (SUCCESS);
}

unsigned				update_media(t_media *media, t_section *section)
{
	unsigned			(*update_section[])(t_media*, t_section*) = { read_levels, read_textures, read_sounds, read_fonts };

	if (!media || !section)
		return (FAIL);
	ft_strcpy(media->paths[section->id], section->path);
	ft_strcpy(media->extensions[section->id], section->extension);
	return (update_section[section->id](media, section));
}

void					free_sector_walls(int *walls)
{
	if (!walls)
		return ;
	free(walls);
}

void					free_sector(t_sector *sector)
{
	if (!sector)
		return ;
	free_sector_walls(sector->sec_walls);
	if (sector->v)
		free(sector->v);
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
		{
			if (media->worlds[j].filename)
				free(media->worlds[j].filename);
			if (media->worlds[j].full_path)
				free(media->worlds[j].full_path);
			if (media->worlds[j].sec)
			{
				i = 0;
				while (i < media->worlds[j].n_sec)
					free_sector(&media->worlds[j].sec[i++]);
				free(media->worlds[j].sec);
			}
			if (media->worlds[j].vertices)
				free(media->worlds[j].vertices);
			if (media->worlds[j].walls)
				free(media->worlds[j].walls);
			if (media->worlds[j].textures)
				free(media->worlds[j].textures);
			j++;
		}
		free(media->worlds);
	}
	if (media->txtrs)
	{
		i = 0;
		while (i < media->n_txtrs)
		{
			if (media->txtrs[i].full_path)
				free(media->txtrs[i].full_path);
			if (media->txtrs[i].name)
				free(media->txtrs[i].name);
			if (media->txtrs[i].sdl_t)
				SDL_DestroyTexture(media->txtrs[i].sdl_t);
			i++;
		}
		free(media->txtrs);
	}
	if (media->fonts)
	{
		i = 0;
		while (i < media->n_fonts)
		{
			if (media->fonts[i])
				free(media->fonts[i]);
			i++;
		}
		free(media->fonts);
	}
	if (media->sounds)
	{
		i = 0;
		while (i < media->n_sounds)
		{
			if (media->sounds[i])
				free(media->sounds[i]);
			i++;
		}
		free(media->sounds);
	}
	ft_memdel((void **)&media);
}

void					free_section(t_section * section)
{
	if (section->path)
		ft_memdel((void **)&section->path);
	if (section->extension)
		ft_memdel((void **)&section->extension);
	if (section->tab)
		free_tab(section->tab, section->n_files);
	if (section->names)
		free_tab(section->names, section->n_files);
}

void					init_media(t_media *media)
{
	if (!media)
		return ;
	media->world_id = -1;
	media->txtrs = NULL;
	media->fonts = NULL;
	media->sounds = NULL;
	media->worlds = NULL;
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
	if (!(media = (t_media *)ft_memalloc(sizeof(t_media))))
		return (NULL);
	init_media(media);
	ft_putendl("allocated media\n");
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
						return (NULL);
					}
					section.tab = tab;
					section.names = names;
				}
				else if (!ft_strcmp(line, "###"))
				{
					if (update_media(media, &section) == SUCCESS)
					{
						ft_putendl("updated media\n");
						refresh_section(&section);
					}
					else
					{
						free_section(&section);
						ft_strdel(&line);
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
		return (NULL);
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
		return (NULL);
	}
	return (media);
}
