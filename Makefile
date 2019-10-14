# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: mshpakov <marvin@42.fr>                    +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2019/07/25 15:29:39 by mshpakov          #+#    #+#              #
#    Updated: 2019/07/25 15:29:41 by mshpakov         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

# Compilation
CC =		gcc
CFLAGS =	-Wall -Wextra -Werror -g
OTHER_FLAGS =
#OTHER_FLAGS = -fsanitize=address

# Default rule
DEFRULE =	all

# Binary
NAME =		builder

# Directories
SRCDIR =	./src
OBJDIR =	./obj
UNAME_S := $(shell uname -s)
FT_INC =	-I libft/

ifeq ($(UNAME_S),Darwin)
#	ID_UN := $(shell id -un)
#	SDL2_INC =	-I /Users/$(ID_UN)/.brew/Cellar/sdl2/2.0.5/include/SDL2\
#				-I /Users/$(ID_UN)/.brew/Cellar/sdl2_ttf/2.0.14/include/SDL2\
#				-I /Users/$(ID_UN)/.brew/Cellar/sdl2_image/2.0.1_2/include/SDL2\
#				-I -F -framework Cocoa
	SDL2_INC =	-I $(CURDIR)/frameworks/SDL2.framework/Headers/ \
                -I $(CURDIR)/frameworks/SDL2_mixer.framework/Headers \
                -I $(CURDIR)/frameworks/SDL2_ttf.framework/Headers/ \
                -I $(CURDIR)/frameworks/SDL2_image.framework/Headers/
	SDL2_FLAGS =
	FLAG_F = -F frameworks
endif

ifeq ($(UNAME_S),Linux)
	SDL2_INC =
	SDL2_FLAGS =		`sdl2-config --cflags`
	FLAG_F =
endif

# Sources
SRC =   sdl_fun.c \
        main_menu.c \
        main.c \
        levels.c \
        get_assets.c \
        get_next_line.c \
        render.c \
        modes.c \
        math.c \
        rewrite.c \
        render_grid.c \
        fill_polygon.c \
        editor.c \
        prog.c \
        add_to_media.c \
        zoom.c \
        textures.c \
        useful.c \
        move.c \
        edit_menu.c \
        values.c \
        init_editor_buttons.c \
        edit_walls.c \
        items.c \
        sel_item.c \
        fill_grid.c \
        grid_main.c \
        find_in_media.c \
        sector_logic.c \
        delete_world.c \
        delete_sector.c \
        validate_media.c \
        file_fun.c \
        memory_fun.c \
        line_math.c \
        vec_ops.c \
        render_btn.c \
        manage_btn.c \
        delete_vector.c \
        add_door.c \
        sort.c \
        get_input.c \
        drawing.c \
        states.c \
        get_sec_v.c \
        get_sec_items.c \
        select_lit.c \
        char_ops.c \
        add_sector.c \
        texture_edit.c \
        edit_world.c \
        editor_render.c \
        editor_update.c \
        render_items.c \
        move_item.c \
        render_elements.c \
        move_player.c \
        move_vector.c \
        update_sec.c \
        render_grid_iso.c \
        render_grid_elems.c \
        delete_useless.c \
        replace.c \
        validate_clockwise.c \
        write_section.c \
        write_level.c \
        identify.c \
        read_assets_help.c \
        get_map.c \
        section.c \
        free_media.c \
        update_media_sect.c \
        read_levels.c \
        get_map_elems.c \
        init_world.c \
        read_line.c \
        drawing_help.c \
        edit_walls_help.c \
        grid_ops.c \
        btn_press.c \
        distribute_btns.c \
        levels_btn.c \
        mode_btn.c \
        math_ops.c \
        btn_main.c \
        clip_line.c \
        check_for_light.c \
        draw_line.c \
        settings.c \
        add_door_help.c \
        texture_name.c \
        btn_press_settings.c \
        get_boxes.c \
        break_wall.c \
        map_useful.c \
        settings_help.c

OBJ =	$(SRC:.c=.o)

# Prefixes
ifeq ($(UNAME_S),Darwin)
#	SDL2_LINK =	-L/Users/$(ID_UN)/.brew/Cellar/sdl2/2.0.5/lib -lSDL2\
#				-L/Users/$(ID_UN)/.brew/Cellar/sdl2_ttf/2.0.14/lib -lSDL2_ttf\
#				-L/Users/$(ID_UN)/.brew/Cellar/sdl2_image/2.0.1_2/lib -lSDL2_image
	SDL2_LINK =	-rpath frameworks -framework SDL2 -framework SDL2_mixer -framework SDL2_image -framework SDL2_ttf
