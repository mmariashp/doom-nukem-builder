

#ifndef BUILDER_H
#define BUILDER_H

# include "./libft/libft.h"
# include "get_next_line.h"

# include <SDL2/SDL.h>
# include <SDL2/SDL_image.h>
# include <SDL2/SDL_ttf.h>
# include <SDL2/SDL_mixer.h>

# include <stdlib.h>
# include <stdio.h> // TO DELETE AFTER
# include <string.h>
# include <math.h>
# include <fcntl.h>
# include <errno.h>
# include <unistd.h>
# include <sys/stat.h>
# include <sys/types.h>


# define SUCCESS				0
# define FAIL					1

# define TRUE					1
# define FALSE					0


# define C_N_EMPTY				0
# define C_N_FULL				1
# define C_W_EMPTY				2
# define C_W_FULL				3

# define WHITE					0xffffff
# define GRAY					0xa9a9a9
# define PURPLE					0x621295
# define RED					0xc40b13
# define ORANGE					0xf69314
# define LIGHT_GRAY				0xdadddf
# define MILKY					0xffffd2
# define BABY_PINK				0xfcbad3
# define SOFT_PURPLE			0xAA96DA
# define SOFT_BLUE				0xa8d8ea
# define DARK_GRAY				0x222831
# define DARK_ORANGE			0xb55400
# define SOFT_GRAY				0x393e46
# define ALMOST_WHITE			0xeeeeee
# define DARK_PURPLE			0x681e21
# define YELLOW					0xfff000
# define BROWN					0x794913
# define NAVY					0x000080
# define GREEN					0x4AA21E

# define LIT_COLOR				0x5A982B   //green

/*
** STR = PRINTS OUT NAME OF MACRO; XSTR = PRINTS OUT VALUE OF MACRO
*/

# define STR(x)					#x
# define XSTR(x)				STR(x)

# define WIN_W					1000
# define WIN_H					850

#if SDL_BYTEORDER == SDL_BIG_ENDIAN
# define RMASK 0xff000000
# define GMASK 0x00ff0000
# define BMASK 0x0000ff00
# define AMASK 0x000000ff
#else
# define RMASK 0x000000ff
# define GMASK 0x0000ff00
# define BMASK 0x00ff0000
# define AMASK 0xff000000
#endif

# define WIN_NAME				"DOOM-NUKEM BUILDER"
# define FONT_NAME              "VCR_OSD_MONO_1.001.ttf"
//# define FONT_NAME "LibreBaskerville-Regular.ttf"

# define LEVELS					"Levels"
# define TEXTURES				"Textures"
# define SOUNDS					"Sounds"
# define FONTS					"Fonts"
# define TOTAL_SECTIONS			4

# define R_VECTORS				1
# define R_WALLS				2
# define R_SECTORS				3
# define R_PLAYER				4
# define R_TEXTURES				5

# define MIN_VERTEX_COORD		0
# define MAX_VERTEX_COORD		200

# define MIN_VERTEX_ID			0
# define MAX_VERTEX_ID			200

# define MIN_N_WALLS			3
# define MAX_N_WALLS			100

# define MIN_N_SECTORS			1
# define MAX_N_SECTORS			50

# define MIN_N_TEXTURES			1
# define MAX_N_TEXTURES			50

# define MIN_HEIGHT				-100
# define MAX_HEIGHT				100

# define MIN_SECTOR_WALLS		3
# define MAX_SECTOR_WALLS		20

//wall types
# define WALL_EMPTY					0
# define WALL_FILLED				1
# define WALL_DOOR					2

# define ASSET_FILE             "./asset_paths.txt"

# define MEMORY_ERR				"Error in memory."

/*
** macros for line clipping
 */
# define REGION_INSIDE      (int)0 // 0000
# define REGION_LEFT        (int)1   // 0001
# define REGION_RIGHT       (int)2  // 0010
# define REGION_BOTTOM      (int)4 // 0100
# define REGION_TOP         (int)8    // 1000

