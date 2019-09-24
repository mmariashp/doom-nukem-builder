
#include "builder.h"

void					render_values(int state, int n, t_value *values, SDL_Renderer *rend, SDL_Texture **t)
{
	int 				i;
	t_rec				box;

	if (!values || !rend)
		return ;
	i = -1;
	n = clamp(n, 0, 255);
	while (++i < n)
	{
		box = layout_menu(2, (char)i);
		if (i == 0 && within(TXTR_RECT_GR_L, -1, TOTAL_TXTRS) && t[TXTR_RECT_GR_L])
			render_box(box, t[TXTR_RECT_GR_L], rend);
		else if (within(TXTR_RECT_GR, -1, TOTAL_TXTRS) && t[TXTR_RECT_GR])
			render_box(box, t[TXTR_RECT_GR], rend);
		write_text(menu_lines(state, i), rend, box, EDIT_TEXT_COLOR, FALSE);
		box = layout_menu(5, (char)i);
		if (values[i].text)
			write_text(values[i].text, rend, box, EDIT_TEXT_COLOR, FALSE);
		else if (values[i].texture)
		{
			box.w = box.h;
			render_box(box, values[i].texture, rend);
		}
	}
}

t_value					*init_values(int n)
{
	t_value				*new;
	int 				i;

	new = ft_memalloc(sizeof(t_value) * n);
	if (!new)
		return (NULL);
	i = 0;
	while (i < n)
	{
		new[i].text = NULL;
		new[i].texture = NULL;
		i++;
	}
	return (new);
}

void					free_values(t_value *values, int n)
{
	int 				i;

	i = 0;
	if (!values)
		return ;
	while (i < n)
	{
		if (values[i].text)
		{
			free(values[i].text);
			values[i].text = NULL;
		}
		i++;
	}
	free(values);
	values = NULL;
}