/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cub3D_utils_bonus.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dongjle2 <dongjle2@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/06 23:06:34 by hipham            #+#    #+#             */
/*   Updated: 2025/02/07 20:26:29 by dongjle2         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/cub3D_bonus.h"

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

// Adjust angle to be within [0, 2*PI)
float	adjust_angle(float angle)
{
	if (angle < 0)
		angle += 2.0f * M_PI;
	return (angle);
}
