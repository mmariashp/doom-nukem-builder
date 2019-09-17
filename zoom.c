
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

float					get_scale_to_sector(int size, int max_w, int max_h)
{
    float               scale;
    int                 box;

    box = get_min(max_w, max_h);
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

void                    zoom_to_sector(t_sector *sector, t_vec2d *vertices, t_grid *grid, t_prog *prog)
{
    int					i;
    t_vec2d				p[sector->n_walls];
    t_vec2d             min;
    t_vec2d             max;
    t_vec2d             center;
    static t_vec2d      desired = { WIN_W * 0.3, WIN_H * 0.5 };
    int                 n;

    if (!prog || !vertices)
        return ;
    max.x = 0;
    min.x = GRID_SIZE;
    max.y = 0;
    min.y = GRID_SIZE;
    i = -1;
    while (++i < sector->n_v)
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
    }
    highlight_sector_nodes((t_vec2d *)p, sector->n_walls, grid);
    n = get_max(max.x - min.x, max.y - min.y);
    min.x = (int)(grid->box.x + min.x * grid->scale);
    min.y = (int)(grid->box.y + min.y * grid->scale);
    max.x = (int)(grid->box.x + max.x * grid->scale);
    max.y = (int)(grid->box.y + max.y * grid->scale);
    center = (t_vec2d){ min.x + (max.x - min.x) / 2, min.y + (max.y - min.y) / 2 };
    grid->box.x = (grid->box.x - center.x) + desired.x;
    grid->box.y = (grid->box.y - center.y) + desired.y;
    zoom_displace(&grid->box.x, &grid->box.y, desired, grid->scale, get_scale_to_sector(n, WIN_W * 0.6, WIN_H * 0.6) );
    grid->scale = get_scale_to_sector(n, WIN_W * 0.6, WIN_H * 0.6);
}

void                    zoom_to_wall(t_vec2d v1, t_vec2d v2, t_grid *grid, t_prog *prog)
{
	t_vec2d				p[2];
	t_vec2d             min;
	t_vec2d             max;
	t_vec2d             center;
	static t_vec2d      desired = { WIN_W * 0.3, WIN_H * 0.5 };
	int                 n;

	if (!prog || !grid)
		return ;
	max.x = 0;
	min.x = GRID_SIZE;
	max.y = 0;
	min.y = GRID_SIZE;

		p[0] = v1;
		draw_node(p[0], 15, BABY_PINK, prog->screen);
		if (p[0].x < min.x)
			min.x = p[0].x;
		if (p[0].x > max.x)
			max.x = p[0].x;
		if (p[0].y < min.y)
			min.y = p[0].y;
		if (p[0].y > max.y)
			max.y = p[0].y;

	p[1] = v2;
	draw_node(p[1], 15, BABY_PINK, prog->screen);
	if (p[1].x < min.x)
		min.x = p[1].x;
	if (p[1].x > max.x)
		max.x = p[1].x;
	if (p[1].y < min.y)
		min.y = p[1].y;
	if (p[1].y > max.y)
		max.y = p[1].y;

	n = get_max(max.x - min.x, max.y - min.y);
	min.x = (int)(grid->box.x + min.x * grid->scale);
	min.y = (int)(grid->box.y + min.y * grid->scale);
	max.x = (int)(grid->box.x + max.x * grid->scale);
	max.y = (int)(grid->box.y + max.y * grid->scale);
	center = (t_vec2d){ min.x + (max.x - min.x) / 2, min.y + (max.y - min.y) / 2 };
	grid->box.x = (grid->box.x - center.x) + desired.x;
	grid->box.y = (grid->box.y - center.y) + desired.y;
	zoom_displace(&grid->box.x, &grid->box.y, desired, grid->scale, get_scale_to_sector(n, WIN_W * 0.6, WIN_H * 0.6) );
	grid->scale = get_scale_to_sector(n, WIN_W * 0.6, WIN_H * 0.6);
}

void                    zoom_to_map(int n_vectors, t_vec2d *v, t_grid *grid)
{
    int                 i;
    int                 n;
    t_vec2d             min;
    t_vec2d             max;
    t_vec2d             center;
    static t_vec2d      desired = { WIN_W / 2, WIN_H / 2 };

    i = 0;
    max.x = 0;
    min.x = GRID_SIZE;
    max.y = 0;
    min.y = GRID_SIZE;
    while (i < n_vectors)
    {
        if (v[i].x < min.x)
            min.x = v[i].x;
        if (v[i].x > max.x)
            max.x = v[i].x;
        if (v[i].y < min.y)
            min.y = v[i].y;
        if (v[i].y > max.y)
            max.y = v[i].y;
        i++;
    }
    n = get_max(max.x - min.x, max.y - min.y);
    min.x = (int)(grid->box.x + min.x * grid->scale);
    min.y = (int)(grid->box.y + min.y * grid->scale);
    max.x = (int)(grid->box.x + max.x * grid->scale);
    max.y = (int)(grid->box.y + max.y * grid->scale);
    center = (t_vec2d){ min.x + (max.x - min.x) / 2, min.y + (max.y - min.y) / 2 };
    grid->box.x = (grid->box.x - center.x) + desired.x;
    grid->box.y = (grid->box.y - center.y) + desired.y;
    zoom_displace(&grid->box.x, &grid->box.y, desired, grid->scale, get_scale_to_sector(n, WIN_W * 0.9, WIN_H * 0.9) );
    grid->scale = get_scale_to_sector(n, WIN_W * 0.9, WIN_H * 0.9);
}