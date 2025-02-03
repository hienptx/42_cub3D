/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cub3D_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hipham <hipham@student.42heilbronn.de>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/06 23:06:34 by hipham            #+#    #+#             */
/*   Updated: 2025/02/03 16:50:56 by hipham           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/cub3D.h"

int error_sms(char *s)
{
	printf("Error: %s\n", s);
	exit(EXIT_FAILURE);
}

void ft_malloc(void **data, size_t size)
{
	*data = malloc(size);
	if (*data == NULL)
	{
		printf("Malloc failed to allocate memory\n");
		exit(EXIT_FAILURE);
	}
}

void load_png_texture(t_cub3d *data)
{	
	data->texture[0] = mlx_load_png(data->map.WE_texture);
	data->texture[1] = mlx_load_png(data->map.EA_texture);
	data->texture[2] = mlx_load_png(data->map.NO_texture);
	data->texture[3] = mlx_load_png(data->map.SO_texture);
	data->wp.texture[0] = mlx_load_png("./images/g.png");
	data->wp.texture[1] = mlx_load_png("./images/aiming.png");
	data->wp.texture[2] = mlx_load_png("./images/flash.png");
	// data->wp.gun_x = (data->img2->width - data->wp.texture[0]->width) / 2;
	// data->wp.gun_y = (data->img2->height * 4 / 3) - data->wp.texture[0]->height;
}

// Exit the program as failure.
void ft_error(void)
{
	fprintf(stderr, "%s", mlx_strerror(mlx_errno));
	exit(EXIT_FAILURE);
}
