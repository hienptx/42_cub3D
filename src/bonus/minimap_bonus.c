/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minimap.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hipham <hipham@student.42heilbronn.de>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/05 01:41:48 by hipham            #+#    #+#             */
/*   Updated: 2025/02/05 23:20:55 by hipham           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/cub3D.h"

void	put_pixel_player(t_cub3d *data, u_int32_t color)
{
	const int		x_margin[7] = {-3, -2, -1, 0, 1, 2, 3};
	const int		y_margin[7] = {-3, -2, -1, 0, 1, 2, 3};
	size_t			i;
	size_t			j;

	i = 0;
	while (i < sizeof(x_margin) / sizeof(int))
	{
		j = 0;
		while (j < sizeof(y_margin) / sizeof(int))
		{
			data->pos.px = data->pos.x + x_margin[i];
			data->pos.py = data->pos.y + y_margin[j];
			if (data->pos.px < data->img->width
				&& data->pos.py < data->img->height)
				mlx_put_pixel(data->img, data->pos.px, data->pos.py, color);
			j++;
		}
		i++;
	}
}

void	draw_cell(t_cub3d *data, int x_start, int y_start, int color)
{
	int	y;
	int	x;

	y = y_start;
	while (y < y_start + data->map.scale_factor_y)
	{
		x = x_start;
		while (x < x_start + data->map.scale_factor_x)
		{
			mlx_put_pixel(data->img, x, y, color);
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
	int	start_x;
	int	start_y;
	int	i;
	int	j;

	data->map.scale_factor_x = data->img->width / data->map.map_width;
	data->map.scale_factor_y = data->img->height / data->map.map_height;
	i = 0;
	while (map[i] != NULL)
	{
		j = 0;
		while (map[i][j] != '\0')
		{
			data->map.color = get_minimap_color(map[i][j]);
			start_x = j * data->map.scale_factor_y;
			start_y = i * data->map.scale_factor_x;
			draw_cell(data, start_x, start_y, data->map.color);
			j++;
		}
		i++;
	}
}

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
