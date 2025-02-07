/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dongjle2 <dongjle2@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/27 22:22:22 by dongjle2          #+#    #+#             */
/*   Updated: 2025/02/07 15:08:29 by dongjle2         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/cub3D.h"

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
	data->mlx = mlx_init(WIDTH, HEIGHT, "4242424242", true);
	if (!data->mlx)
		ft_error();
	cub3d_initialising(data);
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
	cast_ray(&data);
	mlx_key_hook(data.mlx, &my_keyhook, &data);
	mlx_loop(data.mlx);
	ft_delete_texture(&data);
	mlx_terminate(data.mlx);
	ft_free_map(&data.map);
}
