
#include "builder.h"

t_vec					transform_to_screen(t_vec old, t_grid *grid)
{
	t_vec				res;

	res.x = (int)((float)grid->box.x + (float)old.x * grid->scl);
	res.y = (int)((float)grid->box.y + (float)old.y * grid->scl);
	return (res);
}

t_vec					transform_to_grid(t_vec old, t_grid *grid)
{
	t_vec				res;

	res.x = (int)((float)grid->box.x + (float)old.x * grid->scl);
	res.y = (int)((float)grid->box.y + (float)old.y * grid->scl);
	return (res);
}