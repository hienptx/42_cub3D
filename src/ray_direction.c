/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ray_direction.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dongjle2 <dongjle2@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/28 02:23:42 by dongjle2          #+#    #+#             */
/*   Updated: 2025/01/28 02:43:35 by dongjle2         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/cub3D.h"

void init_ray_up(t_intersection *inter, t_ray_data ray)
{
	inter->ry = floor(ray.py / cell_size) * cell_size - 0.01;
	inter->rx = (ray.py - inter->ry) * ray.tan_angle + ray.px;
	inter->yo = -cell_size;
	inter->xo = -inter->yo * ray.tan_angle;
}

void init_ray_down(t_intersection *inter, t_ray_data ray)
{
	inter->ry = floor(ray.py / cell_size) * cell_size + cell_size;
	inter->rx = (ray.py - inter->ry) * ray.tan_angle + ray.px;
	inter->yo = cell_size;
	inter->xo = -inter->yo * ray.tan_angle;
}

void init_ray_right(t_intersection *inter, t_ray_data ray)
{
	inter->rx = floor(ray.px / cell_size) * cell_size + cell_size;
	inter->ry = (ray.px - inter->rx) * ray.tan_angle + ray.py;
	inter->xo = cell_size;
	inter->yo = -inter->xo * ray.tan_angle;
}

void init_ray_left(t_intersection *inter, t_ray_data ray)
{
	inter->rx = floor(ray.px / cell_size) * cell_size - 0.01;
	inter->ry = (ray.px - inter->rx) * ray.tan_angle + ray.py;
	inter->xo = -cell_size;
	inter->yo = -inter->xo * ray.tan_angle;
}
