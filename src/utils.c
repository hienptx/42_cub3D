/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dongjle2 <dongjle2@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/07 15:41:32 by dongjle2          #+#    #+#             */
/*   Updated: 2025/02/07 15:42:04 by dongjle2         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/cub3D.h"

mlx_texture_t	*get_wall_texture(t_cub3d *data, t_ray_data *ray)
{
	if (ray->color == 0)
	{
		if (ray->dirx >= 0)
			return (data->texture[3]);
		else
			return (data->texture[2]);
	}
	else
	{
		if (ray->diry >= 0)
			return (data->texture[1]);
		else
			return (data->texture[0]);
	}
}
