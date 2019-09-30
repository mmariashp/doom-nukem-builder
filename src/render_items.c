/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   render_items.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mshpakov <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/09/29 13:08:56 by mshpakov          #+#    #+#             */
/*   Updated: 2019/09/29 13:08:58 by mshpakov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "builder.h"

void					draw_item(int type, t_rec box, SDL_Renderer *rend, \
SDL_Texture **t)
{
	int					txtr;

	if (!rend || !t)
		return ;
	txtr = type + TXTR_COIN;
	if (within(type, -1, TOTAL_TYPES) && within(txtr, -1, N_TXTR) && t[txtr])
	{
		if (type == KEY || type == HEALTH)
		{
			box.x += box.w * 0.2;
			box.w *= 0.8;
		}
		rend_box(box, t[txtr], rend);
	}
}

unsigned short			ingrid(t_vec p)
{
	if (within(p.x, -1, GRID_SIZE) && within(p.y, -1, GRID_SIZE))
		return (TRUE);
	return (FALSE);
}

t_vec					get_p(t_grid *g, t_vec items_p)
{
	t_vec				p;

	p.x = (int)(g->box.x + (items_p.x * g->scl));
	p.y = (int)(g->box.y + (items_p.y * g->scl));
	return (p);
}

void					draw_itms(SDL_Renderer *r, t_media *m, t_grid *g, \
SDL_Texture **t)
{
	int					s;
	int					i;
	int					id;
	t_vec				p;
	t_rec				b;

	if (!m || !m->worlds || !m->it_f || !g)
		return ;
	s = 0;
	while (s < m->worlds[m->w].n_s)
	{
		b = (t_rec){ 0, 0, g->box.w * 0.009, g->box.w * 0.009 };
		i = -1;
		while (++i < m->worlds[m->w].sec[s].n_it)
		{
			if (ingrid((p = m->worlds[m->w].sec[s].items[i].p)) && nod_in_sec(p,\
		&m->worlds[m->w]) == s && g->nod[p.x][p.y] == (signed char)(-10 - i)
				&& within((id = m->worlds[m->w].sec[s].items[i].id), -1, m->n_itf))
			{
				p = get_p(g, m->worlds[m->w].sec[s].items[i].p);
				draw_item(m->it_f[id].type, (b = (t_rec){ p.x - b.w / 2, p.y - b.h \
			/ 2, b.w, b.h }), r, t);
			}
			else
				delete_item(&m->worlds[m->w].sec[s], i--);
		}
		s++;
	}

}
