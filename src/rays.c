/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   rays.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hipham <hipham@student.42heilbronn.de>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/28 02:23:42 by dongjle2          #+#    #+#             */
/*   Updated: 2025/02/05 13:34:11 by hipham           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/cub3D.h"

static void	calculate_ray_intersection(t_cub3d *data, float ray_angle,
		t_ray_data *ray)
{
	float	disv;
	float	dish;

	ray->angle = ray_angle;
	ray->tan_angle = tan(ray_angle);
	disv = check_vertical_intersection(data, ray, &ray->vx, &ray->vy);
	dish = check_horizontal_intersection(data, ray, &ray->rx, &ray->ry);
	if (disv < dish)
	{
		ray->hit_x = ray->vx;
		ray->hit_y = ray->vy;
		ray->distance = disv;
		ray->color = 1;
	}
	else
	{
		ray->hit_x = ray->rx;
		ray->hit_y = ray->ry;
		ray->distance = dish;
		ray->color = 0;
	}
}

void	render_single_ray(t_cub3d *data, t_ray_data *ray, unsigned int i,
		float ra)
{
	float	corrected_dist;
	float	ca;

	ca = adjust_angle(ra - data->pos.angle);
	corrected_dist = ray->distance * cos(ca);
	draw_wall_slice(data, i, corrected_dist, ray);
	draw_ray(data, ray->px, ray->py, ra, ray->distance, 0x00FF00FF);
}

t_ray_data	init_ray(t_cub3d *data)
{
	t_ray_data	ray;

	ray.angle_step = FOV / NUM_RAYS;
	ray.px = data->pos.x;
	ray.py = data->pos.y;
	return (ray);
}

void	cast_ray(void *param)
{
	t_cub3d			*data;
	t_ray_data		ray;
	float			start_angle;
	unsigned int	i;
	float			ra;

	data = param;
	ray = init_ray(data);
	start_angle = data->pos.angle + (FOV / 2.0);
	render_map(data->map.map_data, data);
	i = 0;
	while (i < NUM_RAYS)
	{
		ra = start_angle - (i * ray.angle_step);
		calculate_ray_intersection(data, ra, &ray);
		render_single_ray(data, &ray, i, ra);
		i++;
	}
	mlx_image_to_window(data->mlx, data->img, 0, 0);
	put_weapon(data);
}
