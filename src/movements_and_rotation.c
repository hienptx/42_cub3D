/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   movements_and_rotation.c                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dongjle2 <dongjle2@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/27 22:21:31 by dongjle2          #+#    #+#             */
/*   Updated: 2025/01/28 03:57:10 by dongjle2         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/cub3D.h"
#define M_PI_16 M_PI_4 / 4.0

int wall_collision(t_cub3d *data, float new_x, float new_y)
{
	// Define an offset value to detect collisions beforehand
	float offset = 2;

	// Calculate the new positions with offset in the direction of movement
	float check_x1 = new_x + data->pos.dx * offset;
	float check_y1 = new_y + data->pos.dy * offset;
	float check_x2 = new_x - data->pos.dx * offset;
	float check_y2 = new_y - data->pos.dy * offset;

	// Calculate the map grid positions
	int ipx1 = check_x1 / data->map.pw;
	int ipy1 = check_y1 / data->map.ph;
	int ipx2 = check_x2 / data->map.pw;
	int ipy2 = check_y2 / data->map.ph;

	// Check for collision in the new positions with offset
	if (data->map.map_data[ipy1][ipx1] == '1' ||
		data->map.map_data[ipy2][ipx1] == '1' ||
		data->map.map_data[ipy1][ipx2] == '1' ||
		data->map.map_data[ipy2][ipx2] == '1')
	{
		return 1; // Collision detected
	}

	return 0; // No collision
}

void move_forward(t_cub3d *data, int dir)
{
	// Calculate the new position
	float new_x = data->pos.x + dir * data->pos.dx * MOVING_SPEED;
	float new_y = data->pos.y + dir * data->pos.dy * MOVING_SPEED;

	// Check for collisions
	if (!wall_collision(data, new_x, new_y))
	{
		// Update the position if no collision
		data->pos.x = new_x;
		data->pos.y = new_y;
	}
}

void move_left_right(t_cub3d *data, int dir)
{
	// Calculate the perpendicular direction
	float perp_dx = -data->pos.dy;
	float perp_dy = data->pos.dx;

	// Normalize the perpendicular direction
	float length = sqrt(perp_dx * perp_dx + perp_dy * perp_dy);
	perp_dx /= length;
	perp_dy /= length;

	// Calculate the new position
	float new_x = data->pos.x + dir * perp_dx * MOVING_SPEED;
	float new_y = data->pos.y + dir * perp_dy * MOVING_SPEED;

	// Check for collisions
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
