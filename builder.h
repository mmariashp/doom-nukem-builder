

#ifndef BUILDER_H
#define BUILDER_H

# include "./libft/libft.h"
# include "get_next_line.h"

# ifdef		__APPLE__
# include <SDL.h>
# include <SDL_image.h>
# include <SDL_ttf.h>
# elif		__linux
# include <SDL2/SDL.h>
# include <SDL2/SDL_image.h>
# include <SDL2/SDL_ttf.h>
# include <SDL2/SDL_mixer.h>
# endif

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

# define SOFT_ORANGE			0xF4511E
# define SOFT_TEAL				0x00897B

# define LIT_COLOR				WHITE

# define CONVEX_COLOR			0x207561
# define CONCAVE_COLOR			0xDA4302
# define ACTIVE_SECTOR_COLOR	0x3949AB

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
# define FONT_NAME              "BebasNeue-Regular.ttf"
//# define FONT_NAME "LibreBaskerville-Regular.ttf"
# define EDIT_TEXT_COLOR		DARK_GRAY

# define LEVELS					"Levels"
# define TEXTURES				"Textures"
# define ITEMS					"Items"
# define SOUNDS					"Sounds"
# define FONTS					"Fonts"
# define TOTAL_SECTIONS			5

# define MAX_LEVELS				5

# define R_VECTORS				1
# define R_WALLS				2
# define R_SECTORS				3
# define R_PLAYER				4
# define R_TEXTURES				5

# define MIN_VERTEX_COORD		0
# define MAX_VERTEX_COORD		200

# define MIN_VERTEX_ID			0
# define MAX_VERTEX_ID			200

# define MIN_N_WALLS			0
# define MAX_N_WALLS			127

# define MIN_N_SECTORS			1
# define MAX_N_SECTORS			50

# define MIN_N_TXTRS			1
# define MAX_N_TXTRS			50

# define MIN_HEIGHT				-100
# define MAX_HEIGHT				100

# define MIN_SECTOR_WALLS		3
# define MAX_SECTOR_WALLS		30

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

# define NODE_EMPTY			-1
# define NODE_FULL			-2
# define NODE_SECTOR		-3
# define NODE_WALL  		-4

# define MENU_TXT_H			25
# define MAX_MENU_TXT_W		200

# define N_MODES			4
# define MODE_MAIN_MENU		0
# define MODE_SUMMARY		1
# define MODE_EDITOR		2
# define MODE_TEXTURES		3

# define N_MM_BTNS		2
# define N_SUMM_BTNS		3
# define N_EDIT_BTNS		3

# define DRAG_BTN		0
# define DRAW_BTN		1
# define DISTORT_BTN		2
# define DELETE_BTN		3
# define SECTOR_BTN		4
# define WALL_BTN		5
# define PLAYER_BTN		6
# define SAVE_BTN		7
# define BACK_BTN		8

# define DESELECT_BTN	0
# define F_UP_BTN		1
# define F_DOWN_BTN		2
# define C_UP_BTN		3
# define C_DOWN_BTN		4
# define FT_EDIT_BTN		5
# define CT_EDIT_BTN		6

# define WT_EDIT_BTN		1
# define W_PORTAL_BTN    2
# define W_DOOR_BTN      3


# define GRID_SIZE			150


# define SEC_CONVEX_CLOSED		1
# define SEC_CONCAVE_CLOSED		0
# define SEC_OPEN				-1

# define WALL_RADIUS	1
# define LIT_WALL_RADIUS	4

# define F_ZOOM					0
# define F_MOVE_GRID			1
# define F_SELECT_NODE			2
# define F_REDRAW				3

// for selected function

# define V_SELECT		0
# define W_SELECT		1
# define S_SELECT		2
# define T_SELECT		3
# define BUTT_SELECT	4
# define STATE_SELECT	5
# define WORLD_SELECT	6

