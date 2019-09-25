
#include "builder.h"

void					quit_sdl(void)
{
	if (TTF_WasInit())
		TTF_Quit();
	IMG_Quit();
	SDL_Quit();
}

void					free_sdl(t_sdl *sdl)
{
	if (!sdl)
		return ;
	if (sdl->rend)
	{
		SDL_DestroyRenderer(sdl->rend);
		sdl->rend = NULL;
	}
	if (sdl->window)
	{
		SDL_DestroyWindow(sdl->window);
		sdl->window = NULL;
	}
	if (sdl->font)
	{
		TTF_CloseFont(sdl->font);
	}
	free(sdl);
	sdl = NULL;
	quit_sdl();
}

SDL_Texture             *load_texture(char *name, SDL_Renderer *rend, t_vec2d *size)
{
	SDL_Texture			*texture;
	SDL_Surface			*image;

	if (!name || !rend)
		return (NULL);
	texture = NULL;
	if (!(image = IMG_Load(name)))
	{
		ft_putendl(IMG_GetError());
		return (NULL);
	}
	if (size)
	    *size = (t_vec2d){ image->w, image->h };
	texture = SDL_CreateTextureFromSurface(rend, image);
	SDL_FreeSurface(image);
	if (!texture)
	{
		ft_putendl(SDL_GetError());
		return (NULL);
	}
	return (texture);
}

int						start_sdl(t_sdl *sdl)
{
	int					img_flag;

	img_flag = IMG_INIT_PNG;
	if(SDL_Init(SDL_INIT_VIDEO) != 0)
		return(FAIL);
	if (!(IMG_Init(img_flag) & img_flag))
		return (FAIL);
	if (TTF_Init() != 0)
		return(FAIL);
	if (!(sdl->window = SDL_CreateWindow(WIN_NAME, SDL_WINDOWPOS_CENTERED, \
	SDL_WINDOWPOS_CENTERED, WIN_W, WIN_H, SDL_WINDOW_SHOWN | \
	SDL_WINDOW_MOUSE_FOCUS)))
		return(FAIL);
	if (!(sdl->rend = SDL_CreateRenderer(sdl->window, -1, \
	SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC)))
		return(FAIL);
	if (!(sdl->font = TTF_OpenFont( FONT_NAME, 30 )))
		return (FAIL);
	SDL_SetRenderDrawColor(sdl->rend, 0xFF, 0xFF, 0xFF, 0xFF);
	return (SUCCESS);
}

t_sdl					*get_sdl(void)
{
	t_sdl				*sdl;

	if (!(sdl = (t_sdl*)ft_memalloc(sizeof(t_sdl))))
		return (NULL);
	ft_bzero(sdl, sizeof(t_sdl));
	*sdl = (t_sdl){ };
	sdl->window = NULL;
	sdl->rend = NULL;
	sdl->mouse = (t_vec2d){ 0, 0 };
	if (start_sdl(sdl) == FAIL)
	{
		ft_putendl("couldn't start sdl");
		free_sdl(sdl);
		return (NULL);
	}
	return (sdl);
}
