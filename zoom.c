
#include "builder.h"

void					get_min_scale(float *res_min_scale)
{
	static int 			min_size = -1;
	static float		min_scale = 1;

	if (min_size == -1)
	{
		if (WIN_H < WIN_W)
			min_size = WIN_H / 2;
		else
			min_size = WIN_W / 2;
		min_scale = (float)min_size / GRID_SIZE;
	}
	*res_min_scale = min_scale;
}

float					get_scale_to_sector(int size)
{
    float               scale;
    int                 box;

    box = get_min(WIN_W * 0.6, WIN_H * 0.6);
    scale = (float)box / size;
    return (scale);
}

void					zoom_displace(int *gridx, int *gridy, t_vec2d p, float old_scale, float new_scale)
{
	float				mapx;
	float				mapy;

	mapx = (float)(p.x - *gridx) / old_scale;
	mapy = (float)(p.y - *gridy) / old_scale;
	*gridx = (int)(p.x - (mapx * new_scale));
	*gridy = (int)(p.y - (mapy * new_scale));
}

void					zoom_grid(t_prog *prog, t_vec2d mouse, t_grid *grid)
{
	float				new;
	static float		min_scale = 1;
	float 				old_scale;

	if (!prog || !grid)
		return ;
	if (mouse_over(grid->box, mouse))
	{
		old_scale = grid->scale;
		if (min_scale <= 1)
			get_min_scale(&min_scale);
		new = grid->scale + grid->scale * 0.3f * prog->zoom;
		grid->scale = clamp_f(new, min_scale, 100);
		zoom_displace(&grid->box.x, &grid->box.y, mouse, old_scale, grid->scale );
		prog->features[F_REDRAW] = 1;
	}
	prog->zoom = 0;
}

void                    highlight_sector_nodes(t_vec2d *p, int n_walls, t_grid *grid)
{
    int                 i;

    i = 0;
    while (i < n_walls)
    {
        if (p[i].x >= 0 && p[i].x < GRID_SIZE && p[i].y >= 0 && p[i].y < GRID_SIZE)
            grid->nodes[p[i].x][p[i].y] = NODE_SECTOR;
        i++;
    }
}

void                    zoom_to_sector(t_sector *sector, t_vec2d *vertices, t_t *t, t_prog *prog)
{
    int					i = 0;
    t_vec2d				p[sector->n_walls];
    t_vec2d             min;
    t_vec2d             max;
    t_vec2d             center;
    static t_vec2d      desired = { WIN_W * 0.3, WIN_H * 0.55 };
    int                 n;

    if (!prog || !vertices)
        return ;
    max.x = 0;
    min.x = GRID_SIZE;
    max.y = 0;
    min.y = GRID_SIZE;
    while (i < sector->n_walls)
    {
        p[i] = vertices[sector->v[i]];
        draw_node(p[i], 15, BABY_PINK, prog->screen);
        if (p[i].x < min.x)
            min.x = p[i].x;
        if (p[i].x > max.x)
            max.x = p[i].x;
        if (p[i].y < min.y)
            min.y = p[i].y;
        if (p[i].y > max.y)
            max.y = p[i].y;
        i++;
    }
    highlight_sector_nodes((t_vec2d *)p, sector->n_walls, &t->grid);
    n = get_max(max.x - min.x, max.y - min.y);
    min.x = (int)(t->grid.box.x + min.x * t->grid.scale);
    min.y = (int)(t->grid.box.y + min.y * t->grid.scale);
    max.x = (int)(t->grid.box.x + max.x * t->grid.scale);
    max.y = (int)(t->grid.box.y + max.y * t->grid.scale);

    center = (t_vec2d){ min.x + (max.x - min.x) / 2, min.y + (max.y - min.y) / 2 };

    t->grid.box.x = (t->grid.box.x - center.x) + desired.x;
    t->grid.box.y = (t->grid.box.y - center.y) + desired.y;

    printf("size = %d, %f, %f\n",n, t->grid.scale, get_scale_to_sector(n));
    zoom_displace(&t->grid.box.x, &t->grid.box.y, desired, t->grid.scale, get_scale_to_sector(n) );
    t->grid.scale = get_scale_to_sector(n);
//    zoom_grid(prog, center, &t->grid);
}