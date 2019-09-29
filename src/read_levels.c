/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   read_levels.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mshpakov <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/09/29 18:45:27 by mshpakov          #+#    #+#             */
/*   Updated: 2019/09/29 18:45:28 by mshpakov         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "builder.h"

unsigned				read_levels(t_media *m, t_section *s)
{
	short				i;

	if (!m || !s || !s->tab || s->n_files < 1 || s->n_files > MAX_LEVELS ||
	!(m->worlds = (t_world *)ft_memalloc(sizeof(t_world) * s->n_files)))
		return (FAIL);
	ft_bzero(m->worlds, sizeof(t_world) * (m->n_worlds = s->n_files));
	i = -1;
	while (++i < s->n_files)
	{
		if (!s->tab[i] || !s->names[i])
			return (FAIL);
		ft_bzero(&m->worlds[i], sizeof(t_world));
		m->worlds[i].full_p = ft_strdup(s->tab[i]);
		m->worlds[i].name = ft_strdup(s->names[i]);
		m->worlds[i].sec = NULL;
		m->worlds[i].walls = NULL;
		m->worlds[i].vecs = NULL;
		m->worlds[i].n_s = 0;
		m->worlds[i].n_v = 0;
		m->worlds[i].n_w = 0;
		if (!m->worlds[i].full_p || !m->worlds[i].name || \
		get_map(&m->worlds[i], i) == FAIL)
			return (FAIL);
	}
	return (SUCCESS);
}
