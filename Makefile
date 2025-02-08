NAME	:= cub3D
CFLAGS	:= -g -Ofast -Wextra -Wall -Werror -I./include -Wunreachable-code
LEAKS = -L../../LeakSanitizer -llsan -lc++ -Wno-gnu-include-next -I ../LeakSanitize
LIBMLX	:= ./MLX
LIBFT_PATH	:= ./libft
SRC_DIR := ./src
MAND_DIR := $(SRC_DIR)/mandatory
BONUS_DIR := $(SRC_DIR)/bonus
LIBFT := $(LIBFT_PATH)/libft.a
HEADERS	:= -I ./include -I $(LIBMLX)/include
LIBS	:= $(LIBMLX)/build/libmlx42.a -ldl -lglfw -pthread -lm
SRCS	:=	cub3D_utils.c \
			initialization_and_free.c \
			main.c \
			parsing_utils.c \
			parsing_more_utils.c \
			parsing.c \
			validation.c \
			movements_and_rotation.c \
			ray_direction.c \
			get_ray_distance.c \
			wall_rendering.c \
			rendering_utils.c \
			keys.c \
			rays.c
MAND_SRCS   := $(addprefix $(MAND_DIR)/, $(SRCS))
SRCS += weapon.c minimap.c
MAND_OBJS   := ${MAND_SRCS:.c=.o}
BONUS_SRCS := $(addsuffix _bonus.c, $(addprefix $(BONUS_DIR)/, $(basename $(SRCS))))
BONUS_OBJS  := ${BONUS_SRCS:.c=.o}

all: libmlx $(NAME)

libmlx:
	@echo "Creating MLX..."
	@cmake $(LIBMLX) -B $(LIBMLX)/build && make -C $(LIBMLX)/build -j4

%.o: %.c
	@$(CC) $(CFLAGS) -o $@ -c $< $(HEADERS) && printf "Compiling: $(notdir $<)\n"

$(NAME): $(MAND_OBJS) $(LIBFT)
	@$(CC) $(MAND_OBJS) $(LIBS) $(LIBFT) $(HEADERS) -o $(NAME) $(LEAKS)

$(LIBFT):
	@make -C $(LIBFT_PATH)

bonus: libmlx $(BONUS_OBJS) $(LIBFT)
	@$(CC) $(BONUS_OBJS) $(LIBS) $(LIBFT) $(HEADERS) -o $(NAME)

clean:
	@echo "Cleaning up..."
	@rm -rf objs/*.o $(MAND_DIR)/*.o $(BONUS_DIR)/*.o
	@rm -rf $(LIBMLX)/build
	@make -C $(LIBFT_PATH) clean

fclean: clean
	@rm -rf $(NAME)
	@rm -rf $(LIBFT) fclean

re: fclean all

.PHONY: all clean fclean re libmlx bonus
