/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   rendering_utils.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hipham <hipham@student.42heilbronn.de>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/04 19:09:10 by hipham            #+#    #+#             */
/*   Updated: 2025/02/04 19:10:44 by hipham           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/cub3D.h"

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
