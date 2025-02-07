/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hipham <hipham@student.42heilbronn.de>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/27 22:22:22 by dongjle2          #+#    #+#             */
/*   Updated: 2025/02/05 19:17:13 by hipham           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../MLX/include/MLX42/MLX42_Int.h"
#include "../include/cub3D.h"

// #define DEG2RAD(angle_in_degrees) ((angle_in_degrees)*M_PI / 180.0)
// #define RAD2DEG(angle_in_radians) ((angle_in_radians)*180.0 / M_PI)

void	my_keyhook(mlx_key_data_t keydata, void *param)
{
	t_cub3d	*data;
	mlx_t	*mlx;

	(void)keydata;
	data = param;
	mlx = data->mlx;
	if (mlx_is_key_down(mlx, MLX_KEY_ESCAPE))
	{
		mlx_close_window(mlx);
	}
	else
	{
		handle_movement_keys(data);
	}
}

void	put_win_and_images(t_cub3d *data)
{
	data->mlx = mlx_init(WIDTH, HEIGHT, "42Balls", true);
	if (!data->mlx)
		ft_error();
	cub3d_initialising(data);
	data->img = mlx_new_image(data->mlx, data->iwidth, data->iheight);
	if (!data->img || (mlx_image_to_window(data->mlx, data->img, 0, 0) < 0))
		ft_error();
	data->img2 = mlx_new_image(data->mlx, SCREEN_WIDTH, SCREEN_HEIGHT);
	if (!data->img2 || (mlx_image_to_window(data->mlx, data->img2, 0, 0) < 0))
		ft_error();
	load_png_texture(data);
}

int32_t	main(int ac, char *av[])
{
	char	*path;
	t_cub3d	data;

	if (ac != 2)
		error_sms("Error: Invalid input!\n./cub3D [MAP.cub]\n");
	path = av[1];
	map_initialising(&data.map);
	if (parsed_map(path, &data) == 0)
	{
		ft_free_map(&data.map);
		error_sms("Error: Invalid map\n");
	}
	put_win_and_images(&data);
	reset_trigger(&data);
	mlx_key_hook(data.mlx, &my_keyhook, &data);
	mlx_loop_hook(data.mlx, update_game_state, &data);
	mlx_loop(data.mlx);
	ft_delete_texture(&data);
	mlx_terminate(data.mlx);
	ft_free_map(&data.map);
}
