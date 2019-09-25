
#include "builder.h"

void					free_btn(t_button *btn, int n)
{
	int 				i;

	i = 0;
	if (!btn)
		return ;
	while (i < n)
	{
		if (btn[i].text)
		{
			free(btn[i].text);
			btn[i].text = NULL;
		}
		i++;
	}
	free(btn);
	btn = NULL;
}

void					free_modes(t_mode *modes)
{
	int 				i;

	if (!modes)
		return ;
	i = 0;
	while (i < N_MODES)
	{
		printf("mode = %d, n-b = %d\n", i, modes[i].n_btn);
		if (modes[i].btn)
		{
			printf("btn yes\n");
			free_btn(modes[i].btn, modes[i].n_btn);
		}
		i++;
	}
	free(modes);
	modes = NULL;
}

t_button				*init_btn(int n_btn)
{
	int 				i;
	t_button			*btn;

	btn = (t_button *)ft_memalloc(sizeof(t_button) * n_btn);
	if (!btn)
		return (NULL);
	ft_bzero(btn, sizeof(t_button) * n_btn);
	i = 0;
	while (i < n_btn)
	{
		btn[i].vis_lit_on[0] = TRUE;
		btn[i].vis_lit_on[1] = FALSE;
		btn[i].vis_lit_on[2] = FALSE;
		btn[i].box = (t_rec){ 0, 0, 10, 10 };
		btn[i].text_color = 0;
		btn[i].text = NULL;
		btn[i].reg_i = -1;
		btn[i].lit_i = -1;
		i++;
	}
	return (btn);
}

unsigned short			distribute_btn_h(t_button *btn, int from, int to, t_rec box, int padding)
{
	t_vec2d				b;
	int 				x;
	unsigned short		i;


	if (!btn || !to)
		return (FAIL);
	b = (t_vec2d){ box.w / (to - from) - padding, box.h };
	x = box.x;
	i = from;
	while (i < to)
	{
		btn[i].box.w = b.x;
		btn[i].box.h = b.y;
		btn[i].box.x = x;
		btn[i].box.y = box.y;
		x += b.x + padding;
		i++;
	}

	return (SUCCESS);
}

unsigned short			distribute_btn_v(t_button *btn, int from, int to, t_rec box, int padding)
{
	t_vec2d				b;
	int 				y;
	unsigned short		i;


	if (!btn || from >= to)
		return (FAIL);
	b = (t_vec2d){ box.w, box.h / (to - from) - padding };
    y = box.y;
	i = from;
	while (i < to)
	{
		btn[i].box.w = b.x;
		btn[i].box.h = b.y;
		btn[i].box.x = box.x;
		btn[i].box.y = y;
		y += b.y + padding;
		i++;
	}
	return (SUCCESS);
}

#define MAX(x,y)    (x)>(y)?(x):(y)
int						distr(int x, int y, int n)
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

unsigned short			distribute_btn_grid(t_button *btn, int from, int to, t_rec box, int padding)
{
    t_vec2d				b;
    int 				y;
    unsigned short		i;
    int x;


    if (!btn || from >= to || padding == -1)
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
            btn[i].box.w = b.x;
            btn[i].box.h = b.y;
            btn[i].box.x = x;
            btn[i].box.y = y;
            x += b.x;
            i++;
        }
        y += b.y;

    }
    return (SUCCESS);
}

unsigned short			distribute_btn_v2(t_button *btn, int from, int to, t_rec box, t_vec2d button_size)
{
    int 				y;
    unsigned short		i;
    int                 padding = (box.h - (button_size.y * (to - from))) / to - from + 1;


    if (!btn || from >= to)
        return (FAIL);
    y = box.y;
    i = from;
    while (i < to)
    {
        y += padding;
        btn[i].box.w = button_size.x;
        btn[i].box.h = button_size.y;
        btn[i].box.x = box.x;
        btn[i].box.y = y;
        y += button_size.y;
        i++;
    }
    return (SUCCESS);
}

