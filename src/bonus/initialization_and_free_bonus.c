/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   initialization_and_free_bonus.c                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hipham <hipham@student.42heilbronn.de>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/06 22:54:20 by hipham            #+#    #+#             */
/*   Updated: 2025/02/08 19:58:49 by hipham           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/cub3D_bonus.h"

void	map_initialising(t_user_map *map)
{
	map->map_width = 0;
	map->map_height = 0;
	map->pw = 0;
	map->ph = 0;
	map->map_data = NULL;
	map->no_texture = NULL;
	map->so_texture = NULL;
	map->we_texture = NULL;
	map->ea_texture = NULL;
	map->ceiling = NULL;
	map->floor = NULL;
	map->player_count = 0;
	map->texture_count = 0;
	map->color_count = 0;
	map->counter.floor = 0;
	map->counter.ceiling = 0;
	map->counter.east = 0;
	map->counter.west = 0;
	map->counter.north = 0;
	map->counter.south = 0;
}

void	ft_free_map(t_user_map *map)
{
	if (map->map_data != NULL)
		ft_free(map->map_data);
	if (map->no_texture != NULL)
		free(map->no_texture);
	if (map->so_texture != NULL)
		free(map->so_texture);
	if (map->we_texture != NULL)
		free(map->we_texture);
	if (map->ea_texture != NULL)
		free(map->ea_texture);
	if (map->floor != NULL)
		free(map->floor);
	if (map->ceiling != NULL)
		free(map->ceiling);
}

void	cub3d_initialising(t_cub3d *data)
{
	if (data->map.map_width < 10)
		data->cell_size = 16;
	else
		data->cell_size = 256 / data->map.map_width;
	data->offset = data->cell_size / 7;
	data->iheight = data->map.map_height * data->cell_size;
	data->iwidth = data->map.map_width * data->cell_size;
	data->pos.dx = data->map.pos.dx;
	data->pos.dy = data->map.pos.dy;
	data->map.pw = data->iwidth / data->map.map_width;
	data->map.ph = data->iheight / data->map.map_height;
	data->pos.x = data->map.pos.x * data->map.pw + data->map.pw / 2.0;
	data->pos.y = data->map.pos.y * data->map.ph + data->map.ph / 2.0;
	data->pos.angle = atan2(-data->pos.dy, data->pos.dx);
}

void	ft_delete_texture(t_cub3d *data)
{
	mlx_delete_texture(data->texture[0]);
	mlx_delete_texture(data->texture[1]);
	mlx_delete_texture(data->texture[2]);
	mlx_delete_texture(data->texture[3]);
	mlx_delete_texture(data->wp.texture[0]);
	mlx_delete_texture(data->wp.texture[1]);
	mlx_delete_texture(data->wp.texture[2]);
}

void	ft_free_texture(t_cub3d *data)
{
	free(data->map.ea_texture);
	free(data->map.no_texture);
	free(data->map.so_texture);
	free(data->map.we_texture);
}
