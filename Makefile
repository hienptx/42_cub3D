NAME = cub3D

CC = cc
CFLAGS = -Wall -Wextra -Werror -I./includes

SRCS_PATH = ./srcs
OBJS_PATH = ./objs

LIBFT = $(LIBFT_PATH)/libft.a
MLX_LIB := $(MLX_PATH)/build/libmlx42.a
MLX = -L$(MLX_PATH)/build -lmlx42 -ldl -lglfw -pthread -lm

SRCS = *.c
OBJS = $(SRCS:.c=.o)

.PHONY all clean fclean re


