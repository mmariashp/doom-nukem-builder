
#include "builder.h"

unsigned 				open_for_write(const char *path, int *fd)
{
	if (!path)
		return (FAIL);
	*fd = open(path, O_WRONLY | O_CREAT | O_TRUNC, S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH);
	if (*fd == -1)
		return (FAIL);
	ft_putstr("Opened the file.\n");
	return (SUCCESS);
}

void					write_items(t_sector sector, int fd)
{
	int 				i;

	if (sector.items && sector.n_items > 0)
	{
		i = 0;
		while (i < sector.n_items)
		{
			ft_putstr_fd("(", fd);
			ft_putnbr_fd(sector.items[i].p.x, fd);
			ft_putstr_fd(",", fd);
			ft_putnbr_fd(sector.items[i].p.y, fd);
			ft_putstr_fd(" ", fd);
			ft_putnbr_fd(sector.items[i].id, fd);
			ft_putstr_fd(")", fd);
			i++;
		}
	}
}

unsigned short			write_level_section(int fd, t_world world, int section)
{
	static char 		title[4][9] = { "Vectors", "Walls", "Sectors", "Player" };
	unsigned short		i;
	unsigned short		n;

	ft_putendl_fd(title[section], fd);
	if (section == 0)
		n = world.n_vectors;
	else if (section == 1)
		n = world.n_walls;
	else if (section == 2)
		n = world.n_sec;
	else if (section == 3)
		n = 2;
	else
		return (FAIL);
	i = 0;
	while (i < n)
	{
		ft_putnbr_fd(i, fd);
		ft_putstr_fd(") ", fd);
		if (section == 0)
			{
				ft_putnbr_fd(world.vertices[i].x, fd);
				ft_putstr_fd(",", fd);
				ft_putnbr_fd(world.vertices[i].y, fd);
			}
		else if (section == 1)
		{
			ft_putnbr_fd(world.walls[i].v1, fd);
			ft_putstr_fd("-", fd);
			ft_putnbr_fd(world.walls[i].v2, fd);
			if (world.walls[i].type == WALL_FILLED)
				ft_putstr_fd(" filled ", fd);
			else if (world.walls[i].type == WALL_EMPTY)
				ft_putstr_fd(" empty ", fd);
			else
				ft_putstr_fd(" door ", fd);
			ft_putnbr_fd(world.walls[i].txtr, fd);
		}
		else if (section == 2)
		{
			ft_putstr_fd("floor(", fd);
			ft_putnbr_fd(world.sec[i].floor, fd);
			ft_putstr_fd(" ", fd);
			ft_putnbr_fd(world.sec[i].floor_txtr, fd);
			ft_putstr_fd(") ceil(", fd);
			ft_putnbr_fd(world.sec[i].ceiling, fd);
			ft_putstr_fd(" ", fd);
			ft_putnbr_fd(world.sec[i].ceil_txtr, fd);

			ft_putstr_fd(") walls '", fd);
			int j = 0;
			while (j < world.sec[i].n_walls)
			{
				ft_putnbr_fd(world.sec[i].sec_walls[j], fd);
				ft_putstr_fd(" ", fd);
				j++;
			}
			ft_putstr_fd("' items '", fd);
			write_items(world.sec[i], fd);
			ft_putstr_fd("'", fd);
		}
		else if (section == 3)
		{
			if (i == 0)
			{
				ft_putnbr_fd(world.p_start.x, fd);
				ft_putstr_fd(",", fd);
				ft_putnbr_fd(world.p_start.y, fd);
				ft_putstr_fd(" start", fd);
			}
			else
			{
				ft_putnbr_fd(world.p_end.x, fd);
				ft_putstr_fd(",", fd);
				ft_putnbr_fd(world.p_end.y, fd);
				ft_putstr_fd(" end", fd);
			}
		}
		ft_putchar_fd('\n', fd);
		i++;
	}
	ft_putchar_fd('\n', fd);
	return (SUCCESS);
}

unsigned short			write_level(int fd, t_world world)
{
	static char 		count[3][12] = { "Vectors: ", "Walls: ", "Sectors: " };
	int i = 0;

	while (i < 3)
	{
		ft_putstr_fd("Count ", fd);
		ft_putstr_fd(count[i], fd);
		if (i == 0)
			ft_putnbr_fd(world.n_vectors, fd);
		else if (i == 1)
			ft_putnbr_fd(world.n_walls, fd);
		else if (i == 2)
			ft_putnbr_fd(world.n_sec, fd);
//		else if (i == 3)
//			ft_putnbr_fd(world.n_txtrs, fd);
		ft_putchar_fd('\n', fd);
		i++;
	}
	ft_putchar_fd('\n', fd);
	i = 0;
	while (i < 4)
	{
		write_level_section(fd, world, i);
		i++;
	}
	return (SUCCESS);
}

