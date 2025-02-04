/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   movements_and_rotation.c                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hipham <hipham@student.42heilbronn.de>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/27 22:21:31 by dongjle2          #+#    #+#             */
/*   Updated: 2025/02/04 17:55:32 by hipham           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/cub3D.h"
#define M_PI_16 M_PI_4 / 4.0

int	wall_collision(t_cub3d *data, float new_x, float new_y)
{
	float	offset;
	float	check_x1;
	float	check_y1;
	float	check_x2;
	float	check_y2;
	int		ipx1;
	int		ipy1;
	int		ipx2;
	int		ipy2;

	// Define an offset value to detect collisions beforehand
	offset = 2;
	// Calculate the new positions with offset in the direction of movement
	check_x1 = new_x + data->pos.dx * offset;
	check_y1 = new_y + data->pos.dy * offset;
	check_x2 = new_x - data->pos.dx * offset;
	check_y2 = new_y - data->pos.dy * offset;
	// Calculate the map grid positions
	ipx1 = check_x1 / data->map.pw;
	ipy1 = check_y1 / data->map.ph;
	ipx2 = check_x2 / data->map.pw;
	ipy2 = check_y2 / data->map.ph;
	return (data->map.map_data[ipy1][ipx1] == '1' ||
			data->map.map_data[ipy2][ipx1] == '1' ||
			data->map.map_data[ipy1][ipx2] == '1' ||
			data->map.map_data[ipy2][ipx2] == '1');
}

void	move_forward(t_cub3d *data, int dir)
{
	float	new_x;
	float	new_y;

	// Calculate the new position
	new_x = data->pos.x + dir * data->pos.dx * MOVING_SPEED;
	new_y = data->pos.y + dir * data->pos.dy * MOVING_SPEED;
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

	// Calculate the perpendicular direction
	perp_dx = -data->pos.dy;
	perp_dy = data->pos.dx;
	// Normalize the perpendicular direction
	length = sqrt(perp_dx * perp_dx + perp_dy * perp_dy);
	perp_dx /= length;
	perp_dy /= length;
	// Calculate the new position
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

void	handle_key_action(t_cub3d *data, int *key_pressed)
{
	memset(data->img2->pixels, 255, data->img2->width * data->img2->height
			* sizeof(int32_t));
	put_pixel_box(data, 0xFFFFFFFF);
	*key_pressed = true;
}

void	handle_key_press(t_cub3d *data, int dir, void (*move_func)(t_cub3d *,
			int), int *key_pressed)
{
	move_func(data, dir);
	handle_key_action(data, key_pressed);
}

void	handle_rotation(t_cub3d *data, float angle, int *key_pressed)
{
	rotate(data, angle);
	handle_key_action(data, key_pressed);
}

void	handle_movement_keys(t_cub3d *data)
{
	mlx_t	*mlx;
	int		key_pressed;

	mlx = data->mlx;
	key_pressed = false;
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
	if (mlx_is_key_down(data->mlx, MLX_KEY_SPACE))
		trigger_flash(data);
	if (key_pressed)
		cast_ray(data);
}
