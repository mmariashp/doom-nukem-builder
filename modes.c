
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
//	if (button->front)
//	{
//		SDL_DestroyTexture(button->front);
//		button->front = NULL;
//	}
}

void					free_modes(t_mode *modes, t_sdl *sdl)
{
	int 				i;
	int 				j;

	button_back(0, 2, sdl);
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
		buttons[i].back = NULL;
		buttons[i].lit_back = NULL;
		buttons[i].front = NULL;
		i++;
	}
	return (buttons);
}

unsigned short			distribute_buttons_h(t_button *buttons, int from, int nb, t_rec box, int padding)
{
	t_vec2d				b;
	int 				x;
	unsigned short		i;


	if (!buttons)
		return (FAIL);
	b = (t_vec2d){ box.w / nb - padding, box.h };
	x = box.x;
	i = from;
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

unsigned short			distribute_buttons_v(t_button *buttons, int from, int to, t_rec box, int padding)
{
	t_vec2d				b;
	int 				y;
	unsigned short		i;


	if (!buttons)
		return (FAIL);
	b = (t_vec2d){ box.w, box.h / (to - from) - padding };
    y = box.y;
	i = from;
	while (i < to)
	{
		buttons[i].box.w = b.x;
		buttons[i].box.h = b.y;
		buttons[i].box.x = box.x;
		buttons[i].box.y = y;

        printf("in distr button n%d; %d, %d, %d, %d\n", i, buttons[i].box.w, buttons[i].box.h, buttons[i].box.x, buttons[i].box.y);
		y += b.y + padding;
		i++;
	}
	return (SUCCESS);
}

unsigned short			distribute_buttons_v2(t_button *buttons, int from, int to, t_rec box, t_vec2d button_size)
{
    int 				y;
    unsigned short		i;
    int                 padding = (box.h - (button_size.y * (to - from))) / to - from + 1;


    if (!buttons)
        return (FAIL);
    y = box.y;
    i = from;
    while (i < to)
    {
        y += padding;
        buttons[i].box.w = button_size.x;
        buttons[i].box.h = button_size.y;
        buttons[i].box.x = box.x;
        buttons[i].box.y = y;
        y += button_size.y;
        i++;
    }
    return (SUCCESS);
}

# define N_BUTTON_BACKS		3

SDL_Texture				*button_back(int id, int set_get_free, t_sdl *sdl)
{
	static int 			init = 0;
	static SDL_Texture	**back = NULL;
	int					i;

	if (!sdl)
		return (NULL);
	if (set_get_free == 0 && back == NULL && init == 0)
	{
		printf("back textures to be set\n");
		if (!(back = (SDL_Texture **)ft_memalloc(sizeof(SDL_Texture *) * N_BUTTON_BACKS)))
			return (NULL);
		back[0] = load_texture("blue_button00.png", sdl);
		back[1] = load_texture("blue_button13.png", sdl);
		back[2] = load_texture("grey_panel.png", sdl);
		init = 1;
		printf("back textures were set\n");
		return (NULL);
	}
	if (init == 1 && set_get_free == 2 && back != NULL)
	{
		i = 0;
		printf("textures freed\n");
		while (i < N_BUTTON_BACKS)
		{
//			if (back[i])
//				SDL_DestroyTexture(back[i]);
			i++;
		}
		free(back);
		init = 0;
		return (NULL);
	}
	else if (init == 1 && set_get_free == 1 && id >= 0 && id < N_BUTTON_BACKS)
		return (back[id]);
	return (NULL);
}



unsigned short			main_menu_buttons(t_button *buttons, t_sdl *sdl)
{
	t_rec				box;
	int 				i;

	if (!buttons)
		return (FAIL);
	box.w = WIN_W / 3;
	box.h = WIN_H / 3;
	box.x = (WIN_W  - box.w) / 2;
	box.y = (WIN_H  - box.h) / 2;
	distribute_buttons_v(buttons, 0, N_MM_BUTTONS ,box, 20);
	i = 0;

	while (i < N_MM_BUTTONS)
	{
		buttons[i].vis_lit_on[0] = TRUE;
		buttons[i].vis_lit_on[1] = FALSE;
		buttons[i].vis_lit_on[2] = FALSE;
		buttons[i].text_color = WHITE;
		buttons[i].back = button_back(0, 1, sdl);
		buttons[i].lit_back = button_back(1, 1, sdl);
		i++;
	}
	buttons[0].text = ft_strdup("START");
	buttons[1].text = ft_strdup("QUIT");
	return (SUCCESS);
}

unsigned short			summary_buttons(t_button *buttons, t_world *worlds, int n_worlds, t_sdl *sdl)
{
	t_rec				box;
	int 				i;
	char				*s;
	char 				*tmp;

	if (!buttons || !worlds)
		return (FAIL);
	box.w = WIN_W / 4;
	box.h = (box.w + 20) * (n_worlds + 1) / 3;
	box.x = (WIN_W  - box.w) / 2;
	box.y = (WIN_H  - box.h) / 2;
	distribute_buttons_v(buttons, 0, n_worlds + 1 , box, 20);
	i = 0;
	s = NULL;
	while (i < n_worlds + 1)
	{
		buttons[i].vis_lit_on[0] = TRUE;
		tmp = ft_itoa(i);
		s = ft_strjoin(tmp, ". ");
		free(tmp);
		if (i == n_worlds)
			buttons[i].text = ft_strjoin(s, " ADD LEVEL");
		else
			buttons[i].text = ft_strjoin(s, worlds[i].filename);
		if (s)
			free(s);
		buttons[i].back = button_back(0, 1, sdl);
		buttons[i].lit_back = button_back(1, 1, sdl);
		buttons[i].text_color = WHITE;
		i++;
	}
	return (SUCCESS);
}

unsigned short			editor_buttons(t_button *buttons, int n, t_sdl *sdl)
{
	t_rec				box;
	int 				i;

	if (!sdl || !buttons)
		return (FAIL);
	box.h = WIN_H * 0.07;
	box.w = box.h  * 7;
	box.x = 10;
	box.y = 0;
	distribute_buttons_h(buttons, 0,  7 , box, 3);
	i = 0;
	while (i < n)
	{
		buttons[i].vis_lit_on[0] = TRUE;
		buttons[i].vis_lit_on[1] = FALSE;
		buttons[i].vis_lit_on[2] = FALSE;
		buttons[i].text_color = 0;
		buttons[i].back = button_back(0, 1, sdl);
		buttons[i].lit_back = button_back(1, 1, sdl);
		buttons[0].text = NULL;
		i++;
	}
	buttons[DRAG_BUTTON].front = load_texture("move2.png", sdl);
	buttons[DRAW_BUTTON].front = load_texture("add2.png", sdl);
	buttons[DISTORT_BUTTON].front = load_texture("distort2.png", sdl);
	buttons[DELETE_BUTTON].front = load_texture("delete2.png", sdl);
	buttons[BACK_BUTTON].front = load_texture("back22.png", sdl);
	buttons[SAVE_BUTTON].front = load_texture("save2.png", sdl);
	buttons[SECTOR_BUTTON].front = load_texture("sector22.png", sdl);
	buttons[DESELECT_SEC_BUTTON].vis_lit_on[0] = FALSE;
	buttons[DESELECT_SEC_BUTTON].front = load_texture("cross2.png", sdl);

	buttons[DRAG_BUTTON].lit = load_texture("move3.png", sdl);
	buttons[DRAW_BUTTON].lit = load_texture("add3.png", sdl);
	buttons[DISTORT_BUTTON].lit = load_texture("distort3.png", sdl);
	buttons[DELETE_BUTTON].lit = load_texture("delete3.png", sdl);
	buttons[BACK_BUTTON].lit = load_texture("back3.png", sdl);
	buttons[SAVE_BUTTON].lit = load_texture("save3.png", sdl);
	buttons[SECTOR_BUTTON].lit = load_texture("sector3.png", sdl);
	buttons[DESELECT_SEC_BUTTON].lit = load_texture("cross3.png", sdl);
	buttons[F_UP_BUTTON].lit = load_texture("up3.png", sdl);
	buttons[F_DOWN_BUTTON].lit = load_texture("down3.png", sdl);
	buttons[C_UP_BUTTON].lit = load_texture("up3.png", sdl);
	buttons[C_DOWN_BUTTON].lit = load_texture("down3.png", sdl);

	buttons[F_UP_BUTTON].front = load_texture("up2.png", sdl);
	buttons[F_DOWN_BUTTON].front = load_texture("down2.png", sdl);
	buttons[F_UP_BUTTON].vis_lit_on[0] = FALSE;
	buttons[F_DOWN_BUTTON].vis_lit_on[0] = FALSE;

	buttons[C_UP_BUTTON].front = load_texture("up2.png", sdl);
	buttons[C_DOWN_BUTTON].front = load_texture("down2.png", sdl);
	buttons[C_UP_BUTTON].vis_lit_on[0] = FALSE;
	buttons[C_DOWN_BUTTON].vis_lit_on[0] = FALSE;


	box = sector_menu(0, 0);
	buttons[DESELECT_SEC_BUTTON].box =  (t_rec){ box.x + box.w,         box.y,          30, 30 };
    buttons[F_UP_BUTTON].box = sector_menu(3, 0);
    buttons[F_DOWN_BUTTON].box = sector_menu(4, 0);
    buttons[C_UP_BUTTON].box = sector_menu(3, 1);
    buttons[C_DOWN_BUTTON].box = sector_menu(4, 1);
//    box.x = box.x + box.w * 0.9;
//    box.w = box.w / 10;
//    t_rec box2 = sector_menu(2, 0);
//    box.y = box2.y;
//    printf("here\n");
//    distribute_buttons_v2(buttons, F_UP_BUTTON, C_DOWN_BUTTON + 1, box, (t_vec2d){ 30 , 30 });
//    printf("button %d, %d, %d, %d\n", buttons[F_UP_BUTTON].box.w, buttons[F_UP_BUTTON].box.h, buttons[F_UP_BUTTON].box.x, buttons[F_UP_BUTTON].box.y);
//	buttons[F_UP_BUTTON].box =          (t_rec){ box.x + box.w * 0.7,   box.y,          30, 30 };
//	buttons[F_DOWN_BUTTON].box =        (t_rec){ box.x + box.w * 0.7,   box.y + 40,     30, 30 };
//	buttons[C_UP_BUTTON].box =          (t_rec){ box.x + box.w * 0.7,   box.y + 80,     30, 30 };
//	buttons[C_DOWN_BUTTON].box =        (t_rec){ box.x + box.w * 0.7,   box.y + 120,    30, 30 };
	return (SUCCESS);
}

void                    refresh_level_list(t_media *media, t_mode *mode, t_sdl *sdl)
{
    int                 j;

    if (mode->n_buttons == media->n_worlds + 1)
        return ;
    if (mode->buttons)
    {
        j = 0;
        while (j < mode->n_buttons)
        {
            free_button(&mode->buttons[j]);
            j++;
        }
        free(mode->buttons);
        mode->buttons = NULL;
    }
    mode->n_buttons = media->n_worlds + 1;
    mode->buttons = init_buttons(mode->n_buttons);
    if (!mode->buttons)
        return ;
    summary_buttons(mode->buttons, media->worlds, media->n_worlds, sdl);
}

unsigned short			init_modes(t_sdl *sdl, t_media *media, t_prog *prog)
{
	t_mode				*modes;
	int 				i;

	if (!media || !sdl || !prog)
		return (FAIL);
	modes = (t_mode *)ft_memalloc(sizeof(t_mode) * N_MODES);
	if (!modes)
		return (FAIL);
	ft_bzero(modes, sizeof(modes));
	prog->modes = modes;

	prog->modes[MODE_MAIN_MENU].input = &input_main_menu;
	prog->modes[MODE_MAIN_MENU].update = &update_main_menu;
	prog->modes[MODE_MAIN_MENU].render = &render_main_menu;

	prog->modes[MODE_SUMMARY].input =   &input_summary;
	prog->modes[MODE_SUMMARY].update = &update_summary;
	prog->modes[MODE_SUMMARY].render = &render_summary;

	prog->modes[MODE_EDITOR].input =   &input_editor;
	prog->modes[MODE_EDITOR].update = &update_editor;
	prog->modes[MODE_EDITOR].render = &render_editor;

	prog->modes[MODE_MAIN_MENU].n_buttons = N_MM_BUTTONS;
	prog->modes[MODE_SUMMARY].n_buttons = media->n_worlds + 1;
	prog->modes[MODE_EDITOR].n_buttons = 12;
	i = 0;
	while (i < N_MODES)
	{
		if (prog->modes[i].n_buttons > 0 && !(prog->modes[i].buttons = init_buttons(prog->modes[i].n_buttons)))
			return (FAIL);
		i++;
	}
	button_back(0, 0, sdl);
	main_menu_buttons(prog->modes[MODE_MAIN_MENU].buttons, sdl);
	summary_buttons(prog->modes[MODE_SUMMARY].buttons, media->worlds, media->n_worlds, sdl);
	editor_buttons(prog->modes[MODE_EDITOR].buttons, prog->modes[MODE_EDITOR].n_buttons, sdl);
	return (SUCCESS);
}