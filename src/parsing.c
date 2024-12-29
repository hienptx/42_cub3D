/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hipham <hipham@student.42heilbronn.de>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/26 21:46:15 by hipham            #+#    #+#             */
/*   Updated: 2024/12/29 19:34:44 by hipham           ###   ########.fr       */
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

void parse_color(char *line, t_user_map map)
{
	char **color;
	int color_arr[3];
	int i;
	char c;
	
	c = line[0];
	i = 0;
	line++;
	color = ft_split(line, ',');
	while (color[i])
	{
		color_arr[i] = ft_atoi(color[i]);
		i++;
	}
	if (c == 'C')
	{
		ft_malloc((void**)&map.ceiling, sizeof(color_arr));
		ft_memcpy(map.ceiling, color_arr, sizeof(color_arr));
	}
	if (c == 'F')
	{
		ft_malloc((void**)&map.floor, sizeof(color_arr));
		ft_memcpy(map.floor, color_arr, sizeof(color_arr));
	}
	ft_free(color);
}

void save_texture(char **dir, char *path)
{
	size_t len;
	
	len = ft_strlen(path) + 1;
	ft_malloc((void**)dir, len);
	ft_memcpy(*dir, path, len);
}


void parse_texture(char *line, t_user_map map)
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
			save_texture(&map.NO_texture, arr[1]);
		else if (ft_strcmp(arr[0], "SO") == 0)
			save_texture(&map.SO_texture, arr[1]);
		else if (ft_strcmp(arr[0], "WE") == 0)
			save_texture(&map.WE_texture, arr[1]);
		else if (ft_strcmp(arr[0], "EA") == 0)
			save_texture(&map.EA_texture, arr[1]);
	}
	ft_free(arr);
}



// void parse_maze(char **line, int fd, t_user_map map)
// {
// 	char **maze_arr;
// 	char *tmp;
	
// 	maze_arr = NULL;
// 	while (*line)
// 	{
// 		tmp = *line;
// 		while(*tmp != '\0' && !ft_isprint(*tmp))
// 			tmp++;
// 		if (*tmp == '1')
// 			save_maze(*line, maze_arr);
// 		else
// 		{
// 			if(maze_arr != NULL)
// 				ft_free(maze_arr);
// 			free(*line);
// 			break ;
// 		}
// 		free(*line);
// 		*line = get_next_line(fd);
// 	}
// 	if (maze_arr != NULL)
// 		convert_to_int_data(maze_arr, map);
// }

bool map_parsing(int fd, t_user_map map)
{
	char *line;
	char *tmp;

	line = get_next_line(fd);
	while (line)
	{
		tmp = line;
		while(*tmp != '\0' && !ft_isprint(*tmp))
			tmp++;
		if (*tmp != '\0' && (*tmp == 'F' || *tmp == 'C'))
			parse_color(tmp, map);
		else if (*tmp != '\0' && ft_strchr("NSWE", *tmp))
			parse_texture(tmp, map);
		// else if (*tmp != '\0' && *tmp == '1')
		// {
		// 	parse_maze(&line, fd, map);
		// 	return (1);
		// }
		free(line);
		line = get_next_line(fd);
	}
	return(1);			
}

bool parsed_map(char *map_path, t_user_map map)
{
	// (void)map_path;
	int fd;
	// char *buffer;

	fd = open(map_path, O_RDONLY);
	if (fd == -1)
	{
		perror("Invalid path");
		return (0);
	}
	
	if (map_parsing(fd, map))
		printf("Parse successfully\n");
		// validate_map();
	else
		return(0);
	return(1);
}
