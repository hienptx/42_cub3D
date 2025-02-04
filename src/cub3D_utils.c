/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cub3D_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hipham <hipham@student.42heilbronn.de>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/06 23:06:34 by hipham            #+#    #+#             */
/*   Updated: 2025/02/04 18:23:41 by hipham           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/cub3D.h"

int	error_sms(char *s)
{
	printf("%s", s);
	exit(EXIT_FAILURE);
}

void	ft_malloc(void **data, size_t size)
{
	*data = malloc(size);
	if (*data == NULL)
	{
		printf("Malloc failed to allocate memory\n");
		exit(EXIT_FAILURE);
	}
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

// Exit the program as failure.
void	ft_error(void)
{
	fprintf(stderr, "%s", mlx_strerror(mlx_errno));
	exit(EXIT_FAILURE);
}

uint32_t	get_pixel_color(mlx_texture_t *texture, uint32_t tex_x,
		uint32_t tex_y)
{
	uint8_t		*pixel;
	uint32_t	a;

	pixel = &texture->pixels[(texture->width * tex_y + tex_x) * 4];
	a = pixel[3];
	if (a == 0)
		return (0x00000000);
	return ((pixel[0] << 24) | (pixel[1] << 16) | (pixel[2] << 8) | pixel[3]);
}
