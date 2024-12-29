#ifndef TYPEDEF_H
# define TYPEDEF_H
# include "../MLX/include/MLX42/MLX42.h"
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
	char	**map_data;
	int	map_width;
	int	map_height;
	char *NO_texture;
	char *SO_texture;
	char *WE_texture;
	char *EA_texture;
	int *floor;
	int *ceiling;
}	t_user_map;

typedef struct s_cub3d
{
	mlx_t		*mlx;
	mlx_image_t	*img;
	mlx_image_t	*img2;
	t_player	pos;
	t_user_map	map;
	int			key_pressed;
}	t_cub3d;

#endif