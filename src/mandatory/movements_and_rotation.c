/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   movements_and_rotation.c                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hipham <hipham@student.42heilbronn.de>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/27 22:21:31 by dongjle2          #+#    #+#             */
/*   Updated: 2025/02/07 18:09:01 by hipham           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/cub3D.h"

int	wall_collision(t_cub3d *data, float new_x, float new_y)
{
	float	check_x1;
	float	check_y1;
	float	check_x2;
	float	check_y2;

	check_x1 = new_x + data->pos.dx * data->offset;
	check_y1 = new_y + data->pos.dy * data->offset;
	check_x2 = new_x - data->pos.dx * data->offset;
	check_y2 = new_y - data->pos.dy * data->offset;
	return (check_collision(data, check_x1, check_y1)
		|| check_collision(data, check_x2, check_y1)
		|| check_collision(data, check_x1, check_y2)
		|| check_collision(data, check_x2, check_y2));
}

void	rotate(t_cub3d *data, float unit_degree)
{
	data->pos.angle = atan2(-data->pos.dy, data->pos.dx);
	data->pos.angle = adjust_angle(data->pos.angle + unit_degree);
	data->pos.dx = cos(data->pos.angle);
	data->pos.dy = -sin(data->pos.angle);
}

void	move_forward(t_cub3d *data, int dir)
{
	float	new_y;
	float	new_x;

	new_y = data->pos.y + dir * data->pos.dy * MOVING_SPEED;
	new_x = data->pos.x + dir * data->pos.dx * MOVING_SPEED;
	if (!wall_collision(data, new_x, new_y))
	{
		data->pos.x = new_x;
		data->pos.y = new_y;
	}
}

void	move_left_right(t_cub3d *data, int dir)
{
	float	perp_dx;
	float	perp_dy;
	float	length;
	float	new_x;
	float	new_y;

	perp_dx = -data->pos.dy;
	perp_dy = data->pos.dx;
	length = sqrt(perp_dx * perp_dx + perp_dy * perp_dy);
	perp_dx /= length;
	perp_dy /= length;
	new_x = data->pos.x + dir * perp_dx * MOVING_SPEED;
	new_y = data->pos.y + dir * perp_dy * MOVING_SPEED;
	if (!wall_collision(data, new_x, new_y))
	{
		data->pos.x = new_x;
		data->pos.y = new_y;
	}
}
