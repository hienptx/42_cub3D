/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   typedef.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hipham <hipham@student.42heilbronn.de>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/05 23:31:29 by hipham            #+#    #+#             */
/*   Updated: 2025/02/08 22:11:30 by hipham           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

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

typedef struct s_ray
{
	int		x1;
	int		y1;
	int		x2;
	int		y2;
	int		dx;
	int		dy;
	int		sx;
	int		sy;
	int		err;
	int		e2;
	float	end_x;
	float	end_y;
}					t_ray;

typedef struct s_intersection
{
	float			rx;
	float			ry;
	float			xo;
	float			yo;
	float			distance;
	int				dof;
	unsigned int	mx;
	unsigned int	my;
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
	float			dirx;
	float			diry;
	float			vx;
	float			vy;
	float			rx;
	float			ry;
	float			end_x;
	float			end_y;
}					t_ray_data;

typedef struct s_wall_data
{
	double			height;
	double			line_top;
	double			line_bottom;
	double			step;
	double			tex_pos;
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
	unsigned int	px;
	unsigned int	py;
}					t_player;

typedef struct s_counter
{
	size_t			floor;
	size_t			ceiling;
	size_t			west;
	size_t			east;
	size_t			north;
	size_t			south;
}					t_counter;

typedef struct s_user_map
{
	size_t			map_width;
	size_t			map_height;
	float			pw; //(pixel_width = scale_factor_x)
	float			ph; //(pixel_height = scale_factor_y)
	char			*no_texture;
	char			*so_texture;
	char			*we_texture;
	char			*ea_texture;
	int				*floor;
	int				*ceiling;
	char			**map_data;
	int				player_count;
	int				texture_count;
	int				color_count;
	bool			valid_status;
	t_player		pos;
	int				scale_factor_x;
	int				scale_factor_y;
	int				color;
	t_counter		counter;
}					t_user_map;

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
	float			cell_size;
	float			offset;
	int				key_pressed;
}					t_cub3d;

#endif