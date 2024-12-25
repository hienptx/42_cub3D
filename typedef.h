#ifndef TYPEDEF_H
# define TYPEDEF_H
# include "MLX/include/MLX42/MLX42.h"

typedef struct s_player
{
	unsigned int	x;
	unsigned int	y;
	float			dx;
	float			dy;
	float			angle;
}	t_player;

typedef struct s_user_map
{
	int	*map_data;
	int	map_width;
}	t_user_map;

typedef struct s_cub3d
{
	mlx_t		*mlx;
	mlx_image_t	*img;
	t_player	pos;
	t_user_map	map;
	int			key_pressed;
}	t_cub3d;

#endif