unsigned short			main_menu_btn(t_button *btn)
{
	t_rec				box;
	int 				i;

	if (!btn)
		return (FAIL);
    box.w = WIN_W / 4;
    box.h = (box.w + 20) * N_MM_BTNS / 3;
	box.x = (WIN_W  - box.w) / 2;
	box.y = (WIN_H  - box.h) / 2;
	distribute_btn_v(btn, 0, N_MM_BTNS ,box, 20);
	i = 0;
	while (i < N_MM_BTNS)
	{
		btn[i].reg_i = TXTR_RECT_GR;
		btn[i].lit_i = TXTR_RECT_GR_L;
		i++;
	}
	btn[0].text = ft_strdup("START");
	btn[1].text = ft_strdup("QUIT");
	return (SUCCESS);
}

unsigned short			levels_btn(t_button *btn, t_world *worlds, int n_worlds)
{
	t_rec				box;
	int 				i;
	char				*s;
	char 				*tmp;
	int 				n_levels = n_worlds + 1;
	int 				n_btn = n_levels * 3 - 2;

	if (!btn || !worlds)
		return (FAIL);
	box.w = WIN_W / 4;
	box.h = (box.w + 20) * n_levels / 3;
	box.x = (WIN_W  - box.w) / 2;
	box.y = (WIN_H  - box.h) / 2;
	distribute_btn_v(btn, 0, n_levels , box, 20);
	i = 0;
	s = NULL;
	while (i < n_levels)
	{
		tmp = ft_itoa(i);
		s = ft_strjoin(tmp, ". ");
		free(tmp);
		if (s)
		{
			if (i == n_worlds)
				btn[i].text = ft_strjoin(s, " ADD LEVEL");
			else if (i < n_worlds)
				btn[i].text = ft_strjoin(s, worlds[i].filename);
			free(s);
		}
		btn[i].reg_i = TXTR_RECT_GR;
		btn[i].lit_i = TXTR_RECT_GR_L;
		i++;
	}
	int j = 0;
	while (i + 1 < n_btn)
	{
		btn[i].reg_i = TXTR_EDIT;
		btn[i].lit_i = TXTR_EDIT_L;
		btn[i + 1].reg_i = TXTR_DEL;
		btn[i + 1].lit_i = TXTR_DEL_L;
		btn[i].box = btn[j].box;
		btn[i + 1].box = btn[j].box;
		btn[i].box.x += btn[j].box.w + 10;
		btn[i + 1].box.x += btn[j].box.w + 50;
		btn[i].box.w = 30;
		btn[i + 1].box.w = 30;
		btn[i].box.h = 30;
		btn[i + 1].box.h = 30;
		i += 2;
		j++;
	}
	return (SUCCESS);
}

unsigned short			textures_btn(t_button *btn, t_texture *textures, int n_txtrs)
{
    t_rec				box;
    int 				i;

    if (!btn || !textures)
        return (FAIL);
    box.w = WIN_W * 0.9;
    box.h = WIN_H * 0.9;
    box.x = (WIN_W  - box.w) / 2;
    box.y = (WIN_H  - box.h) / 2;
    distribute_btn_grid(btn, 0, n_txtrs , box, 20);
    i = 0;
    while (i < n_txtrs)
    {
        btn[i].vis_lit_on[0] = TRUE;
		btn[i].reg_i = TXTR_RECT_GR;
		btn[i].lit_i = TXTR_RECT_GR_L;
        i++;
    }
    return (SUCCESS);
}

