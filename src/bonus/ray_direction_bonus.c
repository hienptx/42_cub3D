/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ray_direction_bonus.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hipham <hipham@student.42heilbronn.de>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/28 02:23:42 by dongjle2          #+#    #+#             */
/*   Updated: 2025/02/07 19:20:15 by hipham           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/cub3D_bonus.h"

void	init_ray_up(t_intersection *inter, t_ray_data *ray, float cell_size)
{
	inter->ry = floor(ray->py / cell_size) * cell_size - 0.01;
	inter->rx = ray->px + (ray->py - inter->ry) * (1 / ray->tan_angle);
	inter->yo = -cell_size;
	inter->xo = -inter->yo * (1 / ray->tan_angle);
}

void	init_ray_down(t_intersection *inter, t_ray_data *ray, float cell_size)
{
	inter->ry = floor(ray->py / cell_size) * cell_size + cell_size;
	inter->rx = ray->px + (ray->py - inter->ry) * (1 / ray->tan_angle);
	inter->yo = cell_size;
	inter->xo = -inter->yo * (1 / ray->tan_angle);
}

void	init_ray_right(t_intersection *inter, t_ray_data *ray, float cell_size)
{
	inter->rx = floor(ray->px / cell_size) * cell_size + cell_size;
	inter->ry = (ray->px - inter->rx) * ray->tan_angle + ray->py;
	inter->xo = cell_size;
	inter->yo = -inter->xo * ray->tan_angle;
}

void	init_ray_left(t_intersection *inter, t_ray_data *ray, float cell_size)
{
	inter->rx = floor(ray->px / cell_size) * cell_size - 0.01;
	inter->ry = (ray->px - inter->rx) * ray->tan_angle + ray->py;
	inter->xo = -cell_size;
	inter->yo = -inter->xo * ray->tan_angle;
}
