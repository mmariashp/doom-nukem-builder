
#include "builder.h"

void					free_button(t_button *button)
{
	if (!button)
		return ;
	if (button->text)
	{
		free(button->text);
		button->text = NULL;
	}
	if (button->texture)
	{
		SDL_DestroyTexture(button->texture);
		button->texture = NULL;
	}
}

void					free_modes(t_mode *modes)
{
	int 				i;
	int 				j;

	if (!modes)
		return ;
	i = 0;
	while (i < N_MODES)
	{
		if (modes[i].buttons)
		{
			j = 0;
			while (j < modes[i].n_buttons)
			{
				free_button(&modes[i].buttons[j]);
				j++;
			}
			free(modes[i].buttons);
			modes[i].buttons = NULL;
		}
		i++;
	}
	free(modes);
	modes = NULL;
}

t_button				*init_buttons(int n_buttons)
{
	int 				i;
	t_button			*buttons;

	buttons = (t_button *)ft_memalloc(sizeof(t_button) * n_buttons);
	if (!buttons)
		return (NULL);
	ft_bzero(buttons, sizeof(buttons));
	i = 0;
	while (i < n_buttons)
	{
		buttons[i].text = NULL;
		buttons[i].texture = NULL;
		buttons[i].background = -1;
		i++;
	}
	return (buttons);
}

unsigned short			distribute_buttons_h(t_button *buttons, int nb, t_rec box, int padding)
{
	t_vec2d				b;
	int 				x;
	unsigned short		i;


	if (!buttons)
		return (FAIL);
	b = (t_vec2d){ box.w / nb - padding, box.h };
	x = box.x;
	i = 0;
	while (i < nb)
	{
		buttons[i].box.w = b.x;
		buttons[i].box.h = b.y;
		buttons[i].box.x = x;
		buttons[i].box.y = box.y;
		x += b.x + padding;
		i++;
	}

	return (SUCCESS);
}

unsigned short			distribute_buttons_v(t_button *buttons, int nb, t_rec box)
{
	t_vec2d				b;
	int 				y;
	unsigned short		i;


	if (!buttons)
		return (FAIL);
	b = (t_vec2d){ box.w, box.h / nb };
	y = box.y;
	i = 0;
	while (i < nb)
	{
		buttons[i].box.w = b.x;
		buttons[i].box.h = b.y;
		buttons[i].box.x = box.x;
		buttons[i].box.y = y;
		y += b.y;
		i++;
	}

	return (SUCCESS);
}

unsigned short			main_menu_buttons(t_button *buttons)
{
	t_rec				box;
	int 				i;

	if (!buttons)
		return (FAIL);
	box.w = WIN_W / 3;
	box.h = WIN_H / 3;
	box.x = WIN_W / 3;
	box.y = WIN_H / 3;
	distribute_buttons_v(buttons, N_MM_BUTTONS ,box);
	i = 0;
	while (i < N_MM_BUTTONS)
	{
		buttons[i].text_color = WHITE;
		buttons[i].is_lit = FALSE;
		i++;
	}
	buttons[0].text = ft_strdup("START");
	buttons[1].text = ft_strdup("QUIT");
	return (SUCCESS);
}

unsigned short			summary_buttons(t_button *buttons, t_world *worlds, int n_worlds)
{
	t_rec				box;
	int 				i;
	char				*s;

	if (!buttons || !worlds)
		return (FAIL);
	box.w = WIN_W / 3;
	box.h = box.w * n_worlds / 4;
	box.x = WIN_W / 4;
	box.y = WIN_H / 3;
	distribute_buttons_v(buttons, n_worlds + 1 ,box);
	i = 0;
	s = NULL;
	while (i < n_worlds + 1)
	{
		s = ft_strjoin(ft_itoa(i), ". ");
		if (i == n_worlds)
			buttons[i].text = ft_strjoin(s, " ADD LEVEL");
		else
			buttons[i].text = ft_strjoin(s, worlds[i].filename);
		buttons[i].text_color = WHITE;
		buttons[i].is_lit = FALSE;
		i++;
	}
	if (s)
		free(s);
	return (SUCCESS);
}

unsigned short			editor_buttons(t_button *buttons, int n)
{
	t_rec				box;
	int 				i;

	if (!buttons)
		return (FAIL);
	box.h = WIN_H * 0.03;
	box.w = box.h * 4 * n;
	box.x = 10;
	box.y = 0;
	distribute_buttons_h(buttons, n , box, 3);
	i = 0;
	while (i < n)
	{
		buttons[i].text_color = WHITE;
		buttons[i].is_lit = FALSE;
		buttons[i].background = DARK_PURPLE;
		i++;
	}
	buttons[0].text = ft_strdup("VIEW");
	buttons[1].text = ft_strdup("DRAW");
	buttons[2].text = ft_strdup("MOVE");
	buttons[3].text = ft_strdup("DELETE");
	return (SUCCESS);
}

unsigned short			init_modes(t_sdl *sdl, t_media *media)
{
	t_mode				*modes;
	int 				i;

	if (!media || !sdl)
		return (FAIL);
	modes = (t_mode *)ft_memalloc(sizeof(t_mode) * N_MODES);
	if (!modes)
		return (FAIL);
	ft_bzero(modes, sizeof(modes));
	sdl->modes = modes;

	sdl->modes[MODE_MAIN_MENU].input = &input_main_menu;
	sdl->modes[MODE_MAIN_MENU].update = &update_main_menu;
	sdl->modes[MODE_MAIN_MENU].render = &render_main_menu;

	sdl->modes[MODE_SUMMARY].input =   &input_summary;
	sdl->modes[MODE_SUMMARY].update = &update_summary;
	sdl->modes[MODE_SUMMARY].render = &render_summary;

	sdl->modes[MODE_EDITOR].input =   &input_editor;
	sdl->modes[MODE_EDITOR].update = &update_editor;
	sdl->modes[MODE_EDITOR].render = &render_editor;

	sdl->modes[MODE_MAIN_MENU].n_buttons = N_MM_BUTTONS;
	sdl->modes[MODE_SUMMARY].n_buttons = media->n_worlds + 1;
	sdl->modes[MODE_EDITOR].n_buttons = 4;
	printf("////%d\n", sdl->modes[MODE_SUMMARY].n_buttons);
	i = 0;
	while (i < N_MODES)
	{
		if (sdl->modes[i].n_buttons > 0 && !(sdl->modes[i].buttons = init_buttons(sdl->modes[i].n_buttons)))
			return (FAIL);
		i++;
	}
	main_menu_buttons(sdl->modes[MODE_MAIN_MENU].buttons);
	summary_buttons(sdl->modes[MODE_SUMMARY].buttons, media->worlds, media->n_worlds);
	editor_buttons(sdl->modes[MODE_EDITOR].buttons, sdl->modes[MODE_EDITOR].n_buttons);
	return (SUCCESS);
}