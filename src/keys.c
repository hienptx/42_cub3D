/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   keys.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dongjle2 <dongjle2@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/05 01:24:09 by hipham            #+#    #+#             */
/*   Updated: 2025/02/06 08:57:07 by dongjle2         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/cub3D.h"

int	check_collision(t_cub3d *data, float x, float y)
{
	int	ipx;
	int	ipy;

	ipx = x / data->map.pw;
	ipy = y / data->map.ph;
	return (data->map.map_data[ipy][ipx] == '1');
}

static void	handle_key_action(t_cub3d *data, int *key_pressed)
{
	memset(data->img2->pixels, 255, data->img2->width * data->img2->height
		* sizeof(int32_t));
	*key_pressed = true;
}

static void	handle_key_press(t_cub3d *data, int dir,
	void (*move_func)(t_cub3d *, int), int *key_pressed)
{
	move_func(data, dir);
	handle_key_action(data, key_pressed);
}

static void	handle_rotation(t_cub3d *data, float angle, int *key_pressed)
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
		handle_rotation(data, M_PI_4 / 8, &key_pressed);
	if (mlx_is_key_down(mlx, MLX_KEY_RIGHT))
		handle_rotation(data, -M_PI_4 / 8, &key_pressed);
	if (key_pressed)
		cast_ray(data);
}