# define NODE_EMPTY			0
# define NODE_FULL			1

# define MAIN_MENU			0
# define SUMMARY			1
# define LEVEL_EDIT			2
# define SMTH				3

# define MENU_TXT_H			25
# define MAX_MENU_TXT_W		200

# define N_MODES			3
# define MODE_MAIN_MENU		0
# define MODE_SUMMARY		1
# define MODE_EDITOR		2

# define N_MM_BUTTONS		2
# define N_SUMM_BUTTONS		3
# define N_EDIT_BUTTONS		3

# define GRID_SIZE			150


# define SEC_CONVEX_CLOSED		1
# define SEC_CONCAVE_CLOSED		0
# define SEC_OPEN				-1

typedef struct					s_vec2d
{
	int							x;
	int							y;
}								t_vec2d;

typedef struct					s_poly
{
	int 						area;
	int 						color;
	t_vec2d						*a;
	t_vec2d						*b;
	t_vec2d						*c;
	t_vec2d						*d;
}								t_poly;

typedef struct                  s_line
{
	t_vec2d                     p0;
	t_vec2d                     p1;
}                               t_line;

typedef struct                  s_rec
{
	int 						x;
	int 						y;
	int 						w;
	int 						h;
}                               t_rec;

typedef struct 					s_grid
{
	t_rec						box;
	char 						nodes[GRID_SIZE][GRID_SIZE];
	float 						scale;
}								t_grid;

typedef struct					s_t
{
	t_grid						grid;
	t_vec2d						active[2];
}								t_t;

typedef struct 					s_button
{
	char 						visible;
	t_rec						box;
	unsigned short				is_lit;
	int 						text_color;
	char 						*text;
	SDL_Texture					*texture;
	SDL_Texture					*back;
	SDL_Texture					*lit_back;
	SDL_Texture					*front;
	int 						background;
}								t_button;

# define F_ZOOM					0
# define F_MOVE_GRID			1
# define F_SELECT_NODE			2
# define F_REDRAW				3


typedef struct					s_sdl
{
	TTF_Font                    *font;
	SDL_Window					*window;
	SDL_Renderer				*rend;

	int 						**screen;
	t_vec2d						mouse;
	int 						mode_id;
	struct s_mode				*modes;
	short 						button_lit;
	short 						button_on;
	short 						zoom;
	t_vec2d						move;

	char 						features[10];
	char 						save;
}								t_sdl;

typedef struct                  s_wall
{
	int 						v1;
	int 						v2;
	int 						type;
	short						txtr;
}                               t_wall;

typedef struct                  s_sector
{
	int 						*sec_walls;
	int 						*v;
	int						    floor;
	int						    ceiling;
	short 						floor_txtr;
	short 						ceil_txtr;
	unsigned short				n_walls;
	char 						status;
}                               t_sector;

typedef struct                  s_sound
{
	short 						id;
	char						*sdl_sound; // ???
}                               t_sound;

typedef struct                  s_font
{
	short 						id;
	char						*sdl_font; // TTF_Font
}                               t_font;

typedef struct                  s_texture
{
	char 						*name;
	char 						*full_path;
	SDL_Texture					*sdl_t;
}                               t_texture;

typedef struct					s_world
{
	char 						*filename;
	char 						*full_path;
	int							*textures;
	t_sector					*sectors;
	t_wall						*walls;
	t_vec2d						*vertices;
	t_vec2d						p_start;
	t_vec2d						p_end;
	short unsigned				n_sectors;
	short unsigned				n_vectors;
	short unsigned				n_walls;
	short unsigned				n_textures;
}								t_world;

typedef struct 					s_section
{
	short 						id;
	char 						*path;
	char						*extension;
	short 						n_files;
	char 						**tab;
	char 						**names;
}								t_section;

