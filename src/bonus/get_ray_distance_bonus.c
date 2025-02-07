/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_ray_distance_bonus.c                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hipham <hipham@student.42heilbronn.de>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/28 02:27:36 by dongjle2          #+#    #+#             */
/*   Updated: 2025/02/07 19:20:15 by hipham           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/cub3D_bonus.h"

static bool	is_wall_hit(t_cub3d *data, unsigned int mx, unsigned int my)
{
	return (mx < data->map.map_width && my < data->map.map_height
		&& data->map.map_data[my][mx] == '1');
}

static void	init_ray_direction(t_intersection *inter, t_ray_data *ray,
		t_ray_dir dir, float cell_size)
{
	inter->distance = 1000;
	inter->dof = 0;
	inter->dir = dir;
	if (dir == DIR_HORIZONTAL)
	{
		ray->dirx = sin(ray->angle);
		if (ray->dirx > 0.001)
			init_ray_up(inter, ray, cell_size);
		else if (ray->dirx < -0.001)
			init_ray_down(inter, ray, cell_size);
		else
			inter->dof = MAX_MAP_HEIGHT;
	}
	else
	{
		ray->diry = cos(ray->angle);
		if (ray->diry > 0.001)
			init_ray_right(inter, ray, cell_size);
		else if (ray->diry < -0.001)
			init_ray_left(inter, ray, cell_size);
		else
			inter->dof = MAX_MAP_WIDTH;
	}
}

static float	check_intersection(t_cub3d *data, float *x,
		float *y, t_ray_dir dir)
{
	t_intersection	inter;
	int				max_dof;

	init_ray_direction(&inter, &data->ray, dir, data->cell_size);
	if (dir == DIR_HORIZONTAL)
		max_dof = data->map.map_height;
	else
		max_dof = data->map.map_width;
	while (inter.dof < max_dof)
	{
		inter.mx = floor(inter.rx / data->cell_size);
		inter.my = floor(inter.ry / data->cell_size);
		if (is_wall_hit(data, inter.mx, inter.my))
		{
			inter.distance = sqrt(pow(inter.rx - data->pos.x, 2)
					+ pow(inter.ry - data->pos.y, 2));
			*x = inter.rx;
			*y = inter.ry;
			break ;
		}
		inter.rx += inter.xo;
		inter.ry += inter.yo;
		inter.dof++;
	}
	return (inter.distance);
}

float	check_horizontal_intersection(t_cub3d *data, float *rx,
		float *ry)
{
	return (check_intersection(data, rx, ry, DIR_HORIZONTAL));
}

float	check_vertical_intersection(t_cub3d *data, float *vx,
		float *vy)
{
	return (check_intersection(data, vx, vy, DIR_VERTICAL));
}
