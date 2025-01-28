/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_ray_distance.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dongjle2 <dongjle2@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/28 02:27:36 by dongjle2          #+#    #+#             */
/*   Updated: 2025/01/28 03:09:27 by dongjle2         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/cub3D.h"

static bool is_wall_hit(t_cub3d *data, int mx, int my)
{
	return (mx >= 0 && my >= 0 && 
			mx < data->map.map_width && 
			my < data->map.map_height && 
			data->map.map_data[my][mx] == '1');
}

// static void calculate_distance(t_intersection *inter, t_ray_data *ray)
// {
// 	inter->distance = sqrt(pow(inter->rx - ray->px, 2) + 
// 							pow(inter->ry - ray->py, 2));
// }

static void init_intersection(t_intersection *inter)
{
	inter->distance = 1000000;
	inter->dof = 0;
}

float check_horizontal_intersection(t_cub3d *data, float ra, float px, float py, float *rx, float *ry)
{
	t_intersection inter;
	t_ray_data ray = {ra, tan(ra), px, py};

	init_intersection(&inter);
	if (sin(ra) > 0.001)
		init_ray_up(&inter, ray);
	else if (sin(ra) < -0.001)
		init_ray_down(&inter, ray);
	else
	{
		inter.rx = px;
		inter.ry = py;
		inter.dof = data->map.map_height;
	}
	while (inter.dof < data->map.map_height)
	{
		int mx = floor(inter.rx / cell_size);
		int my = floor(inter.ry / cell_size);
		if (is_wall_hit(data, mx, my))
		{
			inter.distance = sqrt(pow(inter.rx - px, 2) + pow(inter.ry - py, 2));
			*rx = inter.rx;
			*ry = inter.ry;
			break;
		}
		inter.rx += inter.xo;
		inter.ry += inter.yo;
		inter.dof++;
	}
	return (inter.distance);
}

float check_vertical_intersection(t_cub3d *data, float ra, float px, float py, float *vx, float *vy)
{
	t_intersection inter;
	t_ray_data ray = {ra, tan(ra), px, py};
	// float tan_ra;

	// tan_ra = tan(ra);
	init_intersection(&inter);
	if (cos(ra) > 0.001)
		init_ray_right(&inter, ray);
	else if (cos(ra) < -0.001)
		init_ray_left(&inter, ray);
	else
	{
		inter.rx = px;
		inter.ry = py;
		inter.dof = data->map.map_width;
	}
	while (inter.dof < data->map.map_width)
	{
		int mx = floor(inter.rx / cell_size);
		int my = floor(inter.ry / cell_size);
		if (is_wall_hit(data, mx, my))
		{
			inter.distance = sqrt(pow(inter.rx - px, 2) + pow(inter.ry - py, 2));
			*vx = inter.rx;
			*vy = inter.ry;
			break;
		}
		inter.rx += inter.xo;
		inter.ry += inter.yo;
		inter.dof++;
	}
	return (inter.distance);
}
