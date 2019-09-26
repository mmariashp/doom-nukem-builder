

#ifndef BUILDER_H
#define BUILDER_H

# include "./libft/libft.h"
# include "get_next_line.h"

# ifdef		__APPLE__
# include						<SDL.h>
# include						<SDL_image.h>
# include						<SDL_ttf.h>
# elif		__linux
# include						<SDL2/SDL.h>
# include						<SDL2/SDL_image.h>
# include						<SDL2/SDL_ttf.h>
# include						<SDL2/SDL_mixer.h>
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

# ifdef		__APPLE__
# define SCROLL_UP				-1
# define SCROLL_DOWN			1
# elif		__linux
# define SCROLL_UP				1
# define SCROLL_DOWN			-1
# endif

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

# define TMP_COLOR				SOFT_PURPLE

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

# define MIN_SEC_WALLS		3
# define MAX_SEC_WALLS		30

# define MAX_SECTOR_ITEMS		20
# define MAX_ITEMFULL			50

//wall types
# define WALL_EMPTY					0
# define WALL_FILLED				1

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

# define N_MODES			5
# define MODE_MAIN_MENU		0
# define MODE_LEVELS		1
# define MODE_EDITOR		2
# define MODE_TEXTURES		3
# define MODE_SEL_ITEM		4

# define N_MM_BTNS		2

# define TXTR_RECT_GR		0
# define TXTR_RECT_GR_L		1
# define TXTR_RECT_Y		2
# define TXTR_RECT_Y_L		3
# define TXTR_PANEL_GR		4
# define TXTR_PANEL_GR_L	5
# define TXTR_YES			6
# define TXTR_YES_L			7
# define TXTR_NO			8
# define TXTR_NO_L			9
# define TXTR_MOVE			10
# define TXTR_MOVE_L		11
# define TXTR_DRAW			12
# define TXTR_DRAW_L		13
# define TXTR_DISTORT		14
# define TXTR_DISTORT_L		15
# define TXTR_ISO			16
# define TXTR_ISO_L			17
# define TXTR_SECTOR		18
# define TXTR_SECTOR_L		19
# define TXTR_WALL			20
# define TXTR_WALL_L		21
# define TXTR_DOOR_ADD		22
# define TXTR_DOOR_ADD_L	23
# define TXTR_DOOR_DEL		24
# define TXTR_DOOR_DEL_L	25
# define TXTR_PLAYER		26
# define TXTR_PLAYER_L		27
# define TXTR_SAVE			28
# define TXTR_SAVE_L		29
# define TXTR_EXIT			30
# define TXTR_EXIT_L		31
# define TXTR_BACK			32
# define TXTR_BACK_L		33
# define TXTR_DEL			34
# define TXTR_DEL_L			35
# define TXTR_EDIT			36
# define TXTR_EDIT_L		37
# define TXTR_UP			38
# define TXTR_UP_L			39
# define TXTR_DOWN			40
# define TXTR_DOWN_L		41
# define TXTR_COIN			42
# define TXTR_KEY			43
# define TXTR_OBJECT		44
# define TXTR_ENEMY			45
# define TXTR_SUPER			46
# define TXTR_HEALTH		47
# define TXTR_AMMO			48
# define TXTR_LIGHT			49

# define TOTAL_TXTRS		50


# define DRAG_BTN		0
# define DISTORT_BTN	1
# define DRAW_BTN		2
# define SECTOR_BTN		3
# define WALL_BTN		4
# define DOOR_ADD_BTN	5
# define DOOR_DEL_BTN	6
# define PLAYER_BTN		7
# define ISO_BTN		8
# define SAVE_BTN		9
# define BACK_BTN		10

# define DESELECT_BTN	0
# define DEL_SEC_BTN	1
# define F_UP_BTN		2
# define F_DOWN_BTN		3
# define C_UP_BTN		4
# define C_DOWN_BTN		5
# define FT_EDIT_BTN	6
# define CT_EDIT_BTN	7
# define B_ITEM_EDIT	8
# define B_ITEM_DEL		9
# define B_COIN			10
# define B_KEY			11
# define B_OBJECT		12
# define B_ENEMY		13
# define B_SUPER_BONUS	14
# define B_HEALTH		15
# define B_AMMO			16
# define B_LIGHT		17

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
# define SEL_I_SELECT	4
# define ST_SELECT	5
# define WORLD_SELECT	6
# define I_SELECT		7

