/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hipham <hipham@student.42heilbronn.de>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/06 23:01:38 by hipham            #+#    #+#             */
/*   Updated: 2025/02/08 18:12:38 by hipham           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/cub3D.h"

bool	get_player(char **maze, int i, size_t *longest_line, t_user_map *map)
{
	size_t	j;

	j = 0;
	while (maze[i][j] != '\0')
	{
		if (maze[i][j] == '0' || ft_strchr("SNWE", maze[i][j]))
		{
			if (maze[i - 1][j] == ' ' || maze[i + 1][j] == ' '
				|| maze[i - 1][j] == '\0' || maze[i + 1][j] == '\0')
				return (printf("Maze is not closed\n"), 0);
		}
		if (!validate_player_pos(maze[i][j], j, i, map))
			return (0);
		j++;
	}
	if (j > *longest_line)
		*longest_line = j;
	return (1);
}

void	parse_player_dir(int dx, int dy, t_user_map *map)
{
	map->pos.dx = dx;
	map->pos.dy = dy;
}

void	convert_to_data(t_user_map *map, char *join)
{
	if (join != NULL)
	{
		map->map_data = ft_split(join, '\n');
		free(join);
	}
}

void	save_texture(t_user_map *map, char **dir, char *path, char *s)
{
	size_t	len;

	len = ft_strlen(path);
	*dir = ft_calloc(len, 1);
	ft_memcpy(*dir, path, len - 1);
	if (ft_strcmp(s, "EA") == 0)
		map->counter.east++;
	if (ft_strcmp(s, "WE") == 0)
		map->counter.west++;
	if (ft_strcmp(s, "SO") == 0)
		map->counter.south++;
	if (ft_strcmp(s, "NO") == 0)
		map->counter.north++;
}

void	copy_color(t_user_map *map, int *color_arr, size_t size, char *line)
{
	if (!ft_strcmp(line, "F"))
	{
		if (map->counter.floor > 0)
			return ;
		ft_malloc((void **)&map->floor, size);
		ft_memcpy(map->floor, color_arr, size);
		map->counter.floor++;
	}
	if (!ft_strcmp(line, "C"))
	{
		if (map->counter.ceiling > 0)
			return ;
		ft_malloc((void **)&map->ceiling, size);
		ft_memcpy(map->ceiling, color_arr, size);
		map->counter.ceiling++;
	}
}
