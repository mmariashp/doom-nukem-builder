
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
		if (min_scale == 1)
			get_min_scale(&min_scale);
		new = grid->scale + grid->scale * 0.3f * prog->zoom;
		grid->scale = clamp_f(new, min_scale, 100);
		zoom_displace(&grid->box.x, &grid->box.y, mouse, old_scale, grid->scale );
		prog->features[F_REDRAW] = 1;
	}
	prog->zoom = 0;
}