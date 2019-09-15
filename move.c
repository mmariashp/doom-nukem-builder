
#include "builder.h"

void					move_grid_drag(t_prog *prog, t_vec2d mouse, t_grid *grid)
{
	if (!prog || !grid)
		return ;
	if (mouse_over(grid->box, mouse))
	{
		grid->box.x += mouse.x - prog->click.x;
		grid->box.y += mouse.y - prog->click.y;
		prog->click = mouse;
		prog->features[F_REDRAW] = 1;
	}
	else
		prog->click = (t_vec2d){ 0, 0 };
}

void					move_grid_keys(t_prog *prog, t_grid *grid)
{
	if (!prog || !grid)
		return ;
	grid->box.x += prog->move.x;
	grid->box.y += prog->move.y;
	prog->move = (t_vec2d){ 0, 0 };
	prog->features[F_REDRAW] = 1;
}