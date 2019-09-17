
#include "builder.h"

void					free_buttons(t_button *buttons, int n)
{
	int 				i;

	i = 0;
	if (!buttons)
		return ;
	while (i < n)
	{
		if (buttons[i].text)
		{
			free(buttons[i].text);
			buttons[i].text = NULL;
		}
		i++;
	}
	free(buttons);
	buttons = NULL;
}

void					free_modes(t_mode *modes, t_sdl *sdl)
{
	int 				i;

	button_back(0, 2, sdl->rend);
	if (!modes)
		return ;
	i = 0;
	while (i < N_MODES)
	{
		if (modes[i].buttons)
			free_buttons(modes[i].buttons, modes[i].n_buttons);
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
		buttons[i].vis_lit_on[0] = TRUE;
		buttons[i].vis_lit_on[1] = FALSE;
		buttons[i].vis_lit_on[2] = FALSE;
		buttons[i].box = (t_rec){ 0, 0, 10, 10 };
		buttons[i].text_color = 0;
		buttons[i].text = NULL;
		buttons[i].txtr = NULL;
		buttons[i].lit = NULL;
		i++;
	}
	return (buttons);
}

unsigned short			distribute_buttons_h(t_button *buttons, int from, int nb, t_rec box, int padding)
{
	t_vec2d				b;
	int 				x;
	unsigned short		i;


	if (!buttons || !nb)
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


	if (!buttons || from >= to)
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
		y += b.y + padding;
		i++;
	}
	return (SUCCESS);
}

#define MAX(x,y)    (x)>(y)?(x):(y)
int distr(int x, int y, int n)
{
	int res;
	double px=ceil(sqrt(n*x/y));
	double sx,sy;
	if(floor(px*y/x)*px<n)  //does not fit, y/(x/px)=px*y/x
		sx=y/ceil(px*y/x);
	else
		sx= x/px;
	double py=ceil(sqrt(n*y/x));
	if(floor(py*x/y)*py<n)  //does not fit
		sy=x/ceil(x*py/y);
	else
		sy=y/py;
	res = MAX(sx,sy);
	return (res);
}

unsigned short			distribute_buttons_grid(t_button *buttons, int from, int to, t_rec box, int padding)
{
    t_vec2d				b;
    int 				y;
    unsigned short		i;
    int x;


    if (!buttons || from >= to || padding == -1)
        return (FAIL);
    int side = distr(box.w, box.h, to-from);

    b = (t_vec2d){ side, side };
    y = box.y;
    i = from;
    while (i < to)
    {
//        y += padding;
        x = box.x;
        while (i < to && x  + b.x < box.x + box.w)
        {
//            x += padding;
            buttons[i].box.w = b.x;
            buttons[i].box.h = b.y;
            buttons[i].box.x = x;
            buttons[i].box.y = y;
            x += b.x;
            i++;
        }
        y += b.y;

    }
    return (SUCCESS);
}