// states
# define NORMAL					-1
# define SECTOR_SEARCH			0
# define SECTOR_EDIT			1
# define INP					2
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
# define LIGHT					7
# define TOTAL_TYPES			8

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
//	SDL_Texture					*txtr;
//	SDL_Texture					*lit;
	int 						reg_i;
	int 						lit_i;
}								t_button;

typedef struct					s_sdl
{
	SDL_Window					*window;
	SDL_Renderer				*rend;
	t_vec2d						mouse;
	TTF_Font					*font;
}								t_sdl;

typedef struct					s_prog
{
	int 						**screen;
	int 						mode_id;
	int 						last_mode_id;
	struct s_mode				*modes;
	short 						btn_lit;
	short 						btn_on;
	short 						zoom;
    t_vec2d						move;
	t_vec2d						click;
	char 						features[10];
	SDL_Texture					**t;
}								t_prog;

typedef struct                  s_wall
{
	int 						v1;
	int 						v2;
	signed char 				type;
	short						txtr;
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
	char 						is_door;
}                               t_sec;

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
	t_sec						*sec;
	t_wall						*walls;
	t_vec2d						*vecs;
	t_vec2d						p_start;
	t_vec2d						p_end;
	short unsigned				n_sec;
	short unsigned				n_vecs;
	short unsigned				n_walls;
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
	short 						w_id;
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
	unsigned short				n_btn;
	t_button					*btn;
	int							(*input)(t_sdl*, t_grid*, t_media *, t_prog *);
	unsigned short				(*update)(t_sdl*, t_grid*, t_media *, t_prog *);
	void						(*render)(t_sdl*, t_grid*, t_media *, t_prog *);
}								t_mode;

typedef struct 					s_value
{
	char 						*text;
	int 						t_id;
	char 						media_prog;
}								t_value;

/*
 * sdl_fun
 */

int						start_sdl(t_sdl *sdl);
t_sdl					*get_sdl(void);
SDL_Texture             *load_texture(char *name, SDL_Renderer *rend, t_vec2d *size);
void					free_sdl(t_sdl *sdl);
void					quit_sdl(void);

/*
 * main_menu mode
 */

void					r_mainmenu(t_sdl *sdl, t_grid *grid, t_media *media, t_prog *prog);
unsigned short			u_mainmenu(t_sdl *sdl, t_grid *grid, t_media *media, t_prog *prog);
int						i_mainmenu(t_sdl *sdl, t_grid *grid, t_media *media, t_prog *prog);

/*
 * levels mode
 */

void					r_levels(t_sdl *sdl, t_grid *grid, t_media *media, t_prog *prog);
unsigned short			u_levels(t_sdl *sdl, t_grid *grid, t_media *media, t_prog *prog);
int						i_levels(t_sdl *sdl, t_grid *grid, t_media *media, t_prog *prog);

/*
 * editor mode
 */

int						i_editor(t_sdl *sdl, t_grid *grid, t_media *media, t_prog *prog);
unsigned short			u_editor(t_sdl *sdl, t_grid *grid,  t_media *media, t_prog *prog);
void					r_editor(t_sdl *sdl, t_grid *grid, t_media *media, t_prog *prog);

/*
 * textures mode
 */

int						i_textures(t_sdl *sdl, t_grid *grid, t_media *media, t_prog *prog);
unsigned short			u_textures(t_sdl *sdl, t_grid *grid,  t_media *media, t_prog *prog);
void					r_textures(t_sdl *sdl, t_grid *grid, t_media *media, t_prog *prog);

/*
 * sel_item mode
 */

int						i_sel_item(t_sdl *sdl, t_grid *grid, t_media *media, t_prog *prog);
unsigned short			u_sel_item(t_sdl *sdl, t_grid *grid,  t_media *media, t_prog *prog);
void					r_sel_item(t_sdl *sdl, t_grid *grid, t_media *media, t_prog *prog);

/*
 * get_assets
 */

t_media					*get_assets(void);
void					free_media(t_media *media);

/*
 * rewrite
 */

