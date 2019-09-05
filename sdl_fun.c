
#include "builder.h"

void					quit_sdl(void)
{
	ft_putendl("in quit_sdl");
	if (TTF_WasInit())
		TTF_Quit();
	IMG_Quit();
	if(SDL_WasInit(SDL_INIT_AUDIO))
		SDL_AudioQuit();
	SDL_Quit();
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
	}
	if (sdl->window)
	{
		SDL_DestroyWindow(sdl->window);
		sdl->window = NULL;
	}
	if (sdl->font)
	{
		TTF_CloseFont( sdl->font );
		sdl->font = NULL;
	}
	if (sdl->modes)
		free_modes(sdl->modes);
	ft_memdel((void**)&sdl);
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

SDL_Texture             *load_texture(char *name, t_sdl *sdl)
{
	SDL_Texture		*texture;
	SDL_Surface     *image;

	texture = NULL;
	image = load_image(name);
	if (image == NULL)
		return (NULL);
	texture = SDL_CreateTextureFromSurface(sdl->rend, image);
	if (texture == NULL)
		ft_putendl(SDL_GetError());
	SDL_FreeSurface(image);
	return (texture);
}

//SDL_Surface				*get_surf(int w, int h)
//{
////	return(SDL_CreateRGBSurface(0, w, h, 32, RMASK, GMASK, BMASK, 0));
//
//	return(SDL_CreateRGBSurface(0, w, h, 32, 0, 0, 0, 0));
//
//}

int						start_sdl(t_sdl *sdl)
{
	int					img_flag;

	img_flag = IMG_INIT_PNG;
	atexit(quit_sdl);
	if(SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) != 0)
		return(FAIL);
	if (!(IMG_Init(img_flag) & img_flag))
		return (FAIL);
	if (TTF_Init() != 0)
		return(FAIL);
	if (!(sdl->window = SDL_CreateWindow(WIN_NAME, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WIN_W, WIN_H, SDL_WINDOW_SHOWN | SDL_WINDOW_MOUSE_FOCUS)))
		return(FAIL);
	if (!(sdl->rend = SDL_CreateRenderer(sdl->window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC | SDL_RENDERER_TARGETTEXTURE)))
		return(FAIL);
	sdl->font = TTF_OpenFont( FONT_NAME, 28 );
	if (!sdl->font)
		return (FAIL);
	return (SUCCESS);
}

t_sdl					*get_sdl(void)
{
	t_sdl				*sdl;

	if (!(sdl = (t_sdl*)ft_memalloc(sizeof(t_sdl))))
		return (NULL);
	*sdl = (__typeof__(*sdl)){ };
	if (start_sdl(sdl) == FAIL)
	{
		ft_putendl("couldn't start sdl");
		free_sdl(sdl);
		return (NULL);
	}
	sdl->mouse = (__typeof__(sdl->mouse)){ 0, 0 };
	sdl->mode_id = MAIN_MENU;
	sdl->zoom = 0;
	return (sdl);
}
