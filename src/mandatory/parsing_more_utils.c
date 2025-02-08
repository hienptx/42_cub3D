/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_more_utils.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hipham <hipham@student.42heilbronn.de>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/08 16:36:42 by hipham            #+#    #+#             */
/*   Updated: 2025/02/08 19:52:26 by hipham           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/cub3D.h"

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
