/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   rendering_utils_bonus.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hipham <hipham@student.42heilbronn.de>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/04 19:09:10 by hipham            #+#    #+#             */
/*   Updated: 2025/02/07 19:55:00 by hipham           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/cub3D_bonus.h"

bool	check_for_wall(char *line)
{
	while (*line != '\0')
	{
		if (*line != '1' && *line != ' ' && *line != '\t')
			return (0);
		line++;
	}
	return (1);
}

int	get_rgba(int r, int g, int b, int a)
{
	return (r << 24 | g << 16 | b << 8 | a);
}

uint32_t	get_texture_x(float pos, u_int32_t tex_width, float cell_size)
{
	float		relative_pos;
	uint32_t	tex_x;

	relative_pos = fmod(pos, cell_size);
	tex_x = (uint32_t)((relative_pos / cell_size) * tex_width);
	tex_x %= tex_width;
	return (tex_x);
}

void	load_png_texture(t_cub3d *data)
{
	data->texture[0] = mlx_load_png(data->map.we_texture);
	data->texture[1] = mlx_load_png(data->map.ea_texture);
	data->texture[2] = mlx_load_png(data->map.no_texture);
	data->texture[3] = mlx_load_png(data->map.so_texture);
	data->wp.texture[0] = mlx_load_png("./images/g.png");
	data->wp.texture[1] = mlx_load_png("./images/aiming.png");
	data->wp.texture[2] = mlx_load_png("./images/flash.png");
}

void	init_ray_values(t_ray *val, t_ray_data *ray, float angle)
{
	val->end_x = ray->px + ray->distance * cos(angle);
	val->end_y = ray->py + ray->distance * -sin(angle);
	val->x1 = (int)ray->px;
	val->y1 = (int)ray->py;
	val->x2 = (int)val->end_x;
	val->y2 = (int)val->end_y;
	val->dx = abs(val->x2 - val->x1);
	val->dy = abs(val->y2 - val->y1);
	if (val->x1 < val->x2)
		val->sx = 1;
	else
		val->sx = -1;
	if (val->y1 < val->y2)
		val->sy = 1;
	else
		val->sy = -1;
	if (val->dx > val->dy)
		val->err = val->dx / 2;
	else
		val->err = -val->dx / 2;
}