void					write_item_type(int type, int fd)
{
	static char 		types[TOTAL_TYPES][12] = { "coin", "key", "object",\
						"enemy", "super_bonus", "health", "ammo" };

	if (type >= 0 && type < TOTAL_TYPES)
	{
		ft_putstr_fd("(", fd);
		ft_putstr_fd(types[type], fd);
		ft_putstr_fd(") ", fd);
	}
}

unsigned short			write_section(int fd, t_media *media, int section)
{
	static char 		title[TOTAL_SECTIONS][9] = { "Levels",\
						"Textures", "Items", "Sounds", "Fonts" };
	static char 		prefix[5][13] = {	"#",
											"Path: ",
											"Extension: ",
											"File Names: ",
											"###" };
	unsigned short		line;
	unsigned short		i;
	unsigned short		n_files;

	if (!media)
		return (FAIL);
	line = 0;
	while (line < 4)
	{
		ft_putstr_fd(prefix[line], fd);
		if (line == 0 && title[section])
			ft_putstr_fd(title[section], fd);
		else if (line == 1 && media->paths[section])
			ft_putstr_fd(media->paths[section], fd);
		else if (line == 2 && media->extensions[section])
			ft_putstr_fd(media->extensions[section], fd);
		ft_putchar_fd('\n', fd);
		line++;
	}
	if (section == 0)
		n_files = media->n_worlds;
	else if (section == 1)
		n_files = media->n_txtrs;
	else if (section == 2)
		n_files = media->n_itemfull;
	else if (section == 3)
		n_files = media->n_sounds;
	else if (section == 4)
		n_files = media->n_fonts;
	else
		return (FAIL);
	i = 0;
	n_files = clamp(n_files, 0, 20);
	while (i < n_files)
	{
		ft_putnbr_fd(i, fd);
		ft_putstr_fd(") ", fd);
		if (section == 0 && media->worlds[i].filename)
			ft_putstr_fd(media->worlds[i].filename, fd);
		else if (section == 1 && media->txtrs[i].name)
			ft_putstr_fd(media->txtrs[i].name, fd);
		else if (section == 2 && media->itemfull[i].filename)
		{
			write_item_type(media->itemfull[i].type, fd);
			ft_putstr_fd(media->itemfull[i].filename, fd);
		}
		else if (section == 3 && media->sounds[i])
			ft_putstr_fd(media->sounds[i], fd);
		else if (section == 4 && media->fonts[i])
			ft_putstr_fd(media->fonts[i], fd);
		ft_putchar_fd('\n', fd);
		i++;
	}
	if (prefix[line])
		ft_putendl_fd(prefix[line], fd);
	ft_putchar_fd('\n', fd);
	return (SUCCESS);
}

unsigned short			write_assets(int fd, t_media *media)
{
	int i = 0;

	if (!media)
		return (FAIL);
	while (i < TOTAL_SECTIONS)
	{
		write_section(fd, media, i);
		i++;
	}
	return (SUCCESS);
}

unsigned short			rewrite_levels(t_media *media)
{
	int					fd;
	int 				i;

	if (!media || !media->worlds)
		return (FAIL);
	i = -1;
	while (++i < media->n_worlds)
	{
		if (open_for_write(media->worlds[i].full_path, &fd) == FAIL)
		{
			ft_putstr("Couldn't open the file for writing: ");
			ft_putendl(media->worlds[i].full_path);
			return (FAIL);
		}
		if (write_level(fd, media->worlds[i]) == FAIL)
		{
			ft_putendl("error in saving progress\n");
			return (FAIL);
		}
		if (close_file(fd) == FAIL)
		{
			ft_putstr("Couldn't close the file: ");
			ft_putendl(media->worlds[i].full_path);
			return (FAIL);
		}
	}
	return (SUCCESS);
}

unsigned short			rewrite_media(t_media *media)
{
	int					fd;

	if (!media)
		return (FAIL);
	if (open_for_write(ASSET_FILE, &fd) == FAIL)
	{
		ft_putstr("Couldn't open the file for writing");
		ft_putstr(ASSET_FILE);
		ft_putstr(". Must be in the root folder and have write permissions.\n");
		return (FAIL);
	}
	if (write_assets(fd, media) == FAIL)
	{
		ft_putendl("error in writing assets\n");
		return (FAIL);
	}
	ft_putendl("Wrote assets\n");
	if (close_file(fd) == FAIL)
	{
		ft_putstr("Couldn't close the file");
		ft_putstr(ASSET_FILE);
		ft_putstr(".\n");
		return (FAIL);
	}
	rewrite_levels(media);
	return (SUCCESS);
}

