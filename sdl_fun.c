
#include "builder.h"

void					quit_sdl(void)
{
	ft_putendl("in quit_sdl");
	if (TTF_WasInit())
		TTF_Quit();
	IMG_Quit();
	SDL_Quit();
	ft_putendl("out of quit_sdl");
}

void					free_int_tab(int **tab, int size)
{
	int					i;

	if(!tab)
		return ;
	i = 0;
	while(i < size && tab[i] != NULL)
		ft_memdel((void **)&tab[i++]);
	ft_memdel((void **)&tab);
}

void					free_sdl(t_sdl *sdl)
{
	ft_putendl("in free_sdl");
	if (!sdl)
		return ;
	if (sdl->rend)
	{
		SDL_DestroyRenderer(sdl->rend);
		sdl->rend = NULL;
		ft_putendl("destroyed renderer");
	}
	if (sdl->window)
	{
		SDL_DestroyWindow(sdl->window);
		sdl->window = NULL;
		ft_putendl("destroyed window");

	}
	if (sdl->font)
	{
		TTF_CloseFont( sdl->font );
		sdl->font = NULL;
		ft_putendl("closed font");

	}
	free(sdl);
	ft_putendl("out of free sdl");
	quit_sdl();
}

SDL_Surface             *load_image(char *name)
{
	SDL_Surface     *image;

	if (!name)
		return (NULL);
	image = IMG_Load(name);
	if (image == NULL)
	{
		ft_putendl(IMG_GetError());
	}
	return (image);
}

SDL_Texture             *load_texture(char *name, SDL_Renderer *rend, t_vec2d *size)
{
	SDL_Texture		*texture;
	SDL_Surface     *image;

	texture = NULL;
	image = load_image(name);
	if (size)
	    *size = (t_vec2d){ image->w, image->h };
	if (image == NULL)
		return (NULL);
	texture = SDL_CreateTextureFromSurface(rend, image);
	if (texture == NULL)
		ft_putendl(SDL_GetError());
	SDL_FreeSurface(image);
	return (texture);
}

int						start_sdl(t_sdl *sdl)
{
	int					img_flag;

	img_flag = IMG_INIT_PNG;
	printf("in start sdl\n");
	if(SDL_Init(SDL_INIT_VIDEO) != 0)
		return(FAIL);
	if (!(IMG_Init(img_flag) & img_flag))
		return (FAIL);
	if (TTF_Init() != 0)
		return(FAIL);
	if (!(sdl->window = SDL_CreateWindow(WIN_NAME, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WIN_W, WIN_H, SDL_WINDOW_SHOWN | SDL_WINDOW_MOUSE_FOCUS)))
		return(FAIL);
	if (!(sdl->rend = SDL_CreateRenderer(sdl->window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC)))
		return(FAIL);
	if (!(sdl->font = TTF_OpenFont( FONT_NAME, 30 )))
		return (FAIL);
	printf("out of start sdl\n");
	return (SUCCESS);
}

t_sdl					*get_sdl(void)
{
	t_sdl				*sdl;

	if (!(sdl = (t_sdl*)ft_memalloc(sizeof(t_sdl))))
		return (NULL);
	*sdl = (t_sdl){ };
	sdl->window = NULL;
	sdl->rend = NULL;
	if (start_sdl(sdl) == FAIL)
	{
		ft_putendl("couldn't start sdl");
		free_sdl(sdl);
		return (NULL);
	}
	sdl->mouse = (t_vec2d){ 0, 0 };
	return (sdl);
}
