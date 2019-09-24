
#include "builder.h"

t_mode					*init_sec_btn(t_mode *m)
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

	if (!m || !(m->btn = init_btn(18)))
		return (NULL);
	m->n_btn = 18;
	i = -1;
	while (++i < m->n_btn)
	{
		m->btn[i].reg_i = reg[i];
		m->btn[i].lit_i = lit[i];
	}
	m->btn[DESELECT_BTN].box = layout_menu(0, 0);
	m->btn[DESELECT_BTN].box = (t_rec){ m->btn[DESELECT_BTN].box.x +\
	m->btn[DESELECT_BTN].box.w, m->btn[DESELECT_BTN].box.y, 30, 30 };
	m->btn[DEL_SEC_BTN].box = m->btn[DESELECT_BTN].box;
	m->btn[DEL_SEC_BTN].box.y += m->btn[DEL_SEC_BTN].box.h;
	m->btn[F_UP_BTN].box = layout_menu(3, 1);
	m->btn[F_DOWN_BTN].box = layout_menu(4, 1);
	m->btn[C_UP_BTN].box = layout_menu(3, 2);
	m->btn[C_DOWN_BTN].box = layout_menu(4, 2);
	m->btn[FT_EDIT_BTN].box = layout_menu(6, 3);
	m->btn[CT_EDIT_BTN].box = layout_menu(6, 4);

	t_rec box = layout_menu(2, 6);
	box.w *= 1.27;
	box.h *= 1.19;
	distribute_btn_h(m->btn, B_COIN, B_LIGHT + 1, box, 8);
	m->btn[B_ITEM_EDIT].vis_lit_on[0] = FALSE;
	m->btn[B_ITEM_DEL].vis_lit_on[0] = FALSE;
	return (m);
}

t_mode					*init_wall_btn(t_mode *m)
{
	static int			reg[4] = { TXTR_BACK, TXTR_EDIT, TXTR_EDIT, TXTR_EDIT };
	static int			lit[4] = { TXTR_BACK_L, TXTR_EDIT_L, TXTR_EDIT_L, TXTR_EDIT_L };
	int                 i;

	if (!m || !(m->btn = init_btn(4)))
		return (NULL);
	m->n_btn = 4;
	i = -1;
	while (++i < m->n_btn)
	{
		m->btn[i].reg_i = reg[i];
		m->btn[i].lit_i = lit[i];
	}
	m->btn[DESELECT_BTN].box = layout_menu(0, 0);
	m->btn[DESELECT_BTN].box = (t_rec){ m->btn[DESELECT_BTN].box.x +\
	m->btn[DESELECT_BTN].box.w, m->btn[DESELECT_BTN].box.y, 30, 30 };
	m->btn[WT_EDIT_BTN].box = layout_menu(6, 1);
	m->btn[W_PORTAL_BTN].box = layout_menu(6, 2);
	m->btn[W_DOOR_BTN].box = layout_menu(6, 3);
	return (m);
}

t_mode					*init_regular_btn(t_mode *m)
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
	m->n_btn = 9;
	if (!(m->btn = init_btn(m->n_btn)))
		return (NULL);
	distribute_btn_h(m->btn, 0, m->n_btn, box, 3);
	i = 0;
	while (i < m->n_btn)
	{
		m->btn[i].reg_i = reg[i];
		m->btn[i].lit_i = lit[i];
		i++;
	}
	return (m);
}

void					get_btn(int state, t_mode *mode)
{
	if (!mode)
		return ;
	if (state == SECTOR_EDIT)
		mode = init_sec_btn(mode);
	else if (state == WALL_EDIT)
		mode = init_wall_btn(mode);
	else
		mode = init_regular_btn(mode);
}