unsigned short			distribute_buttons_v2(t_button *buttons, int from, int to, t_rec box, t_vec2d button_size)
{
    int 				y;
    unsigned short		i;
    int                 padding = (box.h - (button_size.y * (to - from))) / to - from + 1;


    if (!buttons || from >= to)
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

# define N_BTN_BACKS		3

SDL_Texture				*button_back(int id, int set_get_free, SDL_Renderer *rend)
{
	static int 			init = 0;
	static SDL_Texture	**back = NULL;
	int					i;

	if (!rend)
		return (NULL);
	if (set_get_free == 0 && back == NULL && init == 0)
	{
		if (!(back = (SDL_Texture **)ft_memalloc(sizeof(SDL_Texture *) * N_BTN_BACKS)))
			return (NULL);
		back[0] = load_texture("blue_button00.png", rend, 0);
		back[1] = load_texture("blue_button13.png", rend, 0);
		back[2] = load_texture("grey_panel.png", rend, 0);
		init = 1;
		return (NULL);
	}
	if (init == 1 && set_get_free == 2 && back != NULL)
	{
		i = 0;
		while (i < N_BTN_BACKS)
		{
//			if (back[i])
//				SDL_DestroyTexture(back[i]);
			i++;
		}
		free(back);
		init = 0;
		return (NULL);
	}
	else if (init == 1 && set_get_free == 1 && id >= 0 && id < N_BTN_BACKS)
		return (back[id]);
	return (NULL);
}

unsigned short			main_menu_buttons(t_button *buttons, t_sdl *sdl)
{
	t_rec				box;
	int 				i;

	if (!buttons)
		return (FAIL);
    box.w = WIN_W / 4;
    box.h = (box.w + 20) * N_MM_BTNS / 3;
	box.x = (WIN_W  - box.w) / 2;
	box.y = (WIN_H  - box.h) / 2;
	distribute_buttons_v(buttons, 0, N_MM_BTNS ,box, 20);
	i = 0;

	while (i < N_MM_BTNS)
	{
		buttons[i].txtr = button_back(1, 1, sdl->rend);
		buttons[i].lit = button_back(0, 1, sdl->rend);
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
		buttons[i].txtr = button_back(1, 1, sdl->rend);
		buttons[i].lit = button_back(0, 1, sdl->rend);
		buttons[i].text_color = 0;
		i++;
	}
	return (SUCCESS);
}

unsigned short			textures_buttons(t_button *buttons, t_texture *textures, int n_txtrs, t_sdl *sdl)
{
    t_rec				box;
    int 				i;

    if (!buttons || !textures || !sdl)
        return (FAIL);
    box.w = WIN_W * 0.9;
    box.h = WIN_H * 0.9;
    box.x = (WIN_W  - box.w) / 2;
    box.y = (WIN_H  - box.h) / 2;
    distribute_buttons_grid(buttons, 0, n_txtrs , box, 20);
    i = 0;
    while (i < n_txtrs)
    {
        buttons[i].vis_lit_on[0] = TRUE;
        buttons[i].txtr = button_back(2, 1, sdl->rend);
        buttons[i].lit = button_back(0, 1, sdl->rend);
        i++;
    }
    return (SUCCESS);
}


//unsigned short			walls_buttons(t_button *buttons, int n_buttons, t_sdl *sdl)
//{
//	t_rec				box;
//	int 				i;
//
//	if (!buttons || !sdl)
//		return (FAIL);
//	box.h = WIN_H * 0.07;
//	box.w = box.h  * n_buttons;
//	box.x = 10;
//	box.y = 0;
//	distribute_buttons_h(buttons, 0, n_buttons , box, 3);
//	i = 0;
//	while (i < n_buttons)
//	{
//		buttons[i].vis_lit_on[0] = TRUE;
//		buttons[i].txtr = NULL;
//		buttons[i].lit = NULL;
//		i++;
//	}
//	buttons[W_BACK_BTN].txtr = load_texture("back22.png", sdl->rend, 0);
//	buttons[W_BACK_BTN].lit = load_texture("back3.png", sdl->rend, 0);
//    buttons[W_DESELECT_BTN].txtr =      load_texture("cross2.png", sdl->rend, 0);
//    buttons[W_DESELECT_BTN].lit =      load_texture("cross3.png", sdl->rend, 0);
//    buttons[WT_BTN].txtr =            load_texture("edit.png", sdl->rend, 0);
//    buttons[WT_BTN].lit =                load_texture("editlit.png", sdl->rend, 0);
//    buttons[W_PORTAL_BTN].txtr =        buttons[WT_BTN].txtr;
//    buttons[W_PORTAL_BTN].lit =          buttons[WT_BTN].lit;
//    buttons[W_DOOR_BTN].txtr =          buttons[WT_BTN].txtr;
//    buttons[W_DOOR_BTN].lit =            buttons[WT_BTN].lit;
//
//    box = layout_menu(0, 0);
//    buttons[W_DESELECT_BTN].box =  (t_rec){ box.x + box.w,         box.y,          30, 30 };
//    buttons[WT_BTN].box = layout_menu(6, 0);
//    buttons[W_PORTAL_BTN].box = layout_menu(6, 1);
//    buttons[W_DOOR_BTN].box = layout_menu(6, 2);
//
//    buttons[W_DESELECT_BTN].vis_lit_on[0] = FALSE;
//    buttons[WT_BTN].vis_lit_on[0] = FALSE;
//    buttons[W_PORTAL_BTN].vis_lit_on[0] = FALSE;
//    buttons[W_DOOR_BTN].vis_lit_on[0] = FALSE;
//	return (SUCCESS);
//}

unsigned short			editor_buttons(t_button *buttons, int n, t_sdl *sdl)
{
	if (!sdl || !buttons || n < 0)
		return (FAIL);
	return (SUCCESS);
}

void                    refresh_level_list(t_media *media, t_mode *mode, t_sdl *sdl)
{
    if (mode->n_buttons == media->n_worlds + 1)
        return ;
    if (mode->buttons)
		free_buttons(mode->buttons, mode->n_buttons);
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

    prog->modes[MODE_TEXTURES].input =   &input_textures;
    prog->modes[MODE_TEXTURES].update = &update_textures;
    prog->modes[MODE_TEXTURES].render = &render_textures;

	prog->modes[MODE_MAIN_MENU].n_buttons = N_MM_BTNS;
	prog->modes[MODE_SUMMARY].n_buttons = media->n_worlds + 1;
	prog->modes[MODE_EDITOR].n_buttons = 8;
    prog->modes[MODE_TEXTURES].n_buttons = media->n_txtrs;
	i = 0;
	while (i < N_MODES)
	{
		if (prog->modes[i].n_buttons > 0 && !(prog->modes[i].buttons = init_buttons(prog->modes[i].n_buttons)))
			return (FAIL);
		i++;
	}
	button_back(0, 0, sdl->rend);
	main_menu_buttons(prog->modes[MODE_MAIN_MENU].buttons, sdl);
	summary_buttons(prog->modes[MODE_SUMMARY].buttons, media->worlds, media->n_worlds, sdl);
//	editor_buttons(prog->modes[MODE_EDITOR].buttons, prog->modes[MODE_EDITOR].n_buttons, sdl);
    textures_buttons(prog->modes[MODE_TEXTURES].buttons, media->txtrs, media->n_txtrs, sdl);
	return (SUCCESS);
}