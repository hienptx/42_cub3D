/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   validation_bonus.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hipham <hipham@student.42heilbronn.de>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/06 23:00:50 by hipham            #+#    #+#             */
/*   Updated: 2025/02/08 16:45:00 by hipham           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/cub3D_bonus.h"

bool	validate_texture_path(char *path)
{
	int	fd;

	fd = open(path, O_RDONLY);
	if (fd == -1)
	{
		perror("Error");
		return (0);
	}
	close(fd);
	return (1);
}

bool	validate_maze(char **maze, t_user_map *map)
{
	char	*last_1;
	size_t	i;
	size_t	longest_line;

	longest_line = 0;
	i = 0;
	while (maze[i] != NULL)
		i++;
	map->map_height = i;
	if (!check_for_wall(maze[0]) || !check_for_wall(maze[i - 1]))
		return (printf("Maze is not closed\n"), 0);
	i = -1;
	while (++i < map->map_height)
	{
		last_1 = ft_strrchr(maze[i], '1');
		last_1++;
		if (*last_1 != '\0')
			return (printf("Maze is not closed\n"), 0);
		if (!get_player(maze, i, &longest_line, map))
			return (0);
	}
	if (map->player_count != 1)
		return (printf("Invalid number of player\n"), 0);
	map->map_width = longest_line;
	return (1);
}

// no(0, -1); so(0, 1); ea(1, 0); we(-1,0)
bool	validate_player_pos(char c, unsigned int j, unsigned int i,
		t_user_map *map)
{
	if (ft_strchr("NSEW", c))
	{
		map->player_count += 1;
		map->pos.x = j;
		map->pos.y = i;
		if (c == 'N')
			parse_player_dir(0, -1, map);
		else if (c == 'S')
			parse_player_dir(0, 1, map);
		else if (c == 'E')
			parse_player_dir(1, 0, map);
		else if (c == 'W')
			parse_player_dir(-1, 0, map);
	}
	else if (ft_strchr("01\t ", c))
		;
	else
		return (0);
	return (1);
}

bool	validate_color_values(int *color)
{
	size_t	i;

	i = 0;
	while (i < 3)
	{
		if (color[i] < 0 || color[i] > 255)
			return (false);
		i++;
	}
	return (true);
}

bool	validate_map(t_user_map *map)
{
	if (map->no_texture == NULL || map->so_texture == NULL
		|| map->we_texture == NULL || map->ea_texture == NULL
		|| map->texture_count > 4)
		return (printf("Error: Invalid texture\n"), 0);
	if (map->ceiling == NULL || map->floor == NULL || map->color_count > 2)
		return (printf("Error: Invalid color\n"), 0);
	if (map->map_data == NULL || map->valid_status == 1)
		return (printf("Error: Invalid map data\n"), 0);
	if (!validate_texture_path(map->no_texture))
		return (printf("Error: Invalid texture\n"), 0);
	if (!validate_texture_path(map->so_texture))
		return (printf("Error: Invalid texture\n"), 0);
	if (!validate_texture_path(map->we_texture))
		return (printf("Error: Invalid texture\n"), 0);
	if (!validate_texture_path(map->ea_texture))
		return (printf("Error: Invalid texture\n"), 0);
	if (!validate_color_values(map->ceiling))
		return (printf("Error: Invalid color\n"), 0);
	if (!validate_color_values(map->floor))
		return (printf("Error: Invalid color\n"), 0);
	if (!validate_maze(map->map_data, map))
		return (printf("Error: Invalid maze\n"), 0);
	return (1);
}
