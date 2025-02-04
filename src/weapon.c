/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   weapon.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hipham <hipham@student.42heilbronn.de>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/03 13:53:19 by hipham            #+#    #+#             */
/*   Updated: 2025/02/04 19:07:41 by hipham           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/cub3D.h"

void	put_object(t_cub3d *data, mlx_texture_t *obj, float obj_x, float obj_y)
{
	unsigned int	i;
	unsigned int	j;
	uint32_t		pix_color;

	i = 0;
	while (i < obj->width)
	{
		j = 0;
		while (j < obj->height)
		{
			pix_color = get_pixel_color(obj, i, j);
			if (pix_color != 0x00000000)
			{
				if ((obj_x + i) < data->img2->width && (obj_y
						+ j) < data->img2->height)
					mlx_put_pixel(data->img2, obj_x + i, obj_y + j, pix_color);
			}
			j++;
		}
		i++;
	}
}

void	trigger_flash(t_cub3d *data)
{
	data->wp.is_flash_active = true;
	data->wp.flash_x = (data->img2->width) / 3.25;
	data->wp.flash_y = (data->img2->height / 2);
	data->wp.timer = time(NULL);
}

void	reset_trigger(t_cub3d *data)
{
	data->wp.is_flash_active = false;
	data->wp.flash_x = 0;
	data->wp.flash_y = 0;
	data->wp.timer = 0;
}

void	update_game_state(void *param)
{
	t_cub3d	*data;

	data = param;
	if (data->wp.is_flash_active)
	{
		if (time(NULL) - data->wp.timer > 0.005)
			reset_trigger(data);
	}
	cast_ray(data);
	put_weapon(data);
}

void	put_weapon(t_cub3d *data)
{
	t_weapon *weapon = &data->wp;
	weapon->gun_x = (data->img2->width - weapon->texture[0]->width) / 2;
	weapon->gun_y = (data->img2->height * 4 / 3) - weapon->texture[0]->height;
	weapon->aim_x = (data->img2->width - weapon->texture[1]->width) / 2;
	weapon->aim_y = (data->img2->height - weapon->texture[1]->height) / 2;
	if (weapon->is_flash_active)
		put_object(data, weapon->texture[2], weapon->flash_x, weapon->flash_y);
	put_object(data, weapon->texture[0], weapon->gun_x, weapon->gun_y);
	put_object(data, weapon->texture[1], weapon->aim_x, weapon->aim_y);
}