typedef struct 					s_media
{
	short 						world_id;
	char 						paths[4][20];
	char 						extensions[4][10];
	t_texture					*txtrs;
	char						**fonts;
	char						**sounds;
	t_world						*worlds;
	short unsigned				n_worlds;
	short unsigned				n_textures;
	short unsigned				n_fonts;
	short unsigned				n_sounds;
}								t_media;

typedef struct					s_mode
{
	unsigned short				n_buttons;
	t_button					*buttons;
	int							(*input)(t_sdl*, float*, t_media *);
	void						(*update)(t_sdl*, t_t*, t_media *);
	void						(*render)(t_sdl*, t_t*, t_media *);
}								t_mode;

int						start_sdl(t_sdl *sdl);
t_sdl					*get_sdl(void);
SDL_Texture             *load_texture(char *name, t_sdl *sdl);
SDL_Surface             *load_image(char *name);
void					free_sdl(t_sdl *sdl);
void					quit_sdl(void);
void					error(char *reason);
void					my_error(char *reason);
void					end(char *reason);

void					render_main_menu(t_sdl *sdl, t_t *t, t_media *media);
void					update_main_menu(t_sdl *sdl, t_t *t, t_media *media);
int						input_main_menu(t_sdl *sdl, float *grid_scale, t_media *media);
void					render_summary(t_sdl *sdl, t_t *t, t_media *media);
void					update_summary(t_sdl *sdl, t_t *t, t_media *media);
int						input_summary(t_sdl *sdl, float *grid_scale, t_media *media);

int						input_editor(t_sdl *sdl, float *grid_scale, t_media *media);
void					update_editor(t_sdl *sdl, t_t *t,  t_media *media);
void					render_editor(t_sdl *sdl, t_t *t, t_media *media);

void					write_text(char *str, t_sdl *sdl, int max_w, int h, t_vec2d pos, int color);

int						clamp(int n, int min, int max);
float					clamp_f(float n, float min, float max);
t_vec2d					add(t_vec2d one, t_vec2d two);
t_vec2d					mult(t_vec2d one, t_vec2d two);
t_vec2d					scale(t_vec2d vec, int scale);

t_media					*get_assets(void);
unsigned 				close_file(int fd);
void					free_media(t_media *media);
void					rewrite_media(t_media *media);

//void					draw_dot_surf(int x, int y, int color, SDL_Surface *surf);
//void				    draw_line_surf(t_line l, int color, SDL_Surface *surf);
//void					draw_circle_fill_surf(t_vec2d c, int radius, int color, SDL_Surface *surf);

void					draw_dot(int x, int y, int color, SDL_Renderer *rend);
void					draw_circle_fill(t_vec2d c, int radius, int color, SDL_Renderer *rend);
void				    get_rgb(unsigned char *r, unsigned char *g, unsigned char *b, int color);
void				    draw_line(t_line l, int color, SDL_Renderer *rend);

unsigned short			init_modes(t_sdl *sdl, t_media *media);
void					free_modes(t_mode *modes);

void					render_buttons(t_button *buttons, t_sdl *sdl);
void					render_button(t_button *button, t_sdl *sdl);
unsigned short			distribute_buttons(t_button *buttons, int nb, t_rec box);
unsigned short			mouse_over(t_rec box, t_vec2d mouse);

void					render_frame(t_rec rec, int color, SDL_Renderer *rend);

unsigned short			light_button(t_sdl *sdl);

void					render_grid(t_world world, t_t *t, t_sdl *sdl);


void					draw_dot2(int x, int y, int color, int **screen);
void				    draw_line2(t_line l, int color, int **screen);
void					draw_circle_fill2(t_vec2d c, int radius, int color, int **screen);


void fillpoly(t_vec2d *p, int polyCorners, int **screen, int color);

int 					fill_sector_v(t_sector *sector, t_wall *walls, int n);
int 					sector_closed(int *tmp, int n);
void					pair_sort(int *a, int n);

#endif