// states
# define NORMAL					-1
# define SECTOR_SEARCH			0
# define SECTOR_EDIT			1
# define VECTOR_SEARCH			2
# define VECTOR_EDIT			3
# define WALL_SEARCH			4
# define WALL_EDIT				5


//item types

# define COIN					0
# define KEY					1
# define OBJECT					2
# define ENEMY					3
# define SUPER_BONUS			4
# define HEALTH					5
# define AMMO					6
# define TOTAL_TYPES			7

typedef struct					s_vec2d
{
	int							x;
	int							y;
}								t_vec2d;

typedef struct                  s_vecd2_d
{
    double                      x;
    double                      y;
}                               t_vec2d_d;

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
	signed char 				nodes[GRID_SIZE][GRID_SIZE];
	float 						scale;
    t_vec2d						active[2];
}								t_grid;

typedef struct 					s_button
{
	char 						vis_lit_on[3];
	t_rec						box;
	int 						text_color;
	char 						*text;
	SDL_Texture					*txtr;
	SDL_Texture					*lit;
}								t_button;

typedef struct					s_sdl
{
	SDL_Window					*window;
	SDL_Renderer				*rend;
	t_vec2d						mouse;
}								t_sdl;

typedef struct					s_prog
{
	int 						**screen;
	int 						mode_id;
	int 						last_mode_id;
	struct s_mode				*modes;
	short 						button_lit;
	short 						button_on;
	short 						zoom;
    t_vec2d						move;
	t_vec2d						click;
	char 						features[10];
	char 						save;
}								t_prog;

typedef struct                  s_wall
{
	int 						v1;
	int 						v2;
	signed char 				type;
	short						txtr;
	signed char                 door;
}                               t_wall;

typedef struct					s_item
{
	t_vec2d						p;
	int 						id;
}								t_item;

typedef struct                  s_sector
{
	int 						*sec_walls;
	int 						*v;
	int						    floor;
	int						    ceiling;
	short 						floor_txtr;
	short 						ceil_txtr;
	t_item						*items;
	unsigned short				n_items;
	unsigned short				n_walls;
    unsigned short				n_v;
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
	t_vec2d                     size;
}                               t_texture;

typedef struct					s_world
{
	char 						*filename;
	char 						*full_path;
	int							*textures;
	t_sector					*sec;
	t_wall						*walls;
	t_vec2d						*vertices;
	t_vec2d						p_start;
	t_vec2d						p_end;
	short unsigned				n_sec;
	short unsigned				n_vectors;
	short unsigned				n_walls;
	short unsigned				n_txtrs;
}								t_world;

typedef struct 					s_section
{
	short 						id;
	char 						*path;
	char						*extension;
	short 						n_files;
	char 						**tab;
	char 						**names;
	int 						*extra;
}								t_section;

typedef struct					s_itemfull
{
	char 						*filename;
	char 						*full_path;
	int 						type;
}								t_itemfull;

typedef struct 					s_media
{
	short 						world_id;
	char 						paths[TOTAL_SECTIONS][20];
	char 						extensions[TOTAL_SECTIONS][10];
	t_texture					*txtrs;
	char						**fonts;
	char						**sounds;
	t_world						*worlds;
	t_itemfull					*itemfull;
	short unsigned				n_itemfull;
	short unsigned				n_worlds;
	short unsigned				n_txtrs;
	short unsigned				n_fonts;
	short unsigned				n_sounds;
}								t_media;

typedef struct					s_mode
{
	unsigned short				n_buttons;
	t_button					*buttons;
	int							(*input)(t_sdl*, t_grid*, t_media *, t_prog *);
	unsigned short				(*update)(t_sdl*, t_grid*, t_media *, t_prog *);
	void						(*render)(t_sdl*, t_grid*, t_media *, t_prog *);
}								t_mode;

typedef struct 					s_value
{
	char 						*text;
	SDL_Texture					*texture;
}								t_value;

