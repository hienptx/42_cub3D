/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   wall_rendering.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hipham <hipham@student.42heilbronn.de>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/02 01:53:22 by hipham            #+#    #+#             */
/*   Updated: 2025/02/08 19:53:09 by hipham           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/cub3D.h"

static t_wall_data	calculate_wall_dimensions(t_cub3d *data, double distance)
{
	t_wall_data	wall_data;

	wall_data.height = (HEIGHT * data->cell_size / distance);
	wall_data.line_top = (HEIGHT - wall_data.height) / 2;
	if (wall_data.line_top < 0)
		wall_data.line_top = 0;
	wall_data.line_bottom = wall_data.line_top + wall_data.height;
	if (wall_data.line_bottom >= HEIGHT)
		wall_data.line_bottom = HEIGHT - 1;
	return (wall_data);
}

void	floor_drawing(t_cub3d *data, t_wall_data wall, int x)
{
	uint32_t	floor_color;

	floor_color = get_rgba(data->map.floor[0], data->map.floor[1],
			data->map.floor[2], 155);
	while (wall.line_bottom < HEIGHT)
	{
		mlx_put_pixel(data->img2, x, wall.line_bottom, floor_color);
		wall.line_bottom++;
	}
}

void	ceiling_drawing(t_cub3d *data, t_wall_data wall, int x)
{
	uint32_t		ceiling_color;
	unsigned int	i;

	i = 0;
	ceiling_color = get_rgba(data->map.ceiling[0], data->map.ceiling[1],
			data->map.ceiling[2], 155);
	while (i < wall.line_top)
	{
		mlx_put_pixel(data->img2, x, i, ceiling_color);
		i++;
	}
}

void	draw_wall_slice(t_cub3d *data, t_wall_data wall, uint32_t tex_x,
		int x)
{
	unsigned int	y;
	uint32_t		tex_y;
	uint32_t		pixel_color;

	y = wall.line_top;
	while (y <= wall.line_bottom)
	{
		tex_y = (uint32_t)wall.tex_pos % wall.texture->height;
		pixel_color = get_pixel_color(wall.texture, tex_x, tex_y);
		mlx_put_pixel(data->img2, x, y, pixel_color);
		wall.tex_pos += wall.step;
		y++;
	}
}

void	draw_single_slice(t_cub3d *data, int x, double distance_to_wall,
		t_ray_data *ray)
{
	t_wall_data		wall;
	uint32_t		tex_x;

	wall = calculate_wall_dimensions(data, distance_to_wall);
	wall.texture = get_wall_texture(data, ray);
	wall.step = (double)wall.texture->height / wall.height;
	if (wall.height > HEIGHT)
		wall.tex_pos = ((wall.height - HEIGHT) / 2) * wall.step;
	else
		wall.tex_pos = 0;
	if (ray->color == 0)
		tex_x = get_texture_x(ray->hit_x, wall.texture->width, data->cell_size);
	else
		tex_x = get_texture_x(ray->hit_y, wall.texture->width, data->cell_size);
	draw_wall_slice(data, wall, tex_x, x);
	floor_drawing(data, wall, x);
	ceiling_drawing(data, wall, x);
}
