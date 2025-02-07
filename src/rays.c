/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   rays.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dongjle2 <dongjle2@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/28 02:23:42 by dongjle2          #+#    #+#             */
/*   Updated: 2025/02/07 15:24:10 by dongjle2         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/cub3D.h"

static void	calculate_ray_intersection(t_cub3d *data, float ray_angle)
{
	float		disv;
	float		dish;
	t_ray_data	*ray;

	ray = &data->ray;
	ray->angle = ray_angle;
	ray->tan_angle = tan(ray_angle);
	disv = check_vertical_intersection(data, &ray->vx, &ray->vy);
	dish = check_horizontal_intersection(data, &ray->rx, &ray->ry);
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
	draw_single_slice(data, i, corrected_dist, ray);
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
	float			start_angle;
	unsigned int	i;
	float			ra;

	data = param;
	data->ray = init_ray(data);
	start_angle = data->pos.angle + (FOV / 2.0);
	i = 0;
	while (i < NUM_RAYS)
	{
		ra = start_angle - (i * data->ray.angle_step);
		calculate_ray_intersection(data, ra);
		render_single_ray(data, &data->ray, i, ra);
		i++;
	}
}
