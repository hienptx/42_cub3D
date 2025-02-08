/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_more_utils_bonus.c                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hipham <hipham@student.42heilbronn.de>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/08 16:36:42 by hipham            #+#    #+#             */
/*   Updated: 2025/02/08 22:01:28 by hipham           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/cub3D_bonus.h"

bool	process_color_values(char *s, char **arr, char ***color)
{
	if (s[ft_strlen(s) - 1] == '\n')
		s[ft_strlen(s) - 1] = '\0';
	if (count_comma(s) != 2)
	{
		ft_free(arr);
		return (false);
	}
	*color = ft_split(s, ',');
	if (!*color)
	{
		ft_free(arr);
		return (false);
	}
	return (true);
}

bool	check_for_char(const char *str)
{
	int	i;

	i = 0;
	while (str[i] && str[i] != '\n')
	{
		if (!ft_isdigit(str[i]))
			return (false);
		i++;
	}
	return (true);
}

int	count_comma(char *s)
{
	int	count;

	count = 0;
	while (*s != '\0')
	{
		if (*s == ',')
			count++;
		s++;
	}
	return (count);
}

bool	convert_to_int(int *i, int *rgb, char **arr, char **color)
{
	if (!check_for_char(color[*i]))
	{
		ft_free(color);
		ft_free(arr);
		return (false);
	}
	rgb[*i] = ft_atoi(color[*i]);
	return (true);
}

uint32_t	calculate_tex_x(int is_vertical, t_wall_data *wall, t_ray_data *ray,
	t_cub3d *data)
{
	uint32_t	tex_x;

	if (is_vertical == 0)
	{
		if (data->ray.dirx >= 0)
			tex_x = get_texture_x(ray->hit_x, wall->texture->width,
					data->cell_size);
		else
			tex_x = wall->texture->width - get_texture_x(ray->hit_x,
					wall->texture->width, data->cell_size) - 1;
	}
	else
	{
		if (ray->diry >= 0)
			tex_x = get_texture_x(ray->hit_y, wall->texture->width,
					data->cell_size);
		else
			tex_x = wall->texture->width - get_texture_x(ray->hit_y,
					wall->texture->width, data->cell_size) - 1;
	}
	return (tex_x);
}
