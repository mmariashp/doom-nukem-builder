
#include "builder.h"

void					normal_st(t_prog *prog, t_vec mouse, t_grid *grid, \
																t_world *world)
{
	if (!prog)
		return ;
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
	if (world->sec[world->n_s - 1].is_door == TRUE && world->sec[world->n_s - 1].n_w == 4)
	{
		if (within(world->sec[world->n_s - 1].s_walls[0], -1, world->n_w))
			world->walls[world->sec[world->n_s - 1].s_walls[0]].type = WALL_EMPTY;
		if (within(world->sec[world->n_s - 1].s_walls[2], -1, world->n_w))
			world->walls[world->sec[world->n_s - 1].s_walls[2]].type = WALL_EMPTY;
	}
}

//private int FindLineCircleIntersections(
//		float cx, float cy, float radius,
//		PointF point1, PointF point2,
//		out PointF intersection1, out PointF intersection2)
//{
//	float dx, dy, A, B, C, det, t;
//
//	dx = point2.X - point1.X;
//	dy = point2.Y - point1.Y;
//
//	A = dx * dx + dy * dy;
//	B = 2 * (dx * (point1.X - cx) + dy * (point1.Y - cy));
//	C = (point1.X - cx) * (point1.X - cx) +
//		(point1.Y - cy) * (point1.Y - cy) -
//		radius * radius;
//
//	det = B * B - 4 * A * C;
//	if ((A <= 0.0000001) || (det < 0))
//	{
//		// No real solutions.
//		intersection1 = new PointF(float.NaN, float.NaN);
//		intersection2 = new PointF(float.NaN, float.NaN);
//		return 0;
//	}
//	else if (det == 0)
//	{
//		// One solution.
//		t = -B / (2 * A);
//		intersection1 =
//				new PointF(point1.X + t * dx, point1.Y + t * dy);
//		intersection2 = new PointF(float.NaN, float.NaN);
//		return 1;
//	}
//	else
//	{
//		// Two solutions.
//		t = (float)((-B + Math.Sqrt(det)) / (2 * A));
//		intersection1 =
//				new PointF(point1.X + t * dx, point1.Y + t * dy);
//		t = (float)((-B - Math.Sqrt(det)) / (2 * A));
//		intersection2 =
//				new PointF(point1.X + t * dx, point1.Y + t * dy);
//		return 2;
//	}
//}

unsigned short			circle_inter_line(t_vec mouse, int radius, t_line line)
{
	float dx, dy, A, B, C, det;

	dx = line.p1.x - line.p0.x;
	dy = line.p1.y - line.p0.y;

	A = dx * dx + dy * dy;
	B = 2 * (dx * (line.p0.x - mouse.x) + dy * (line.p0.y - mouse.y));
	C = (line.p0.x - mouse.x) * (line.p0.x - mouse.x) +
		(line.p0.y - mouse.y) * (line.p0.y - mouse.y) -
		radius * radius;

	det = B * B - 4 * A * C;
	if (A <= 0.0000001 || det < 0 || (!within(mouse.x, get_min(line.p0.x, line.p1.x), \
	get_max(line.p0.x, line.p1.x)) && !within(mouse.y, get_min(line.p0.y, line.p1.y), \
	get_max(line.p0.y, line.p1.y))))
	{
//		// No real solutions.
//		intersection1 = new PointF(float.NaN, float.NaN);
//		intersection2 = new PointF(float.NaN, float.NaN);
		return 0;
	}
	else if (det == 0)
	{
//		// One solution.
//		t = -B / (2 * A);
//		intersection1 =
//				new PointF(point1.X + t * dx, point1.Y + t * dy);
//		intersection2 = new PointF(float.NaN, float.NaN);
		return 1;
	}
	else
	{
//		// Two solutions.
//		t = (float)((-B + Math.Sqrt(det)) / (2 * A));
//		intersection1 =
//				new PointF(point1.X + t * dx, point1.Y + t * dy);
//		t = (float)((-B - Math.Sqrt(det)) / (2 * A));
//		intersection2 =
//				new PointF(point1.X + t * dx, point1.Y + t * dy);
		return 2;
	}
}

void					wall_search_st(t_prog *prog, t_vec node, \
											t_grid *grid, t_world *world)
{
	int					v1;
	int					v2;
	int 				w;

	if (!prog || !world || !grid)
		return ;
	w = -1;
	if (mouse_over((t_rec){ 0, 0, W_W, W_H }, node))
	{
		if (prog->screen[node.x][node.y] == WHITE)
		{
			int i = 0;
			while (i < world->n_w)
			{
				if (circle_inter_line(node, 15, \
				(t_line){ transform_to_screen(world->vecs[world->walls[i].v1], grid),
			  transform_to_screen(world->vecs[world->walls[i].v2], grid) }))
				{
					w = lit_it(0, W_SELECT, i);
//					printf("intersects with wall n %d\n", i);
					break ;
				}
				i++;
			}
		}
	}
	if ((prog->click.x || prog->click.y) && within(w, -1, world->n_w))
	{
		select_it(0, W_SELECT, w);
		if (prog->btn_on == DOOR_ADD_BTN)
		{
			add_door(world, w, grid);
			setup_door(world);
		}
		else
		{
			select_it(0, ST_SEL, WALL_EDIT);
			if (within((v1 = world->walls[w].v1), -1, world->n_v) &&
				within((v2 = world->walls[w].v2), -1, world->n_v))
					zoom_to_box(grid, (t_vec[2]){ world->vecs[v1], world->vecs[v2] }, 2);
		}
	}
	prog->redraw = 1;
//	if (within(node.x, -1, GRID_SIZE) && within(node.y, -1, GRID_SIZE))
//	{
//		w = lit_it(0, W_SELECT, grid->nod[node.x][node.y]);
//		if ((prog->click.x || prog->click.y) && within(w, -1, world->n_w))
//		{
//			select_it(0, W_SELECT, w);
//			if (prog->btn_on == DOOR_ADD_BTN)
//			{
//				add_door(world, w, grid);
//				setup_door(world);
//			}
//			else
//			{
//				select_it(0, ST_SEL, WALL_EDIT);
//				if (within((v1 = world->walls[w].v1), -1, world->n_v) &&
//					within((v2 = world->walls[w].v2), -1, world->n_v))
//						zoom_to_box(grid, (t_vec[2]){ world->vecs[v1], world->vecs[v2] }, 2);
//			}
//		}
//		prog->redraw = 1;
//	}
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
	if (mouse_in_stor(mouse, &media->worlds[media->w], grid) == sector)
	{
		if (prog->btn_on == -1)
			move_item(prog, mouse, grid, \
			&media->worlds[media->w].sec[sector]);
		else if ((prog->click.x || prog->click.y) && check_for_light(media, prog->btn_on - B_COIN, sector))
			add_item(find_def_item(prog->btn_on - B_COIN, media->it_f, \
			media->n_itf), mouse, grid, \
			&media->worlds[media->w].sec[sector]);
	}
	if (prog->btn_on != -1 && (prog->click.x || prog->click.y))
	{
		turn_btns_off(prog);
		prog->redraw = 1;
		prog->click = (t_vec){ 0, 0 };
	}
	upd_sec(media->worlds[media->w].sec, media->worlds[media->w].walls,\
	media->worlds[media->w].vecs, media->worlds[media->w].n_s);
}