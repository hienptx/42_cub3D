#ifndef TYPEDEF_H
# define TYPEDEF_H
# include "MLX/include/MLX42/MLX42.h"

typedef struct s_position
{
	unsigned int	x;
	unsigned int	y;
	float			dx;
	float			dy;
}	t_position;

typedef struct s_cub3d
{
	mlx_t		*mlx;
	mlx_image_t	*img;
	t_position	pos;
}	t_cub3d;

#endif