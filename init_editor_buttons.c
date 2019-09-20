
#include "builder.h"

t_mode					*init_sector_buttons(t_mode *m, SDL_Renderer *rend)
{
	static char 		path_items[10] = "./items3/";
	static char 		path_buttons[10] = "./";
	static char 		ext[5] = ".png";
	static char         reg[17][15] = { "cross2", "up2", "down2",\
						"up2", "down2", "edit", "edit", "edit", "trash", \
						"coin", \
						"key", \
						"object", \
						"enemy", \
						"super_bonus", \
						"health", \
						"ammo", \
						"light" };

	static char         lit[17][15] = { "cross3", "up3", "down3",\
						"up3", "down3", "editlit", "editlit",  "editlit", "trash3", \
						"coin", \
						"key", \
						"object", \
						"enemy", \
						"super_bonus", \
						"health", \
						"ammo", \
						"light" };
	int                 i;

	if (!m || !rend || !(m->buttons = init_buttons(17)))
		return (NULL);
	m->n_buttons = 17;
	i = -1;
	while (++i < m->n_buttons)
	{
		if (!reg[i] || !lit[i])
			continue ;
		if (i < B_COIN)
		{
			m->buttons[i].txtr = load_texture(get_full_path(reg[i], ext, path_buttons), rend, 0);
			m->buttons[i].lit = load_texture(get_full_path(lit[i], ext, path_buttons), rend, 0);
		}
		else
		{
			m->buttons[i].txtr = load_texture(get_full_path(reg[i], ext, path_items), rend, 0);
			m->buttons[i].lit = load_texture(get_full_path(lit[i], ext, path_items), rend, 0);
		}

	}
	m->buttons[DESELECT_BTN].box = layout_menu(0, 0);
	m->buttons[DESELECT_BTN].box = (t_rec){ m->buttons[DESELECT_BTN].box.x +\
	m->buttons[DESELECT_BTN].box.w, m->buttons[DESELECT_BTN].box.y, 30, 30 };
	m->buttons[F_UP_BTN].box = layout_menu(3, 1);
	m->buttons[F_DOWN_BTN].box = layout_menu(4, 1);
	m->buttons[C_UP_BTN].box = layout_menu(3, 2);
	m->buttons[C_DOWN_BTN].box = layout_menu(4, 2);
	m->buttons[FT_EDIT_BTN].box = layout_menu(6, 3);
	m->buttons[CT_EDIT_BTN].box = layout_menu(6, 4);

	t_rec box = layout_menu(2, 6);
	box.w *= 1.27;
	box.h *= 1.19;
	distribute_buttons_h(m->buttons, B_COIN, B_LIGHT + 1, box, 8);
	m->buttons[B_ITEM_EDIT].vis_lit_on[0] = FALSE;
	m->buttons[B_ITEM_DEL].vis_lit_on[0] = FALSE;
	return (m);
}

t_mode					*init_wall_buttons(t_mode *m, SDL_Renderer *rend)
{
	static char         reg[4][15] = { "cross2.png", "edit.png", "edit.png",\
						"edit.png" };
	static char         lit[4][15] = { "cross3.png", "editlit.png",\
						"editlit.png", "editlit.png" };
	int                 i;

	if (!m || !rend || !(m->buttons = init_buttons(4)))
		return (NULL);
	m->n_buttons = 4;
	i = -1;
	while (++i < m->n_buttons)
	{
		if (reg[i])
			m->buttons[i].txtr = load_texture(reg[i], rend, 0);
		if (lit[i])
			m->buttons[i].lit = load_texture(lit[i], rend, 0);
	}
	m->buttons[DESELECT_BTN].box = layout_menu(0, 0);
	m->buttons[DESELECT_BTN].box = (t_rec){ m->buttons[DESELECT_BTN].box.x +\
	m->buttons[DESELECT_BTN].box.w, m->buttons[DESELECT_BTN].box.y, 30, 30 };
	m->buttons[WT_EDIT_BTN].box = layout_menu(6, 1);
	m->buttons[W_PORTAL_BTN].box = layout_menu(6, 2);
	m->buttons[W_DOOR_BTN].box = layout_menu(6, 3);
	return (m);
}

t_mode					*init_regular_buttons(t_mode *mode, SDL_Renderer *rend)
{
	static t_rec		box = { 10, 0, WIN_H * 0.07 * 9, WIN_H * 0.07};
	static char         reg[9][15] = { "move2.png", "add2.png", "distort2.png",\
						"delete2.png", "sector22.png", "wall2.png",\
						"player2.png", "save2.png", "back22.png" };
	static char         lit[9][15] = { "move3.png", "add3.png", "distort3.png",\
    					"delete3.png", "sector3.png", "wall3.png",\
						"player3.png", "save3.png", "back3.png" };
	int                 i;

	if (!mode || !rend)
		return (NULL);
	mode->n_buttons = 9;
	if (!(mode->buttons = init_buttons(mode->n_buttons)))
		return (NULL);
	distribute_buttons_h(mode->buttons, 0,  mode->n_buttons, box, 3);
	i = 0;
	while (i < mode->n_buttons)
	{
		if (reg[i])
			mode->buttons[i].txtr = load_texture(reg[i], rend, 0);
		if (lit[i])
			mode->buttons[i].lit = load_texture(lit[i], rend, 0);
		i++;
	}
	return (mode);
}

void					get_buttons(int state, t_mode *mode, SDL_Renderer *rend)
{
	if (!mode)
		return ;
	if (state == SECTOR_EDIT)
	{
		mode = init_sector_buttons(mode, rend);
	}
	else if (state == WALL_EDIT)
	{
		mode = init_wall_buttons(mode, rend);
	}
	else
	{
		mode = init_regular_buttons(mode, rend);
	}
}