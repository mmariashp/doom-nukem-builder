
#include "builder.h"

void 					free_prog(t_prog *prog, t_sdl *sdl)
{
	if (!prog)
		return ;
	if (prog->modes)
		free_modes(prog->modes, sdl);
	if (prog->screen)
		free_int_tab(prog->screen, WIN_W);
	free(prog);
	prog = NULL;
}

t_prog					*get_prog(void)
{
	t_prog				*prog;

	prog = (t_prog *)ft_memalloc(sizeof(t_prog));
	if (!prog)
		return (NULL);
	prog->screen = (int **)ft_memalloc(sizeof(int *) * WIN_W);
	if (!prog->screen)
		return (NULL);
	int k = 0;
	while (k < WIN_W)
	{
		prog->screen[k] = (int *)ft_memalloc(sizeof(int) * WIN_H);
		if (!prog->screen[k])
			return (NULL);
		ft_bzero(prog->screen[k], sizeof(prog->screen[k]));
		k++;
	}
	prog->mode_id = 0;
	prog->modes = NULL;
	prog->button_lit = -1;
	prog->button_on = -1;
	prog->zoom = 0;
	prog->move = (t_vec2d){ 0, 0 };
	prog->features[F_ZOOM] = 1;
	prog->features[F_MOVE_GRID] = 1;
	prog->features[F_SELECT_NODE] = 0;
	prog->features[F_REDRAW] = 1;
	prog->save = FALSE;
	return (prog);
}
