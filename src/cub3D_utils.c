/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cub3D_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dongjle2 <dongjle2@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/06 23:06:34 by hipham            #+#    #+#             */
/*   Updated: 2025/02/03 10:00:57 by dongjle2         ###   ########.fr       */
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
}

// float rad2deg(float rad) 
// {
//     return rad * (180.0 / M_PI);
// }