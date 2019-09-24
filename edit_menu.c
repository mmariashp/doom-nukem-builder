
#include "builder.h"

t_rec                    layout_menu(char i, char n)
{
	static t_rec        out = { WIN_W * 0.6, WIN_H / 10, WIN_W * 0.34, WIN_W * 0.34 };
	static t_rec        l = { WIN_W * 0.62, WIN_H * 0.12, WIN_W * 0.24, WIN_H * 0.03 };
	static t_rec        v = { WIN_W * 0.87, WIN_H * 0.12, WIN_W * 0.04, WIN_H * 0.03 };
	int 				arrow;

	arrow = i == 3 ? -10 : 10;
	if (i == 0)
		return (out);
	else if (i == 2) //lines like height
	{
		if (n == 0)
			return ((t_rec){ l.x, l.y + n * (l.h + 20), WIN_W * 0.3, WIN_H * 0.05 });
		return ((t_rec){ l.x, l.y + n * (l.h + 20) + 10, l.w, l.h });
	}
	else if (i == 5) // values column
	{
		if (n == 0)
			return ((t_rec){ v.x, (v.y + n * (v.h + 20)), v.w, WIN_H * 0.05 });
		return ((t_rec){ v.x, (v.y + n * (v.h + 20) + 10), v.w, v.h });
	}
	arrow = i == 6 ? 0 : arrow;
	return ((t_rec){ out.x + out.w * 0.9, (l.y + n * (l.h + 20) + 10) + arrow,
				  l.h * 0.9, l.h * 0.9 });
}

char 					*menu_lines(int id, int i)
{
	static short 		n_s_lines = 6;
	static short 		n_w_lines = 4;
	static char         sector_lines[6][20] = { "SECTOR " ,
												"Floor height ",
											 	"Ceiling height ",
											 	"Floor texture ",
											 	"Ceiling texture ", "Items" };
	static char         wall_lines[4][20] = { "WALL ", "Texture ",
										   "Portal ", "Door " };

	if (id == SECTOR_EDIT && within(i, -1, n_s_lines))
		return (sector_lines[i]);
	else if (id == WALL_EDIT && within(i, -1, n_w_lines))
		return (wall_lines[i]);
	else
		return (NULL);
}

t_value					*get_sec_values(int *n, t_sec *sector)
{
	static int 			nb = 6;
	t_value				*new;

	if (!sector)
		return (NULL);
	if (!(new = init_values(nb)))
		return (NULL);
	*n = nb;
	new[1].text = ft_itoa(sector->floor);
	new[2].text = ft_itoa(sector->ceiling);
	new[3].t_id = sector->floor_txtr;
	new[4].t_id = sector->ceil_txtr;
	new[5].text = ft_itoa(sector->n_items);
	new[3].media_prog = 0;
	new[4].media_prog = 0;
	if (!new[1].text || !new[2].text || !new[5].text)
		return (NULL);
	return (new);
}

t_value					*get_wall_values(int *n, t_wall *wall)
{
	static int 			nb = 4;
	t_value				*new;

	if (!wall)
		return (NULL);
	if (!(new = init_values(nb)))
		return (NULL);
	*n = nb;
	new[1].t_id = wall->txtr;
	new[2].t_id = wall->type == WALL_EMPTY ? TXTR_YES : TXTR_NO;
	new[3].t_id = wall->door > -1 ? TXTR_YES : TXTR_NO;
	new[1].media_prog = 0;
	new[2].media_prog = 1;
	new[3].media_prog = 1;
	return (new);
}

void					render_edit_menu(SDL_Renderer *r, t_texture *txtrs, t_world *w, int state, int n_txtrs, SDL_Texture **t)
{
	t_value             *values;
	int 				i;
	int                 n;

	if (!r || !txtrs || !w || (state != SECTOR_EDIT && state != WALL_EDIT))
		return ;
	values = NULL;
	n = 0;
	i = state == SECTOR_EDIT ? select_it(1, S_SELECT, -1) : select_it(1, W_SELECT, -1);
	if (state == SECTOR_EDIT && within(i, -1, w->n_sec) &&
	within(w->sec[i].floor_txtr, -1, n_txtrs) &&
	within(w->sec[i].ceil_txtr, -1, n_txtrs))
	{
		if (!(values = get_sec_values(&n, &w->sec[i])))
			return ;
	}
	else if (state == WALL_EDIT && (!within(i, -1, w->n_walls) || !within(\
	w->walls[i].txtr, -1, n_txtrs) || !(values = get_wall_values(&n, &w->walls[i]))))
		return ;
	values[0].text = ft_itoa(i);
	if (within(TXTR_PANEL_GR, -1, TOTAL_TXTRS) && t[TXTR_PANEL_GR])
		render_box(layout_menu(0, 0), t[TXTR_PANEL_GR], r);
	render_values(state, n, values, r, t, txtrs, n_txtrs);
	free_values(values, n);
}

