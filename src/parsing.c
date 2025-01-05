/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hipham <hipham@student.42heilbronn.de>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/26 21:46:15 by hipham            #+#    #+#             */
/*   Updated: 2025/01/05 01:47:09 by hipham           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/cub3D.h"

int error_sms(char *s, int ret)
{
	printf("Error: %s\n", s);
	return(ret);
}

void ft_malloc(void **data, size_t size)
{
	*data = malloc(size);
	if (*data == NULL)
	{
		printf("Malloc failed to allocate memory\n");
		exit(EXIT_FAILURE);
	}
}

void parse_color(char *line, t_user_map *map)
{
	char **color;
	int color_arr[3];
	int i;
	char **arr;
	
	arr = ft_split(line, ' ');
	i = 0;
	while(arr[i] != NULL)
		i++;
	if (i == 2)
	{
		i = -1;
		color = ft_split(arr[1], ',');
		while (color[++i])
			color_arr[i] = ft_atoi(color[i]);
		if (!ft_strcmp(arr[0], "C"))
		{
			ft_malloc((void**)&map->ceiling, sizeof(color_arr));
			ft_memcpy(map->ceiling, color_arr, sizeof(color_arr));
		}
		if (!ft_strcmp(arr[0], "F"))
		{
			ft_malloc((void**)&map->floor, sizeof(color_arr));
			ft_memcpy(map->floor, color_arr, sizeof(color_arr));
		}
		ft_free(color);
		ft_free(arr);
	}
}

void save_texture(char **dir, char *path)
{
	size_t len;

	len = ft_strlen(path) + 1;
	ft_malloc((void**)dir, len);
	ft_memcpy(*dir, path, len);
}


void parse_texture(char *line, t_user_map *map)
{
	char **arr;
	int i;

	i = 0;
	arr = ft_split(line, ' ');
	while(arr[i] != NULL)
		i++;
	if (i == 2)
	{
		if (ft_strcmp(arr[0], "NO") == 0)
			save_texture(&map->NO_texture, arr[1]);
		else if (ft_strcmp(arr[0], "SO") == 0)
			save_texture(&map->SO_texture, arr[1]);
		else if (ft_strcmp(arr[0], "WE") == 0)
			save_texture(&map->WE_texture, arr[1]);
		else if (ft_strcmp(arr[0], "EA") == 0)
			save_texture(&map->EA_texture, arr[1]);
	}
	ft_free(arr);
}

void convert_to_data(t_user_map *map, char *join)
{
	if (join != NULL)
    {
        map->map_data = ft_split(join, '\n');
        free(join);
    }
    // for (int i = 0; map->map_data[i] != NULL; i++)
	// {
    // 	printf("%s\n", map->map_data[i]);
	// }
}

bool parsed_maze(char **line, int fd, t_user_map *map)
{
    char *tmp;
    char *join;

	join = ft_strdup("");
    while (*line)
    {
        tmp = *line;
        while (*tmp != '\0' && ft_isspace(*tmp))
            tmp++;
        if (*tmp == '1')
        {
            tmp = join;
            join = ft_strjoin(join, *line);
			free(tmp);
        }
        else
        {
            if (join != NULL)
                free(join);
            free(*line);
			return(0);
        }        
        free(*line);
        *line = get_next_line(fd);
    }
	convert_to_data(map, join);
	return (1);
}

bool map_parsing(int fd, t_user_map *map)
{
	char *line;
	char *tmp;

	line = get_next_line(fd);
	while (line)
	{
		tmp = line;
		while(*tmp != '\0' && ft_isspace(*tmp))
			tmp++;
		if (*tmp == '\0')
			;
		else if (*tmp != '\0' && (*tmp == 'F' || *tmp == 'C'))
			parse_color(tmp, map);
		else if (*tmp != '\0' && ft_strchr("NSWE", *tmp))
			parse_texture(tmp, map);
		else if (*tmp != '\0' && *tmp == '1')
		{
			if (parsed_maze(&line, fd, map) == 0)
				return (0);
			else
				return (1);
		}
		else
			return(printf("Error: Invalid maze\n"),free(line), 0);
		free(line);
		line = get_next_line(fd);
	}
	return(1);			
}

bool validate_texture_path(char *path)
{
	int fd;

	fd = open(path, O_RDONLY);
	printf("%s\n", path);
	if (fd == -1)
	{
		perror("Error");
		return (0);
	}
	close(fd);
	return (1);
}

bool validate_color_values(int *color)
{
	int i;

	i = 0;
	while (color[i])
	{
		if (color[i ] < 0 || color[i] > 255)
			return(0);
		i++;
	}
	if (i != 2)
		return (0);
	return (1);
}

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

void parse_player_dir(int dx, int dy, t_user_map *map)
{
	map->pos.dx = dx;
	map->pos.dy = dy;
}

// NO(0, -1); SO(0, 1); EA(1, 0); WE(-1,0) 
bool validate_player_pos(char c, unsigned int i, unsigned int j, t_user_map *map)
{
	if (ft_strchr("NSEW", c))
	{
		map->pos.x = i;
		map->pos.y = j;
		if(c == 'N')
			parse_player_dir(0, -1, map);	
		else if(c == 'S')
			parse_player_dir(0, 1, map);	
		else if(c == 'N')
			parse_player_dir(1, 0, map);	
		else if(c == 'W')
			parse_player_dir(-1, 0, map);			
	}
	else if (ft_strchr("01", c))
		;
	else
		return (printf("Error: Invalid player\n"), 0);
	return (1);
}

bool validate_maze(char **maze, t_user_map *map)
{
	char *last_1;
	int i;
	int j;
	int longest_line;
	
	longest_line = 0;
	i = 0;
	while(maze[i] != NULL)
		i++;
	map->map_height = i;
	if (!check_for_wall(maze[0]) || !check_for_wall(maze[i - 1]))
		return (printf("Maze is not closed\n"), 0);
	i = 0;
	while (i < map->map_height)
	{
		last_1 = ft_strrchr(maze[i], '1');
		last_1++;
		if (*last_1 != '\0')
			return (printf("Maze is not closed\n"), 0);
		j = 0;
		while (maze[i][j] != '\0')
		{
			if (maze[i][j] == '0' && (maze[i - 1][j] == ' ' || maze[i + 1][j] == ' '))
				return (printf("Maze is not closed\n"), 0);
			if(!validate_player_pos(maze[i][j], j, i, map))
				return(0);
			j++;	
		}
		if (j > longest_line)	
			longest_line = j;
		i++;
	}
	map->map_width = longest_line;	
	return (1);	
}

bool valid_map(t_user_map *map)
{
	if (map->NO_texture == NULL || map->SO_texture == NULL 
	|| map->WE_texture == NULL || map->EA_texture == NULL
	|| map->ceiling == NULL || map->floor == NULL
	|| map->map_data == NULL)
		return (printf("Error: Invalid map\n"), 0);
	// if (!validate_texture_path(map->NO_texture))
	// 	return (0);		
	// if (!validate_texture_path(map->SO_texture))
	// 	return (0);		
	// if (!validate_texture_path(map->WE_texture))
	// 	return (0);		
	// if (!validate_texture_path(map->EA_texture))
	// 	return (0);		
	if (!validate_color_values(map->ceiling))
		return (printf("Error: Invalid color\n"),0);
	if (!validate_color_values(map->floor))
		return (printf("Error: Invalid color\n"),0);
	if (!validate_maze(map->map_data, map))
		return (printf("Error: Invalid maze\n"),0);
	return (1);
}

void ft_free_map(t_user_map map)
{
	if (map.map_data != NULL)
		ft_free(map.map_data);
	if (map.NO_texture != NULL)
		free(map.NO_texture);
	if (map.SO_texture != NULL)
		free(map.SO_texture);
	if (map.WE_texture != NULL)
		free(map.WE_texture);
	if (map.EA_texture != NULL)
		free(map.EA_texture);
	if (map.floor != NULL)
		free(map.floor);
	if (map.ceiling != NULL)
		free(map.ceiling);
}

bool parsed_map(char *map_path, t_cub3d *data)
{
	int fd;

	fd = open(map_path, O_RDONLY);
	if (fd == -1)
	{
		perror("Error: Invalid path!");
		return (0);
	}
	if (map_parsing(fd, &data->map))
	{
		if (!valid_map(&data->map))
		{
			// TO_DO:
			ft_free_map(data->map);
			return(0);
		}	
		// printf("test\n");
		printf("map_width = %i", data->map.map_width); 
		printf("map_height = %i", data->map.map_height); 
	}
	else
		return(0);
	return(1);
}