unsigned short			rewrite_media(t_media *media);
unsigned short			rewrite_levels(t_media *media);

/*
 * modes
 */

unsigned short			init_modes(t_media *media, t_prog *prog);
void					free_modes(t_mode *modes);
void                    refresh_level_list(t_media *media, t_mode *mode);

/*
 * buttons
 */

void					render_button(t_button *button, t_sdl *sdl, SDL_Texture **t);
void					render_button_big(t_button *button, t_sdl *sdl, SDL_Texture **t);
void					render_btn(t_button *btn, t_sdl *sdl, int n_btn, int mode_id, SDL_Texture **t);
unsigned short			light_button(t_sdl *sdl, t_button *btn, int n_btn, t_prog *prog);
t_button				*init_btn(int n_btn);
void					free_btn(t_button *btn, int n);
unsigned short			distribute_btn_h(t_button *btn, int from, int to, t_rec box, int padding);
void					get_btn(int state, t_mode *mode);

/*
 * zoom
 */

void					zoom_grid(t_prog *prog, t_vec2d mouse, t_grid *grid);
void                    zoom_to_sector(t_sec *sector, t_vec2d *vecs, t_grid *grid, t_prog *prog);
void                    zoom_to_map(int n_vecs, t_vec2d *v, t_grid *grid);
void                    zoom_to_wall(t_vec2d v1, t_vec2d v2, t_grid *grid, t_prog *prog);

/*
 * move
 */

void					move_grid_drag(t_prog *prog, t_vec2d mouse, t_grid *grid);
unsigned short			move_grid_keys(t_prog *prog, t_grid *grid);
void					move_player(t_prog *prog, t_vec2d mouse, t_grid *grid, t_world *world);
void					move_vector(t_prog *prog, t_vec2d mouse, t_grid *grid, t_world *world);

/*
 * render
 */

void					draw_dot(int x, int y, int color, SDL_Renderer *rend);
void					draw_circle_fill(t_vec2d c, int radius, int color, SDL_Renderer *rend);
void				    get_rgb(unsigned char *r, unsigned char *g, unsigned char *b, int color);
void				    draw_line(t_line l, int color, SDL_Renderer *rend);
void				    draw_line_grid(t_line l, char c, signed char nodes[GRID_SIZE][GRID_SIZE]);


unsigned short			mouse_over(t_rec box, t_vec2d mouse);

void					render_frame(t_rec rec, int color, SDL_Renderer *rend);


/*
 * render_grid
 */

void					render_grid(t_world world, t_grid *grid, t_prog *prog, t_vec2d mouse);
void					render_grid_iso(t_world world, t_grid *grid, t_prog *prog);


void					draw_dot2(int x, int y, int color, int **screen);
void				    draw_line2(t_line l, int color, int **screen);
void					draw_circle_fill2(t_vec2d c, int radius, int color, int **screen);
void					draw_node(t_vec2d c, int r, int color, int **screen);
void				    draw_thick_line(t_line l, int color, int r, int **screen);

unsigned short			fill_polygon(t_vec2d *p, int n_p, int **screen, int color);

//int 					fill_sector_v(t_sec *sector, int *tmp, int n);





void 					free_prog(t_prog *prog);
t_prog					*get_prog(SDL_Renderer *rend);

void					render_screen(SDL_Renderer *rend, int **screen);

void					add_to_media(t_grid *grid, t_media *media);
unsigned short			add_world(t_world **worlds, short n_worlds, char *ext, char *path);

void					update_sector_status(t_sec *sec, t_wall *walls, t_vec2d *vecs, int n_sec);



t_vec2d					find_node(int p_x, int p_y, t_grid *grid);

t_vec2d_d				make_iso(int x, int y, int z);


void					render_box(t_rec box, SDL_Texture *t, SDL_Renderer *rend);


unsigned short			add_wall(t_wall **walls, short n_walls, int one, int two);


/*
 * edit_menu
 */

char 					*menu_lines(int id, int i);
void					render_edit_menu(t_sdl *sdl, t_texture *txtrs, t_world *w, int state, int n_txtrs, SDL_Texture **t);
t_rec                   layout_menu(char i, char n);


/*
 * values
 */

