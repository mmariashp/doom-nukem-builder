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
SRCDIR =	./
OBJDIR =	obj
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
SRC =	main.c \
        level_edit.c \
        editor.c \
        main_menu.c \
        summary.c \
        buttons.c \
        rewrite.c \
        fill_polygon.c \
        get_assets.c \
        get_next_line.c \
        math.c \
        modes.c \
        prog.c \
        zoom.c \
        render.c \
        sdl_fun.c \
        render_grid.c \
        textures.c \
        add_to_media.c \
        walls.c \
        useful.c

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
	@$(CC) $(CFLAGS) $(OTHER_FLAGS) -c -o $@ $^ $(FLAG_F) $(SDL2_INC) $(SDL2_FLAGS)

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

# SDL2 brew

#sdl2:
#	brew install sdl2
#	brew link sdl2
#	brew install sdl2_ttf
#	brew link sdl2_ttf
#	brew install sdl2_image
#	brew link sdl2_image