endif
ifeq ($(UNAME_S),Linux)
	SDL2_LINK =	`sdl2-config --libs` -lSDL2_ttf -lSDL2_image
endif

MATH_LINK =		-lm
FT_LINK =		-L libft/ -lft

# Paths foreach
OBJP =		$(addprefix $(OBJDIR)/, $(SRC:.c=.o))
OBJS_DIRS = $(sort $(dir $(OBJP)))

# **************************************************************************** #
# SPECIAL CHARS

LOG_CLEAR		= \033[2K
LOG_UP			= \033[A
LOG_NOCOLOR		= \033[0m
LOG_BOLD		= \033[1m
LOG_UNDERLINE	= \033[4m
LOG_BLINKING	= \033[5m
LOG_BLACK		= \033[1;30m
LOG_RED			= \033[1;31m
LOG_GREEN		= \033[1;32m
LOG_YELLOW		= \033[1;33m
LOG_BLUE		= \033[1;34m
LOG_VIOLET		= \033[1;35m
LOG_CYAN		= \033[1;36m
LOG_WHITE		= \033[1;37m

# **************************************************************************** #
# RULES
.SILENT:

.PHONY: fclean clean re ft default

# Main rules
default:
	@echo -e "$(LOG_BOLD)Default execution: rule $(DEFRULE)$(LOG_NOCOLOR)"
	@make $(DEFRULE)
	@echo -e "$(LOG_BOLD)Execution finished     $(LOG_NOCOLOR)ヽ(ヅ)ノ"

all: ft $(OBJDIR) $(NAME)

# Compilation rules
ft:
	@make all -C libft/

$(OBJDIR)/%.o: $(SRCDIR)/%.c
	@echo -e "--$(LOG_CLEAR)$(LOG_VIOLET)$(NAME)$(LOG_NOCOLOR)........................... $(LOG_YELLOW)$<$(LOG_NOCOLOR)$(LOG_UP)"
	@$(CC) $(CFLAGS) $(OTHER_FLAGS) -c -o $@ $^ $(FLAG_F) $(FT_INC) $(SDL2_INC) $(SDL2_FLAGS)

$(OBJDIR):
	@echo -e "$(LOG_CLEAR)$(LOG_BLUE)build $(LOG_YELLOW)$(NAME)$(LOG_NOCOLOR)"
	@mkdir -p $(OBJDIR)
	@mkdir -p $(OBJS_DIRS)

$(NAME): $(OBJP)
	@echo -e "--$(LOG_CLEAR)$(LOG_VIOLET)$(NAME)$(LOG_NOCOLOR)......................... $(LOG_YELLOW)assembling$(LOG_NOCOLOR)$(LOG_UP)"
	@$(CC) $(CFLAGS) $(OTHER_FLAGS) -o $@ $^ $(FLAG_F) $(SDL2_LINK) $(FT_LINK) $(MATH_LINK) $(SDL2_INC)
	@echo -e "--$(LOG_CLEAR)$(LOG_VIOLET)$(NAME)$(LOG_NOCOLOR) compiled.................. $(LOG_GREEN)✓$(LOG_NOCOLOR)"

clean:
	@echo -e "$(LOG_CLEAR)$(LOG_BLUE)clean $(LOG_YELLOW)$(NAME)$(LOG_NOCOLOR)"
	@echo -e "--$(LOG_CLEAR)$(LOG_YELLOW)Objects$(LOG_NOCOLOR) deletion............. $(LOG_RED)×$(LOG_NOCOLOR)"
	@rm -rf $(OBJDIR)
	@echo -e "$(LOG_CLEAR)$(LOG_BLUE)clean $(LOG_YELLOW)libft$(LOG_NOCOLOR)"
	@make clean -C libft/


fclean:
	@echo -e "$(LOG_CLEAR)$(LOG_BLUE)fclean $(LOG_YELLOW)libft$(LOG_NOCOLOR)"
	@make fclean -C libft/
	@echo -e "$(LOG_CLEAR)$(LOG_BLUE)fclean $(LOG_YELLOW)$(NAME)$(LOG_NOCOLOR)"
	@echo -e "--$(LOG_CLEAR)$(LOG_YELLOW)Objects$(LOG_NOCOLOR) deletion............. $(LOG_RED)×$(LOG_NOCOLOR)"
	@rm -rf $(OBJDIR)
	@echo -e "--$(LOG_CLEAR)$(LOG_YELLOW)Binary$(LOG_NOCOLOR) deletion.............. $(LOG_RED)×$(LOG_NOCOLOR)"
	@rm -f $(NAME)

re: fclean all
