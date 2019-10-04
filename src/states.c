
#include "builder.h"

void					normal_st(t_prog *prog, t_vec mouse, t_grid *grid, \
																t_world *world)
{
	if (!prog || select_it(1, ST_SEL, -1) != NORMAL)
		return ;
	select_it(0, W_SELECT, -1);
	lit_it(0, W_SELECT, -1);
	select_it(0, S_SELECT, -1);
	lit_it(0, S_SELECT, -1);
	if (prog->btn_on == DRAW_BTN)
	{
		drawing(world, prog, grid, mouse);
		upd_sec(world->sec, world->walls, world->vecs, world->n_s);
	}
	else if (prog->btn_on == DISTORT_BTN )
	{
		move_vector(prog, mouse, grid, world);
		upd_sec(world->sec, world->walls, world->vecs, world->n_s);
	}
	else if (prog->btn_on == PLAYER_BTN )
		move_player(prog, mouse, grid, world);
	else if (prog->btn_on == DRAG_BTN && (prog->click.x || prog->click.y))
		move_grid_drag(prog, mouse, grid);
	else if (prog->btn_on == ISO_BTN && (prog->click.x || prog->click.y))
		move_grid_drag(prog, mouse, grid);
}

void					setup_door(t_world *world)
{
	if (!world || world->n_s < 2)
		return ;
	if (world->sec[world->n_s - 1].is_door == TRUE && 
	world->sec[world->n_s - 1].n_w == 4)
	{
		if (within(world->sec[world->n_s - 1].s_walls[0], -1, world->n_w))
			world->walls[world->sec[world->n_s - 1].s_walls[0]].type = \
			WALL_EMPTY;
		if (within(world->sec[world->n_s - 1].s_walls[2], -1, world->n_w))
			world->walls[world->sec[world->n_s - 1].s_walls[2]].type = \
			WALL_EMPTY;
	}
}

unsigned short			circle_inter_line(t_vec mouse, int radius, t_line line)
{
	t_vec_f				d;
	float				a;
	float				b;
	float				c;
	float				det;

	d.x = line.p1.x - line.p0.x;
	d.y = line.p1.y - line.p0.y;

	a = d.x * d.x + d.y * d.y;
	b = 2 * (d.x * (line.p0.x - mouse.x) + d.y * (line.p0.y - mouse.y));
	c = (line.p0.x - mouse.x) * (line.p0.x - mouse.x) +
		(line.p0.y - mouse.y) * (line.p0.y - mouse.y) -
		radius * radius;
	det = b * b - 4 * a * c;
	if (a <= 0.0000001 || det < 0 || (!within(mouse.x, \
	get_min(line.p0.x, line.p1.x), get_max(line.p0.x, line.p1.x)) && 
	!within(mouse.y, get_min(line.p0.y, line.p1.y), \
	get_max(line.p0.y, line.p1.y))))
		return (0);
	else if (det == 0)
		return (1);
	else
		return (2);
}

void					wall_search_st(t_prog *prog, t_vec node, \
											t_grid *g, t_world *world)
{
	int					v1;
	int					v2;
	int 				w;

	if (!prog || !world || !g || !(mouse_over((t_rec){ 0, 0, W_W, W_H }, node)))
		return ;
	w = prog->screen[node.x][node.y].is == SCREEN_WALL ? lit_it(0, W_SELECT, \
	prog->screen[node.x][node.y].n) : -1;
	prog->redraw = 1;
	if (!((prog->click.x || prog->click.y) && within(w, -1, world->n_w)))
		return ;
	select_it(0, W_SELECT, w);
	if (prog->btn_on == DOOR_ADD_BTN)
	{
		add_door(world, w, g);
		setup_door(world);
	}
	else
	{
		select_it(0, ST_SEL, WALL_EDIT);
		if (within((v1 = world->walls[w].v1), -1, world->n_v) &&
			within((v2 = world->walls[w].v2), -1, world->n_v))
			zoom_to_box(g, (t_vec[2]){ world->vecs[v1], \
						world->vecs[v2] }, 2);
	}
}
void					sec_search_st(t_prog *prog, t_vec mouse, \
											t_grid *grid, t_world *world)
{
	int 				sec;
	t_vec				*p;

	if (!prog || !world)
		return ;
	sec = lit_it(0, S_SELECT, mouse_in_stor(mouse, world, grid));
	if ((prog->click.x || prog->click.y) && within(sec, -1, world->n_s))
	{
		select_it(0, S_SELECT, sec);
		select_it(0, ST_SEL, SEC_EDIT);
		p = make_vec_tab(&world->sec[sec], world->vecs, world->n_v);
		if (p)
		{
			zoom_to_box(grid, make_vec_tab(&world->sec[sec], world->vecs, world->n_v), world->sec[sec].n_v);
			free(p);
		}
	}
	prog->redraw = 1;
}

unsigned short			check_for_light(t_media *media, int id, int sec)
{
	int 				i;

	i = 0;
	if (id != LIGHT)
		return (TRUE);
	while (i < media->worlds[media->w].sec[sec].n_it)
	{
		if (within(media->worlds[media->w].sec[sec].items[i].id, -1, media->n_itf))
		{
			if (media->it_f[media->worlds[media->w].sec[sec].items[i].id].type == LIGHT)
				return (FALSE);
		}
		i++;
	}
	return (TRUE);
}

void					sec_edit_st(t_prog *prog, t_vec mouse, \
												t_grid *grid, t_media *media)
{
	int 				sector;

	if (!prog)
		return ;
	sector = select_it(1, S_SELECT, -1);
	prog->redraw = 1;
	if (mouse_in_stor(mouse, &media->worlds[media->w], grid) == sector)
	{
		if (prog->btn_on == -1)
			move_item(prog, mouse, grid, \
			&media->worlds[media->w].sec[sector]);
		else if ((prog->click.x || prog->click.y) && \
		check_for_light(media, prog->btn_on - B_COIN, sector))
			add_item(find_def_item(prog->btn_on - B_COIN, media->it_f, \
			media->n_itf), mouse, grid, \
			&media->worlds[media->w].sec[sector]);
	}
	if (prog->btn_on != -1 && (prog->click.x || prog->click.y) &&
	!(within(prog->btn_on, F_UP_BTN - 1, C_DOWN_BTN + 1)))
	{
		turn_btns_off(prog);
		prog->click = (t_vec){ 0, 0 };
	}
	upd_sec(media->worlds[media->w].sec, media->worlds[media->w].walls,\
	media->worlds[media->w].vecs, media->worlds[media->w].n_s);
}
