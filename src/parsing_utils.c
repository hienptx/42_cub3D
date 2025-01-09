/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hipham <hipham@student.42heilbronn.de>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/06 23:01:38 by hipham            #+#    #+#             */
/*   Updated: 2025/01/09 21:34:03 by hipham           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/cub3D.h"

bool check_for_wall(char *line)
{
	while(*line != '\0')
	{
		if (*line != '1' && *line != ' ')
			return(0);
		line++;	
	}
	return (1);
}

bool get_player(char **maze, int i, int *longest_line, t_user_map *map)
{
	int j;
	
	j = 0;
	while (maze[i][j] != '\0')
	{
		if (maze[i][j] == '0' || ft_strchr("SNWE", maze[i][j]))
		{
			if (maze[i - 1][j] == ' ' || maze[i + 1][j] == ' '
			|| maze[i - 1][j] == '\0' || maze[i + 1][j] == '\0')
				return (printf("Maze is not closed\n"), 0);
		}
		if(!validate_player_pos(maze[i][j], j, i, map))
			return(0);
		j++;	
	}
	if (j > *longest_line)	
		*longest_line = j;
	return (1);
}

void parse_player_dir(int dx, int dy, t_user_map *map)
{
	map->pos.dx = dx;
	map->pos.dy = dy;
}

void convert_to_data(t_user_map *map, char *join)
{
	if (join != NULL)
    {
        map->map_data = ft_split(join, '\n');
        free(join);
    }
}

void save_texture(char **dir, char *path)
{
	size_t len;

	len = ft_strlen(path);
	ft_malloc((void**)dir, len);
	ft_memcpy(*dir, path, len - 1);
}
