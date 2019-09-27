//
// Created by moosh on 15.08.19.
//

#ifndef MEDIA_H
#define MEDIA_H

# include "general.h"
# include "builder.h"

# define LEVELS					"Levels"
# define TEXTURES				"Textures"
# define SOUNDS					"Sounds"
# define FONTS					"Fonts"
# define TOTAL_SECTIONS			4

# define R_VECTORS				1
# define R_WALLS				2
# define R_SECS				3
# define R_PLAYER				4
# define R_TEXTURES				5

# define MIN_VERTEX_COORD		0
# define MAX_VERTEX_COORD		200

# define MIN_VERTEX_ID			0
# define MAX_VERTEX_ID			200

# define MIN_N_WALLS			3
# define MAX_N_WALLS			100

# define MIN_N_SECS			1
# define MAX_N_SECS			50

# define MIN_N_TXTRS			1
# define MAX_N_TXTRS			50

# define MIN_HEIGHT				-100
# define MAX_HEIGHT				100

# define MIN_SEC_WALLS		3
# define MAX_SEC_WALLS		20

//wall types
# define WALL_EMPTY					0
# define WALL_FILLED				1
# define WALL_DOOR					2

/*
** program
*/

# define BINARY_NAME			media
# define ASSET_FILE             "./asset_paths.txt"

typedef struct                  s_wall
{
	int 						v1;
	int 						v2;
	int 						type;
	short						txtr;
}                               t_wall;

typedef struct                  s_sector
{
	int 						*s_walls;
	int						    fl;
	int						    ceiling;
	short 						fl_txtr;
	short 						ceil_txtr;
	unsigned short				n_walls;
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
}                               t_texture;

typedef struct					s_world
{
	char 						*filename;
	char 						*full_path;
	int							*textures;
	t_sec					*sec;
	t_wall						*walls;
	t_vec2d						*vecs;
	t_vec2d						p_start;
	t_vec2d						p_end;
	short unsigned				n_sec;
	short unsigned				n_vecs;
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
}								t_section;

typedef struct 					s_media
{
	t_texture					*txtrs;
	char						**fonts;
	char						**sounds;
	t_world						*worlds;
	short unsigned				n_worlds;
	short unsigned				n_txtrs;
	short unsigned				n_fonts;
	short unsigned				n_sounds;
}								t_media;

t_media							*get_assets(void);
void							free_media(t_media *media);

#endif
