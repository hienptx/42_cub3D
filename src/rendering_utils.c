/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   rendering_utils.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hipham <hipham@student.42heilbronn.de>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/04 19:09:10 by hipham            #+#    #+#             */
/*   Updated: 2025/02/05 02:37:10 by hipham           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/cub3D.h"

bool	check_for_wall(char *line)
{
	while (*line != '\0')
	{
		if (*line != '1' && *line != ' ')
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
	data->texture[0] = mlx_load_png(data->map.WE_texture);
	data->texture[1] = mlx_load_png(data->map.EA_texture);
	data->texture[2] = mlx_load_png(data->map.NO_texture);
	data->texture[3] = mlx_load_png(data->map.SO_texture);
	data->wp.texture[0] = mlx_load_png("./images/g.png");
	data->wp.texture[1] = mlx_load_png("./images/aiming.png");
	data->wp.texture[2] = mlx_load_png("./images/flash.png");
}
