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

typedef struct s_cub3d
{
	mlx_t		*mlx;
	mlx_image_t	*img;
	t_player	pos;
}	t_cub3d;

#endif