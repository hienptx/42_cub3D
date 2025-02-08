/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_more_utils_bonus.c                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hipham <hipham@student.42heilbronn.de>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/08 16:36:42 by hipham            #+#    #+#             */
/*   Updated: 2025/02/08 16:50:01 by hipham           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/cub3D_bonus.h"

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

void	convert_to_int(int *i, int *rgb, char **arr, char **color)
{
	if (!check_for_char(color[*i]))
	{
		ft_free(color);
		ft_free(arr);
		return ;
	}
	rgb[*i] = ft_atoi(color[*i]);
}
