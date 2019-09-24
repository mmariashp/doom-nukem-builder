
#include "builder.h"

t_mode					*init_sec_buttons(t_mode *m)
{
	static int			reg[18] = { TXTR_BACK, TXTR_DEL, TXTR_UP, TXTR_DOWN,\
						TXTR_UP, TXTR_DOWN, TXTR_EDIT, TXTR_EDIT, TXTR_EDIT, TXTR_DEL, \
						TXTR_COIN,
						TXTR_KEY,
						TXTR_OBJECT,
						TXTR_ENEMY,
						TXTR_SUPER,
						TXTR_HEALTH,
						TXTR_AMMO,
						TXTR_LIGHT };

	static int         lit[18] = { TXTR_BACK_L, TXTR_DEL_L, TXTR_UP_L, TXTR_DOWN_L,\
						TXTR_UP_L, TXTR_DOWN_L, TXTR_EDIT_L, TXTR_EDIT_L, TXTR_EDIT_L, TXTR_DEL_L, \
						TXTR_COIN,
						TXTR_KEY,
						TXTR_OBJECT,
						TXTR_ENEMY,
						TXTR_SUPER,
						TXTR_HEALTH,
						TXTR_AMMO,
						TXTR_LIGHT };
	int                 i;

	if (!m || !(m->buttons = init_buttons(18)))
		return (NULL);
	m->n_buttons = 18;
	i = -1;
	while (++i < m->n_buttons)
	{
		m->buttons[i].reg_i = reg[i];
		m->buttons[i].lit_i = lit[i];
	}
	m->buttons[DESELECT_BTN].box = layout_menu(0, 0);
	m->buttons[DESELECT_BTN].box = (t_rec){ m->buttons[DESELECT_BTN].box.x +\
	m->buttons[DESELECT_BTN].box.w, m->buttons[DESELECT_BTN].box.y, 30, 30 };
	m->buttons[DEL_SEC_BTN].box = m->buttons[DESELECT_BTN].box;
	m->buttons[DEL_SEC_BTN].box.y += m->buttons[DEL_SEC_BTN].box.h;
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

t_mode					*init_wall_buttons(t_mode *m)
{
	static int			reg[4] = { TXTR_BACK, TXTR_EDIT, TXTR_EDIT, TXTR_EDIT };
	static int			lit[4] = { TXTR_BACK_L, TXTR_EDIT_L, TXTR_EDIT_L, TXTR_EDIT_L };
	int                 i;

	if (!m || !(m->buttons = init_buttons(4)))
		return (NULL);
	m->n_buttons = 4;
	i = -1;
	while (++i < m->n_buttons)
	{
		m->buttons[i].reg_i = reg[i];
		m->buttons[i].lit_i = lit[i];
	}
	m->buttons[DESELECT_BTN].box = layout_menu(0, 0);
	m->buttons[DESELECT_BTN].box = (t_rec){ m->buttons[DESELECT_BTN].box.x +\
	m->buttons[DESELECT_BTN].box.w, m->buttons[DESELECT_BTN].box.y, 30, 30 };
	m->buttons[WT_EDIT_BTN].box = layout_menu(6, 1);
	m->buttons[W_PORTAL_BTN].box = layout_menu(6, 2);
	m->buttons[W_DOOR_BTN].box = layout_menu(6, 3);
	return (m);
}

t_mode					*init_regular_buttons(t_mode *m)
{
	static t_rec		box = { 10, 0, WIN_H * 0.07 * 9, WIN_H * 0.07};
	static int			reg[9] = { TXTR_MOVE, TXTR_DRAW, TXTR_DISTORT, \
						TXTR_ISO, TXTR_SECTOR, TXTR_WALL, TXTR_PLAYER, \
						TXTR_SAVE, TXTR_EXIT };
	static int 			lit[9] = { TXTR_MOVE_L, TXTR_DRAW_L, TXTR_DISTORT_L, \
						TXTR_ISO_L, TXTR_SECTOR_L, TXTR_WALL_L, TXTR_PLAYER_L, \
						TXTR_SAVE_L, TXTR_EXIT_L };
	int                 i;

	if (!m)
		return (NULL);
	m->n_buttons = 9;
	if (!(m->buttons = init_buttons(m->n_buttons)))
		return (NULL);
	distribute_buttons_h(m->buttons, 0, m->n_buttons, box, 3);
	i = 0;
	while (i < m->n_buttons)
	{
		m->buttons[i].reg_i = reg[i];
		m->buttons[i].lit_i = lit[i];
		i++;
	}
	return (m);
}

void					get_buttons(int state, t_mode *mode)
{
	if (!mode)
		return ;
	if (state == SECTOR_EDIT)
		mode = init_sec_buttons(mode);
	else if (state == WALL_EDIT)
		mode = init_wall_buttons(mode);
	else
		mode = init_regular_buttons(mode);
}