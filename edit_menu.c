
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

t_value					*get_sector_values(int *n, t_sector *sector, SDL_Texture *f_texture, SDL_Texture *c_texture)
{
	static int 			nb = 6;
	t_value				*new;

	if (!sector || !f_texture || !c_texture)
		return (NULL);
	if (!(new = init_values(nb)))
		return (NULL);
	*n = nb;
	new[1].text = ft_itoa(sector->floor);
	new[2].text = ft_itoa(sector->ceiling);
	new[3].texture = f_texture;
	new[4].texture = c_texture;
	new[5].text = ft_itoa(sector->n_items);
	if (!new[1].text || !new[2].text || !new[5].text || !new[3].texture
	|| !new[4].texture)
		return (NULL);
	return (new);
}

t_value					*get_wall_values(int *n, t_wall *wall, SDL_Renderer *rend, SDL_Texture *wall_texture)
{
	static int 			nb = 4;
	t_value				*new;

	if (!wall || !rend || !wall_texture)
		return (NULL);
	if (!(new = init_values(nb)))
		return (NULL);
	*n = nb;
	new[1].texture = wall_texture;
	new[2].texture = wall->type == WALL_EMPTY ?\
		load_texture("yes.png", rend, 0) : load_texture("no.png", rend, 0);
	new[3].texture = wall->door > -1 ?\
		load_texture("yes.png", rend, 0) : load_texture("no.png", rend, 0);
	if (!new[1].texture || !new[2].texture || !new[3].texture)
		return (NULL);
	return (new);
}

void					render_edit_menu(SDL_Renderer *r, t_texture *txtrs, t_world *w, int state, int n_txtrs)
{
	t_value             *values;
	int 				i;
	int                 n;
	int 				f;
	int 				c;

	if (!r || !txtrs || !w || (state != SECTOR_EDIT && state != WALL_EDIT))
		return ;
	values = NULL;
	n = 0;
	i = state == SECTOR_EDIT ? selected_item(1, S_SELECT, -1)
			: selected_item(1, W_SELECT, -1);
	if (state == SECTOR_EDIT && within(i, -1, w->n_sec) &&
	within(w->sec[i].floor_txtr, -1, n_txtrs) &&
	within(w->sec[i].ceil_txtr, -1, n_txtrs))
	{
		f = w->sec[i].floor_txtr;
		c = w->sec[i].ceil_txtr;
		if (!(values = get_sector_values(&n, &w->sec[i], txtrs[f].sdl_t, txtrs[c].sdl_t)))
			return ;
	}
	else if (state == WALL_EDIT && (!within(i, -1, w->n_walls) || !within(\
	w->walls[i].txtr, -1, n_txtrs) || !(values = get_wall_values(&n, \
	&w->walls[i], r, txtrs[w->walls[i].txtr].sdl_t))))
		return ;
	values[0].text = ft_itoa(i);
	render_box(layout_menu(0, 0), button_back(2, 1, r), r);
	render_values(state, n, values, r);
	free_values(values, n);
}

