/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   weapon.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hipham <hipham@student.42heilbronn.de>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/03 13:53:19 by hipham            #+#    #+#             */
/*   Updated: 2025/02/03 18:01:30 by hipham           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/cub3D.h"



uint32_t	get_pixel_color(mlx_texture_t *texture, uint32_t tex_x, uint32_t tex_y)
{
	uint8_t	*pixel;
	uint32_t a;

	pixel = &texture->pixels[(texture->width * tex_y + tex_x) * 4];
	a = pixel[3];
	if(a == 0)
		return(0x00000000);
	return ((pixel[0] << 24) | (pixel[1] << 16) | (pixel[2] << 8) | pixel[3]);
}

void put_object(t_cub3d *data, mlx_texture_t *obj, float obj_x, float obj_y)
{
	unsigned int i;
	unsigned int j;
	uint32_t pix_color;
	
	i = 0;
	while (i < obj->width)
	{
		j = 0;
		while (j < obj->height)
		{
			pix_color = get_pixel_color(obj, i, j);
			if (pix_color != 0x00000000)
			{
				if ((obj_x + i) < data->img2->width && (obj_y + j) < data->img2->height) 
					mlx_put_pixel(data->img2, obj_x + i, obj_y + j, pix_color);
			}
			j++;
		}
		i++;
	}
}

void trigger_flash(t_cub3d *data)
{
	if (data.)
	data->wp.flash_x = (data->img2->width) / 3.25;
	data->wp.flash_y = (data->img2->height / 2);
	data->wp.timer = 10;
}

void put_weapon(t_cub3d *data)
{
	t_weapon weapon;

	weapon = data->wp;
	weapon.gun_x = (data->img2->width - weapon.texture[0]->width) / 2;
	weapon.gun_y = (data->img2->height * 4 / 3) - weapon.texture[0]->height;
	weapon.aim_x = (data->img2->width - weapon.texture[1]->width) / 2;
	weapon.aim_y = (data->img2->height - weapon.texture[1]->height) / 2;
	put_object(data, weapon.texture[0], weapon.gun_x, weapon.gun_y);
	put_object(data, weapon.texture[1], weapon.aim_x, weapon.aim_y);
	if (weapon.is_flash_active == true && weapon.timer > 0)
	{
		put_object(data, weapon.texture[2], weapon.flash_x, weapon.flash_y);
		weapon.timer--;
		if (weapon.timer == 0)
			weapon.is_flash_active = false;
	}
}