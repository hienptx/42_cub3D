/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   movements_and_rotation.c                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dongjle2 <dongjle2@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/27 22:21:31 by dongjle2          #+#    #+#             */
/*   Updated: 2025/02/03 18:47:55 by dongjle2         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/cub3D.h"
#define M_PI_16 M_PI_4 / 4.0

int	check_collision(t_cub3d *data, float x, float y)
{
	int	ipx;
	int	ipy;

	ipx = x / data->map.pw;
	ipy = y / data->map.ph;
	return (data->map.map_data[ipy][ipx] == '1');
}

int	wall_collision(t_cub3d *data, float new_x, float new_y)
{
	float	offset;
	float	check_x1;
	float	check_y1;
	float	check_x2;
	float	check_y2;

	offset = 2;
	check_x1 = new_x + data->pos.dx * offset;
	check_y1 = new_y + data->pos.dy * offset;
	check_x2 = new_x - data->pos.dx * offset;
	check_y2 = new_y - data->pos.dy * offset;
	return (check_collision(data, check_x1, check_y1) || \
			check_collision(data, check_x2, check_y1) || \
			check_collision(data, check_x1, check_y2) || \
			check_collision(data, check_x2, check_y2));
}

void	move_forward(t_cub3d *data, int dir)
{
	float	new_y;
	float	new_x;

	new_y = data->pos.y + dir * data->pos.dy * MOVING_SPEED;
	new_x = data->pos.x + dir * data->pos.dx * MOVING_SPEED;
	// Check for collisions
	data->pos.angle = atan2(-data->pos.dy, data->pos.dx);
	if (!wall_collision(data, new_x, new_y))
	{
		// Update the position if no collision
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
	// Check for collisions
	data->pos.angle = atan2(-data->pos.dy, data->pos.dx);
	if (!wall_collision(data, new_x, new_y))
	{
		// Update the position if no collision
		data->pos.x = new_x;
		data->pos.y = new_y;
	}
}

void handle_key_action(t_cub3d *data, int *key_pressed)
{
	memset(data->img2->pixels, 255, data->img2->width * data->img2->height * sizeof(int32_t));
	put_pixel_box(data, 0xFFFFFFFF);
	*key_pressed = true;
}

void handle_key_press(t_cub3d *data, int dir, void (*move_func)(t_cub3d *, int), int *key_pressed)
{
	move_func(data, dir);
	handle_key_action(data, key_pressed);
}

void handle_rotation(t_cub3d *data, float angle, int *key_pressed)
{
	rotate(data, angle);
	handle_key_action(data, key_pressed);
}

void handle_movement_keys(t_cub3d *data)
{
	mlx_t* mlx = data->mlx;
	int key_pressed = false;

	if (mlx_is_key_down(mlx, MLX_KEY_W))
		handle_key_press(data, 1, move_forward, &key_pressed);
	if (mlx_is_key_down(mlx, MLX_KEY_S))
		handle_key_press(data, -1, move_forward, &key_pressed);
	if (mlx_is_key_down(mlx, MLX_KEY_A))
		handle_key_press(data, -1, move_left_right, &key_pressed);
	if (mlx_is_key_down(mlx, MLX_KEY_D))
		handle_key_press(data, 1, move_left_right, &key_pressed);
	if (mlx_is_key_down(mlx, MLX_KEY_LEFT))
		handle_rotation(data, M_PI_16, &key_pressed);
	if (mlx_is_key_down(mlx, MLX_KEY_RIGHT))
		handle_rotation(data, -M_PI_16, &key_pressed);

	if (key_pressed)
	{
		// printf("x = %f, y = %f\n", data->pos.dx, data->pos.dy);
		cast_ray(data);
	}
}
