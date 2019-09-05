
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

unsigned short			write_level_section(int fd, t_world world, int section)
{
	static char 		title[5][9] = { "Textures", "Vectors", "Walls", "Sectors", "Player" };
	unsigned short		i;
	unsigned short		n;

	ft_putendl_fd(title[section], fd);
	if (section == 0)
		n = world.n_textures;
	else if (section == 1)
		n = world.n_vectors;
	else if (section == 2)
		n = world.n_walls;
	else if (section == 3)
		n = world.n_sectors;
	else if (section == 4)
		n = 2;
	else
		return (FAIL);
	i = 0;
	while (i < n)
	{
		ft_putnbr_fd(i, fd);
		ft_putstr_fd(") ", fd);
		if (section == 0)
			ft_putnbr_fd(world.textures[i], fd);
		else if (section == 1)
			{
				ft_putnbr_fd(world.vertices[i].x, fd);
				ft_putstr_fd(",", fd);
				ft_putnbr_fd(world.vertices[i].y, fd);
			}
		else if (section == 2)
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
		else if (section == 3)
		{
			ft_putstr_fd("floor(", fd);
			ft_putnbr_fd(world.sectors[i].floor, fd);
			ft_putstr_fd(" ", fd);
			ft_putnbr_fd(world.sectors[i].floor_txtr, fd);
			ft_putstr_fd(") ceil(", fd);
			ft_putnbr_fd(world.sectors[i].ceiling, fd);
			ft_putstr_fd(" ", fd);
			ft_putnbr_fd(world.sectors[i].ceil_txtr, fd);

			ft_putstr_fd(") walls '", fd);
			int j = 0;
			while (j < world.sectors[i].n_walls)
			{
				ft_putnbr_fd(world.sectors[i].sec_walls[j], fd);
				ft_putstr_fd(" ", fd);
				j++;
			}
			ft_putstr_fd("' items '' enemies ''", fd);
		}
		else if (section == 4)
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
	static char 		count[4][12] = { "Vectors: ", "Walls: ", "Sectors: ", "Textures: " };
	int i = 0;

	printf("n vectors %d\n",world.n_vectors );
	while (i < 4)
	{
		ft_putstr_fd("Count ", fd);
		ft_putstr_fd(count[i], fd);
		if (i == 0)
			ft_putnbr_fd(world.n_vectors, fd);
		else if (i == 1)
			ft_putnbr_fd(world.n_walls, fd);
		else if (i == 2)
			ft_putnbr_fd(world.n_sectors, fd);
		else if (i == 3)
			ft_putnbr_fd(world.n_textures, fd);
		ft_putchar_fd('\n', fd);
		i++;
	}
	ft_putchar_fd('\n', fd);
	i = 0;
	while (i < 5)
	{
		write_level_section(fd, world, i);
		i++;
	}
	return (SUCCESS);
}

unsigned short			write_section(int fd, t_media *media, int section)
{
	static char 		title[TOTAL_SECTIONS][9] = { "Levels", "Textures", "Sounds", "Fonts" };
	static char 		prefix[5][13] = {	"# ",
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
		if (line == 0)
			ft_putstr_fd(title[section], fd);
		else if (line == 1)
			ft_putstr_fd(media->paths[section], fd);
		else if (line == 2)
			ft_putstr_fd(media->extensions[section], fd);
		ft_putchar_fd('\n', fd);
		line++;
	}
	if (section == 0)
		n_files = media->n_worlds;
	else if (section == 1)
		n_files = media->n_textures;
	else if (section == 2)
		n_files = media->n_sounds;
	else if (section == 3)
		n_files = media->n_fonts;
	else
		return (FAIL);
	i = 0;
	while (i < n_files)
	{
		ft_putnbr_fd(i, fd);
		ft_putstr_fd(". ", fd);
		if (section == 0)
			ft_putstr_fd(media->worlds[i].filename, fd);
		else if (section == 1)
			ft_putstr_fd(media->txtrs[i].name, fd);
		else if (section == 2)
			ft_putstr_fd(media->sounds[i], fd);
		else if (section == 3)
			ft_putstr_fd(media->fonts[i], fd);
		ft_putchar_fd('\n', fd);
		i++;
	}
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

void					rewrite_levels(t_media *media)
{
	int					fd;
	int 				i;

	if (!media)
		return ;
	i = 0;
	while (i < media->n_worlds)
	{
		printf("writing file %s\n", ft_strjoin("./media/test/", media->worlds[i].filename) );
		if (open_for_write(ft_strjoin(ft_strjoin("./media/test/", media->worlds[i].filename), ".txt"), &fd) == FAIL)
		{
			ft_putstr("Couldn't open the file for writing");
			ft_putstr(ASSET_FILE);
			ft_putstr(". Must be in the root folder and have write permissions.\n");
			return ;
		}
		if (write_level(fd, media->worlds[i]) == FAIL)
		{
			ft_putendl("error in writing assets\n");
			return ;
		}
		ft_putendl("Wrote level\n");
		if (close_file(fd) == FAIL)
		{
			ft_putstr("Couldn't close the file");
			ft_putstr(ASSET_FILE);
			ft_putstr(".\n");
			return ;
		}
		i++;
	}
}

void					rewrite_media(t_media *media)
{
	int					fd;

	if (!media)
		return ;
	if (open_for_write("test.txt", &fd) == FAIL)
	{
		ft_putstr("Couldn't open the file for writing");
		ft_putstr(ASSET_FILE);
		ft_putstr(". Must be in the root folder and have write permissions.\n");
		return ;
	}
	if (write_assets(fd, media) == FAIL)
	{
		ft_putendl("error in writing assets\n");
		return ;
	}
	ft_putendl("Wrote assets\n");
	if (close_file(fd) == FAIL)
	{
		ft_putstr("Couldn't close the file");
		ft_putstr(ASSET_FILE);
		ft_putstr(".\n");
		return ;
	}
	rewrite_levels(media);
}
