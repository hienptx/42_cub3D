NAME	:= cub3D
CFLAGS	:= -g -Ofast -Wextra -Wall -Werror -I./includes -Wunreachable-code -Ofast
LEAKS = -L../../LeakSanitizer -llsan -lc++ -Wno-gnu-include-next -I ../LeakSanitize
LIBMLX	:= ./MLX
LIBFT_PATH	:= ./libft
SRC_DIR := ./src

LIBFT := $(LIBFT_PATH)/libft.a
HEADERS	:= -I ./include -I $(LIBMLX)/include
LIBS	:= $(LIBMLX)/build/libmlx42.a -ldl -lglfw -pthread -lm
SRCS	:= $(addprefix $(SRC_DIR)/, \
			cub3D_utils.c \
			initialization_and_free.c \
			main.c \
			parsing_utils.c \
			parsing.c \
			validation.c \
			movements_and_rotation.c \
			ray_direction.c \
			get_ray_distance.c \
			wall_rendering.c \
			rendering_utils.c \
			keys.c \
			rays.c)
OBJS	:= ${SRCS:.c=.o}

all: libmlx $(NAME)

libmlx:
	@echo "Creating MLX..."
	@cmake $(LIBMLX) -B $(LIBMLX)/build && make -C $(LIBMLX)/build -j4

%.o: %.c
	@$(CC) $(CFLAGS) -o $@ -c $< $(HEADERS) && printf "Compiling: $(notdir $<)\n"

$(NAME): $(OBJS) $(LIBFT)
	@$(CC) $(OBJS) $(LIBS) $(LIBFT) $(HEADERS) -o $(NAME)
$(LIBFT):
	@make -C $(LIBFT_PATH)

clean:
	@echo "Cleaning up..."
	@rm -rf $(OBJS)
	@rm -rf $(LIBMLX)/build
	@make -C $(LIBFT_PATH) clean

fclean: clean
	@rm -rf $(NAME)
	@rm -rf $(LIBFT) fclean

re: clean all

.PHONY: all, clean, fclean, re, libmlx