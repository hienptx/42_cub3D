/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cub3D_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hipham <hipham@student.42heilbronn.de>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/06 23:06:34 by hipham            #+#    #+#             */
/*   Updated: 2025/02/02 16:20:30 by hipham           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/cub3D.h"

int error_sms(char *s, int ret)
{
	printf("Error: %s\n", s);
	return(ret);
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
}

float rad2deg(float rad) 
{
    return rad * (180.0 / M_PI);
}