unsigned short			sel_item_btn(t_button *btn, t_itemfull *itemfull, int n_itemfull)
{
	t_rec				button_box;
	int 				i;

	if (!btn || !itemfull)
		return (FAIL);
	button_box.w = WIN_W * 0.4;
	button_box.h = WIN_H * 0.9 / 25;
	if (n_itemfull > 25)
	{
		button_box.x = WIN_W * 0.1;
		button_box.y = WIN_H * 0.05;
	}
	else
	{
		button_box.x = WIN_W * 0.3;
		button_box.y = (WIN_H - button_box.h * n_itemfull) / 2;
	}
	i = 0;
	while (i < n_itemfull)
	{
		btn[i].vis_lit_on[0] = TRUE;
		btn[i].reg_i = TXTR_RECT_GR;
		btn[i].lit_i = TXTR_RECT_GR_L;
		if (itemfull[i].filename)
			btn[i].text = ft_strdup(itemfull[i].filename);
		btn[i].box = button_box;
		button_box.y += button_box.h;
		if (i == 25)
		{
			button_box.x = WIN_W * 0.5;
			button_box.y = WIN_H * 0.05;
		}
		i++;
	}
	return (SUCCESS);
}

void                    refresh_level_list(t_media *media, t_mode *mode)
{
	if (!media || !mode)
		return ;
    if (mode->n_btn == media->n_worlds + 1)
        return ;
    if (mode->btn)
		free_btn(mode->btn, mode->n_btn);
    mode->n_btn = (media->n_worlds + 1) * 3;
    mode->btn = init_btn(mode->n_btn);
    if (!mode->btn)
        return ;
    levels_btn(mode->btn, media->worlds, media->n_worlds);
}

unsigned short			init_modes(t_media *media, t_prog *prog)
{
	t_mode				*modes;
	int 				i;

	if (!media || !prog)
		return (FAIL);
	modes = (t_mode *)ft_memalloc(sizeof(t_mode) * N_MODES);
	if (!modes)
		return (FAIL);
	ft_bzero(modes, sizeof(t_mode) * N_MODES);
	prog->modes = modes;
	prog->modes[MODE_MAIN_MENU].input = &i_mainmenu;
	prog->modes[MODE_MAIN_MENU].update = &u_mainmenu;
	prog->modes[MODE_MAIN_MENU].render = &r_mainmenu;

	prog->modes[MODE_LEVELS].input =   &i_levels;
	prog->modes[MODE_LEVELS].update = &u_levels;
	prog->modes[MODE_LEVELS].render = &r_levels;

	prog->modes[MODE_EDITOR].input =   &i_editor;
	prog->modes[MODE_EDITOR].update = &u_editor;
	prog->modes[MODE_EDITOR].render = &r_editor;

    prog->modes[MODE_TEXTURES].input =   &i_textures;
    prog->modes[MODE_TEXTURES].update = &u_textures;
    prog->modes[MODE_TEXTURES].render = &r_textures;

	prog->modes[MODE_SEL_ITEM].input =   &i_sel_item;
	prog->modes[MODE_SEL_ITEM].update = &u_sel_item;
	prog->modes[MODE_SEL_ITEM].render = &r_sel_item;

	prog->modes[MODE_MAIN_MENU].n_btn = N_MM_BTNS;
	prog->modes[MODE_LEVELS].n_btn = (media->n_worlds + 1) * 3;
	prog->modes[MODE_EDITOR].n_btn = 8;
    prog->modes[MODE_TEXTURES].n_btn = media->n_txtrs;
	prog->modes[MODE_MAIN_MENU].n_btn = N_MM_BTNS;
	prog->modes[MODE_LEVELS].n_btn = (media->n_worlds + 1) * 3;
	prog->modes[MODE_EDITOR].n_btn = 18;
	prog->modes[MODE_SEL_ITEM].n_btn = media->n_itemfull;
	i = 0;
	while (i < N_MODES)
	{
		prog->modes[i].btn = NULL;
		if (prog->modes[i].n_btn > 0 && !(prog->modes[i].btn = init_btn(prog->modes[i].n_btn)))
			return (FAIL);
		i++;
	}
	main_menu_btn(prog->modes[MODE_MAIN_MENU].btn);
	levels_btn(prog->modes[MODE_LEVELS].btn, media->worlds, media->n_worlds);
    textures_btn(prog->modes[MODE_TEXTURES].btn, media->txtrs, media->n_txtrs);
	sel_item_btn(prog->modes[MODE_SEL_ITEM].btn, media->itemfull, media->n_itemfull);
	return (SUCCESS);
}