int						start_sdl(t_sdl *sdl);
t_sdl					*get_sdl(void);
SDL_Texture             *load_texture(char *name, SDL_Renderer *rend, t_vec2d *size);
SDL_Surface             *load_image(char *name);
void					free_sdl(t_sdl *sdl);
void					quit_sdl(void);
void					error(char *reason);
void					my_error(char *reason);
void					end(char *reason);

void					render_main_menu(t_sdl *sdl, t_grid *grid, t_media *media, t_prog *prog);
unsigned short					update_main_menu(t_sdl *sdl, t_grid *grid, t_media *media, t_prog *prog);
int						input_main_menu(t_sdl *sdl, t_grid *grid, t_media *media, t_prog *prog);
void					render_summary(t_sdl *sdl, t_grid *grid, t_media *media, t_prog *prog);
unsigned short					update_summary(t_sdl *sdl, t_grid *grid, t_media *media, t_prog *prog);
int						input_summary(t_sdl *sdl, t_grid *grid, t_media *media, t_prog *prog);

int						input_editor(t_sdl *sdl, t_grid *grid, t_media *media, t_prog *prog);
unsigned short					update_editor(t_sdl *sdl, t_grid *grid,  t_media *media, t_prog *prog);
void					render_editor(t_sdl *sdl, t_grid *grid, t_media *media, t_prog *prog);

int						input_textures(t_sdl *sdl, t_grid *grid, t_media *media, t_prog *prog);
unsigned short					update_textures(t_sdl *sdl, t_grid *grid,  t_media *media, t_prog *prog);
void					render_textures(t_sdl *sdl, t_grid *grid, t_media *media, t_prog *prog);

int						input_walls(t_sdl *sdl, t_grid *grid, t_media *media, t_prog *prog);
unsigned short					update_walls(t_sdl *sdl, t_grid *grid,  t_media *media, t_prog *prog);
void					render_walls(t_sdl *sdl, t_grid *grid, t_media *media, t_prog *prog);


void					write_text(char *str, SDL_Renderer *rend, t_rec rec, int color, char h_center);

int						clamp(int n, int min, int max);
float					clamp_f(float n, float min, float max);
t_vec2d					add(t_vec2d one, t_vec2d two);
t_vec2d					mult(t_vec2d one, t_vec2d two);
t_vec2d					scale(t_vec2d vec, int scale);
int                     get_max(int one, int two);
int                     get_min(int one, int two);

t_media					*get_assets(void);
unsigned 				close_file(int fd);
void					free_media(t_media *media);
unsigned short			rewrite_media(t_media *media);
unsigned short			rewrite_levels(t_media *media);

void					draw_dot(int x, int y, int color, SDL_Renderer *rend);
void					draw_circle_fill(t_vec2d c, int radius, int color, SDL_Renderer *rend);
void				    get_rgb(unsigned char *r, unsigned char *g, unsigned char *b, int color);
void				    draw_line(t_line l, int color, SDL_Renderer *rend);

void				    draw_line_grid(t_line l, char c, signed char nodes[GRID_SIZE][GRID_SIZE]);
//void					draw_dot_grid(int x, int y, int color, char **nodes);

unsigned short			init_modes(t_sdl *sdl, t_media *media, t_prog *prog);
void					free_modes(t_mode *modes, t_sdl *sdl);
void                    refresh_level_list(t_media *media, t_mode *mode, t_sdl *sdl);

void					render_buttons(t_button *buttons, SDL_Renderer *rend, int n_buttons);
void					render_button(t_button *button, SDL_Renderer *rend);
unsigned short			mouse_over(t_rec box, t_vec2d mouse);

void					render_frame(t_rec rec, int color, SDL_Renderer *rend);

unsigned short			light_button(t_sdl *sdl, t_button *buttons, int n_buttons, t_prog *prog);

void					render_grid(t_world world, t_grid *grid, t_prog *prog, t_vec2d mouse);


void					draw_dot2(int x, int y, int color, int **screen);
void				    draw_line2(t_line l, int color, int **screen);
void					draw_circle_fill2(t_vec2d c, int radius, int color, int **screen);
void					draw_node(t_vec2d c, int r, int color, int **screen);
void				    draw_thick_line(t_line l, int color, int r, int **screen);

