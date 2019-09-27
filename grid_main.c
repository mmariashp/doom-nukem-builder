
#include "builder.h"

void					clean_grid(t_grid *grid)
{
	int					i;
	int					j;

	if (!grid)
		return ;
	i = 0;
	while (i < GRID_SIZE)
	{
		j = 0;
		while (j < GRID_SIZE)
			grid->nodes[i][j++] = NODE_EMPTY;
		i++;
	}
}

t_grid                  *get_grid(void)
{
	t_grid					*grid;

	if (!(grid = (t_grid *)ft_memalloc(sizeof(t_grid))))
		return (NULL);
	grid->scale = WIN_H / GRID_SIZE;
	grid->box.w = GRID_SIZE * grid->scale;
	grid->box.h = GRID_SIZE * grid->scale;
	grid->box.x = (WIN_W - grid->box.w) / 2;
	grid->box.y = (WIN_H - grid->box.h) / 2;
	grid->p[0] = (t_vec2d){ -1, -1 };
	grid->p[1] = (t_vec2d){ -1, -1 };
	clean_grid(grid);
	return (grid);
}

void					grid_refresh(t_grid *grid, t_media *media, int state, int sector)
{
	if (!grid || !media)
		return ;
	clean_grid(grid);
	fill_grid_walls(media->worlds[media->w_id].n_walls,\
	media->worlds[media->w_id].walls,\
	media->worlds[media->w_id].n_vecs,\
	media->worlds[media->w_id].vecs, grid);
	fill_grid(media->worlds[media->w_id].n_vecs,\
	media->worlds[media->w_id].vecs, grid);
	if (state == SEC_EDIT && within(sector, -1, media->worlds[media->w_id].n_sec))
		fill_grid_items(&media->worlds[media->w_id].sec[sector], grid);
}