
#include "builder.h"

SDL_Texture				**get_prog_txtr(SDL_Renderer *rend)
{
	int 				i;
	SDL_Texture			**t;
	char 				*tmp;
	char 				*tmp2;
	static char			name[TOTAL_TXTRS][30] = { "rect_gr", "rect_gr_l",\
	"rect_y", "rect_y_l", "panel_gr", "panel_gr_l", "yes", "yes_l", "no",\
	"no_l", "move", "move_l", "draw", "draw_l", "distort", "distort_l", "iso",\
	"iso_l", "sector", "sector_l", "wall", "wall_l", "player", "player_l",\
	"save", "save_l", "exit", "exit_l", "back", "back_l", "del", "del_l",\
	"edit", "edit_l", "up", "up_l", "down", "down_l", "coin", "key", "object",\
	"enemy", "super", "health", "ammo", "light" };

	if (!(t = ft_memalloc(sizeof(SDL_Texture *) * TOTAL_TXTRS)))
		return (NULL);
	i = 0;
	while (i < TOTAL_TXTRS)
	{
		if ((tmp = ft_strjoin("./buttons/", name[i])))
		{
			tmp2 = ft_strjoin(tmp, ".bmp");
			free(tmp);
			if (tmp2)
			{
				t[i] = load_texture(tmp2, rend, 0);
				free(tmp2);
				if (t[i])
				{
					i++;
					continue ;
				}
			}
		}
		return (NULL);
	}
	return (t);
}

void					free_prog_txtr(SDL_Texture **t)
{
	int 				i;

	if (t)
	{
		i = 0;
		while (i < TOTAL_TXTRS)
		{
			if (t[i])
				SDL_DestroyTexture(t[i]);
			i++;
		}
		free(t);
	}
}

void 					free_prog(t_prog *prog)
{
	if (!prog)
		return ;
	if (prog->modes)
		free_modes(prog->modes);
	if (prog->screen)
		free_int_tab(prog->screen, WIN_W);
	if (prog->t)
		free_prog_txtr(prog->t);
	free(prog);
	prog = NULL;
}

t_prog					*get_prog(SDL_Renderer *rend)
{
	t_prog				*prog;

	prog = (t_prog *)ft_memalloc(sizeof(t_prog));
	if (!prog || !rend)
		return (NULL);
	ft_bzero(prog, sizeof(t_prog));
	prog->screen = (int **)ft_memalloc(sizeof(int *) * WIN_W);
	if (!prog->screen)
		return (NULL);
	int k = 0;
	while (k < WIN_W)
	{
		prog->screen[k] = (int *)ft_memalloc(sizeof(int) * WIN_H);
		if (!prog->screen[k])
			return (NULL);
		ft_bzero(prog->screen[k], sizeof(int) * WIN_H);
		k++;
	}
	prog->mode_id = 0;
	prog->last_mode_id = 0;
	prog->modes = NULL;
	prog->btn_lit = -1;
	prog->btn_on = -1;
	prog->zoom = 0;
	prog->click = (t_vec2d){ 0, 0 };
    prog->move = (t_vec2d){ 0, 0 };
	prog->features[F_ZOOM] = 1;
	prog->features[F_MOVE_GRID] = 1;
	prog->features[F_SELECT_NODE] = 0;
	prog->features[F_REDRAW] = 1;
	prog->t = NULL;
	prog->t = get_prog_txtr(rend);
	if (!prog->t)
	{
		return (NULL);
	}
	return (prog);
}