void					render_values(int state, int n, t_value *values, t_sdl *sdl, SDL_Texture **t, t_texture *txtrs, int n_txtrs);
void					free_values(t_value *values, int n);
t_value					*init_values(int n);


//door
//void                    delete_door(t_world *world, int id);
void                    add_door(t_world *world, int id, t_grid *grid);
void					delete_wall(int id, t_world *world);
//unsigned short			add_wall_in_secs(t_world *world, int to_add, int find);



/*
 * items
 */

void					draw_item(int type, t_rec box, SDL_Renderer *rend, SDL_Texture **t);
void					render_items(SDL_Renderer *rend, t_world *world, t_itemfull *itemfull, int n, t_grid *grid, SDL_Texture **t);
void					delete_item(t_sec *sector, int id);
void					move_item(t_prog *prog, t_vec2d mouse, t_grid *grid, t_sec *sector);
void					add_item(int default_id, t_vec2d mouse, t_grid *grid, t_sec *sector);
int 					find_default_item(int type, t_itemfull *itemfull, int n);

/*
 * fill_grid
 */

void					fill_grid(int n_vecs, t_vec2d *vecs, t_grid *grid);
void					fill_grid_items(t_sec *sector, t_grid *grid);
void                    fill_grid_walls(int n_walls, t_wall *walls, int n_vecs, t_vec2d *vecs, t_grid *grid);

/*
 * grid_main
 */

void					clean_grid(t_grid *grid);
t_grid                  *get_grid(void);
void					grid_refresh(t_grid *grid, t_media *media, int state, int sector);

/*
 * find_in_media
 */

short 					find_vector(t_vec2d *vecs, t_vec2d p, int n);
short 					find_wall(short one, short two, t_wall *walls, short n_walls);

/*
 * sector_logic
 */

unsigned short			dot_inside_sector(int x, int y, t_vec2d *p, int n);
int 					node_in_sector(t_vec2d grid_p, t_world *world);
int 					mouse_in_sector(t_vec2d p, t_world *world, t_grid *grid);
int 					sector_closed(int *tmp, int n);
unsigned short 			sec_is_convex(t_vec2d *vecs, int *v, int n);

//delete from media

void					delete_vector(int id, t_world *world);

/*
 * delete_world
 */

void					delete_world(t_media *media, int id);
void					free_world(t_world *world);
void					copy_world(t_world *new, t_world *old);

/*
 * delete_sector
 */

void					delete_sector(int id, t_world *world);
void					copy_sector(t_sec *new, t_sec *old);
void					free_sector(t_sec *sector);

/*
 * validate_media
 */

void					validate_media(t_media *media);

/*
 * file_fun
 */

int						my_rename(const char *oldname, const char *newname);
unsigned 				open_for_read(const char *path, int *fd);
unsigned 				open_for_write(const char *path, int *fd);
unsigned 				close_file(int fd);

int 					get_sec_v(t_sec *sector, t_wall *walls);

/*
 * math
 */

int						clamp(int n, int min, int max);
float					clamp_f(float n, float min, float max);
t_vec2d					scale(t_vec2d vec, int scale);
t_vec2d					mult(t_vec2d one, t_vec2d two);
t_vec2d					sub_one_from_two(t_vec2d one, t_vec2d two);
t_vec2d					add(t_vec2d one, t_vec2d two);
int                     get_max(int one, int two);
int                     get_min(int one, int two);

/*
 * useful
 */

unsigned short			within(int value, int min, int max);
int 					select_it(char set_get_unset, unsigned short id, int value);
int 					lit_it(char set_get_unset, unsigned short id, int value);
char 					*get_full_path(char *filename, char *ext, char *path);

/*
 * memory_fun
 */

void					*realloc_tab(void *old_tab, size_t new_size, size_t old_size);
void					free_tab(void **tab, int size);

void				    swap_ints(int *one, int *two);
unsigned short          vec_same(t_vec2d one, t_vec2d two);
void					pair_sort(int *a, int n);;

void					write_text(char *str, t_sdl *sdl, t_rec rec, int color, char h_center);

void					turn_btns_off(t_prog *prog);

unsigned short			add_sector(t_sec **sec, short n_sec);
unsigned short			add_vector(t_vec2d **vecs, short n_vecs, t_grid *grid, short i);

#endif
