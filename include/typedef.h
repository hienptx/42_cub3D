#ifndef TYPEDEF_H
# define TYPEDEF_H
# include "../MLX/include/MLX42/MLX42.h"
# include <stddef.h>
# include <time.h>

typedef enum e_ray_dir
{
	DIR_HORIZONTAL,
	DIR_VERTICAL
}					t_ray_dir;

typedef struct s_intersection
{
	float			rx;
	float			ry;
	float			xo;
	float			yo;
	float			distance;
	int				dof;
	t_ray_dir		dir;
}					t_intersection;

typedef struct s_ray_data
{
	float			angle;
	float			tan_angle;
	float			angle_step;
	float			px;
	float			py;
	float			hit_x;
	float			hit_y;
	float			distance;
	float			color;
	float			dirX;
	float			dirY;
	float			vx;
	float			vy;
	float			rx;
	float			ry;
}					t_ray_data;

typedef struct s_wall_data
{
	unsigned int	height;
	unsigned int	line_top;
	unsigned int	line_bottom;
	double			step;
	uint32_t		tex_x;
	mlx_texture_t	*texture;
}					t_wall_data;

typedef struct s_player
{
	float			x;
	float			y;
	float			dx;
	float			dy;
	float			angle;
}					t_player;

typedef struct s_user_map
{
	size_t			map_width;
	size_t			map_height;
	float pw; //(pixel_width = scale_factor_x)
	float ph; //(pixel_height = scale_factor_y)
	char			*NO_texture;
	char			*SO_texture;
	char			*WE_texture;
	char			*EA_texture;
	int				*floor;
	int				*ceiling;
	char			**map_data;
	int				player_count;
	int				texture_count;
	int				color_count;
	bool			valid_status;
	t_player		pos;
}					t_user_map;

typedef struct s_weapon
{
	mlx_texture_t	*texture[3];
	bool			is_shooting;
	bool			is_flash_active;
	float			flash_x;
	float			flash_y;
	float			gun_x;
	float			gun_y;
	float			aim_x;
	float			aim_y;
	float			duration;
	time_t			timer;
}					t_weapon;

typedef struct s_cub3d
{
	uint32_t		iwidth;
	uint32_t		iheight;
	mlx_t			*mlx;
	mlx_image_t		*img;
	mlx_image_t		*img2;
	mlx_texture_t	*texture[4];
	t_player		pos;
	t_user_map		map;
	t_ray_data		ray;
	t_weapon		wp;
	float			cell_size;
	int				key_pressed;
}					t_cub3d;

#endif