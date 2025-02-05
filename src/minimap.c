/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minimap.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hipham <hipham@student.42heilbronn.de>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/05 01:41:48 by hipham            #+#    #+#             */
/*   Updated: 2025/02/05 22:07:49 by hipham           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/cub3D.h"

void	put_pixel_player(t_cub3d *data, u_int32_t color)
{
	const int		x_margin[7] = {-3, -2, -1, 0, 1, 2, 3};
	const int		y_margin[7] = {-3, -2, -1, 0, 1, 2, 3};
	unsigned int	px;
	unsigned int	py;
	size_t			i;
	size_t			j;

	i = 0;
	while (i < sizeof(x_margin) / sizeof(int))
	{
		j = 0;
		while (j < sizeof(y_margin) / sizeof(int))
		{
			px = data->pos.x + x_margin[i];
			py = data->pos.y + y_margin[j];
			if (px < data->img->width && py < data->img->height)
				mlx_put_pixel(data->img, px, py, color);
			j++;
		}
		i++;
	}
}

void	draw_cell(mlx_image_t *img, int x_start, int y_start,
		int scale_factor_x, int scale_factor_y, int color)
{
	int	y;
	int	x;

	y = y_start;
	while (y < y_start + scale_factor_y)
	{
		x = x_start;
		while (x < x_start + scale_factor_x)
		{
			mlx_put_pixel(img, x, y, color);
			x++;
		}
		y++;
	}
}

int	get_minimap_color(char cell)
{
	if (cell == '1')
		return (0xFF00003F);
	else if (cell == '0' || ft_strchr("NSEW", cell))
		return (0xFFFFFF3F);
	return (0);
}

void	render_map(char **map, t_cub3d *data)
{
	int	scale_factor_x;
	int	scale_factor_y;
	int	color;
	int	i;
	int	j;

	scale_factor_x = data->img->width / data->map.map_width;
	scale_factor_y = data->img->height / data->map.map_height;
	i = 0;
	while (map[i] != NULL)
	{
		j = 0;
		while (map[i][j] != '\0')
		{
			color = get_minimap_color(map[i][j]);
			draw_cell(data->img, j * scale_factor_y, i * scale_factor_x,
				scale_factor_x, scale_factor_y, color);
			j++;
		}
		i++;
	}
}

// void calculate_end_point(float start_x, float start_y, float angle,
// 		float length, int *end_x, int *end_y)
// {
// 	*end_x = (int)(start_x + length * cos(angle));
// 	*end_y = (int)(start_y + length * -sin(angle));
// }

void	draw_ray(t_cub3d *data, float angle, int color)
{
	t_ray		val;
	t_ray_data	ray;

	ray = data->ray;
	init_ray_values(&val, &ray, angle);
	while (1)
	{
		mlx_put_pixel(data->img, val.x1, val.y1, color);
		if (val.x1 == val.x2 && val.y1 == val.y2)
			break ;
		val.e2 = val.err;
		if (val.e2 > -val.dx)
		{
			val.err -= val.dy;
			val.x1 += val.sx;
		}
		if (val.e2 < val.dy)
		{
			val.err += val.dx;
			val.y1 += val.sy;
		}
	}
}