void					fillpoly(t_vec2d *p, int polyCorners, int **screen, int color);

int 					fill_sector_v(t_sector *sector, t_wall *walls, int n);
int 					sector_closed(int *tmp, int n);
void					pair_sort(int *a, int n);

SDL_Texture				*button_back(int id, int set_get_free, SDL_Renderer *rend);

void					free_int_tab(int **tab, int size);

void 					free_prog(t_prog *prog, t_sdl *sdl);
t_prog					*get_prog(void);

short 					find_vector(t_vec2d *vertices, t_vec2d p, int n);
short 					find_wall(short one, short two, t_wall *walls, short n_walls);

void					render_screen(SDL_Renderer *rend, int **screen);

void					fill_grid(int n_vectors, t_vec2d *vertices, t_grid *grid);
void					clean_grid(t_grid *grid);
void                    fill_grid_walls(int n_walls, t_wall *walls, int n_vectors, t_vec2d *vertices, t_grid *grid);

void					add_to_media(t_grid *grid, t_media *media);
unsigned short			add_world(t_world **worlds, short n_worlds, char *ext, char *path);

void					update_sector_status(t_sector *sec, t_wall *walls, t_vec2d *vertices, int n_sec);
int 					in_sector(t_vec2d p, t_world *world, t_grid *grid);

void					delete_vector(int id, t_world *world);

void					move_grid_drag(t_prog *prog, t_vec2d mouse, t_grid *grid);
unsigned short			move_grid_keys(t_prog *prog, t_grid *grid);
void					zoom_grid(t_prog *prog, t_vec2d mouse, t_grid *grid);
void                    zoom_to_sector(t_sector *sector, t_vec2d *vertices, t_grid *grid, t_prog *prog);
void                    zoom_to_map(int n_vectors, t_vec2d *v, t_grid *grid);
void                    zoom_to_wall(t_vec2d v1, t_vec2d v2, t_grid *grid, t_prog *prog);

void					move_vector(t_prog *prog, t_vec2d mouse, t_grid *grid, t_world *world);

t_vec2d					find_node(int p_x, int p_y, t_grid *grid);

t_vec2d                 make_iso(int x, int y, int z);





int                      texture_in_world(int id, t_world world);
int					    *realloc_textures(int *textures, int n);
unsigned short			add_texture(int **textures, short n_txtrs, int id);
unsigned short			add_wall(t_wall **walls, short n_walls, int one, int two);

//useful
unsigned short			within(int value, int min, int max);
int 					selected_item(char set_get_unset, unsigned short id, int value);
int 					lit_item(char set_get_unset, unsigned short id, int value);


// edit menu
char 					*menu_lines(int id, int i);
void					render_edit_menu(SDL_Renderer *r, t_texture *txtrs, t_world *w, int state);
t_rec                   layout_menu(char i, char n);

//font
TTF_Font				*set_get_free_font(char set_get_free);

//values
void					render_values(int state, int n, t_value *values, SDL_Renderer *rend);
void					free_values(t_value *values, int n);
t_value					*init_values(int n);

void					render_box(t_rec box, SDL_Texture *t, SDL_Renderer *rend);

// buttons
t_button				*init_buttons(int n_buttons);
void					free_buttons(t_button *buttons, int n);
t_button				*set_get_free_buttons(char set_get_free, int *n, int state);
unsigned short			distribute_buttons_h(t_button *buttons, int from, int nb, t_rec box, int padding);
void					get_buttons(int state, t_mode *mode, SDL_Renderer *rend);

//door
void                    delete_door(t_world *world, int id);
void                    add_door(t_world *world, int id);
void					delete_wall(int id, t_world *world);
unsigned short			add_wall_in_secs(t_world *world, int to_add, int find);

void					move_player(t_prog *prog, t_vec2d mouse, t_grid *grid, t_world *world);

void					grid_refresh(t_grid *grid, t_media *media);

#endif
