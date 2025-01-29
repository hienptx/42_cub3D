/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dongjle2 <dongjle2@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/27 22:22:22 by dongjle2          #+#    #+#             */
/*   Updated: 2025/01/30 00:49:01 by dongjle2         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/cub3D.h"
#include "../MLX/include/MLX42/MLX42_Int.h"

#define DEG2RAD(angle_in_degrees) ((angle_in_degrees) * M_PI / 180.0)
#define RAD2DEG(angle_in_radians) ((angle_in_radians) * 180.0 / M_PI)

// Exit the program as failure.
static void ft_error(void)
{
	fprintf(stderr, "%s", mlx_strerror(mlx_errno));
	exit(EXIT_FAILURE);
}

void my_keyhook(mlx_key_data_t keydata, void* param)
{
	t_cub3d *data = param;
	mlx_t* mlx = data->mlx;
	(void) keydata;

	if (mlx_is_key_down(mlx, MLX_KEY_ESCAPE))
	{
		mlx_close_window(mlx);
	}
	else
	{
		handle_movement_keys(data);
	}
}

// Adjust angle to be within [0, 2*PI)
float adjust_angle(float angle)
{
	// angle = fmod(angle, 2.0f * M_PI);
	if (angle < 0)
		angle += 2.0f * M_PI;
	return angle;
}

void rotate(t_cub3d *data, float unit_degree)
{
	data->pos.angle += unit_degree;
	data->pos.angle = adjust_angle(data->pos.angle);
	// printf("angle: %f\n", data->pos.angle);
	data->pos.dx = cos(data->pos.angle);
	data->pos.dy = -sin(data->pos.angle);
}

void	put_pixel_box(t_cub3d *data, u_int32_t color)
{
	const int x_margin[7] = {-3, -2, -1, 0 ,1, 2, 3};
	const int y_margin[7] = {-3, -2, -1, 0 ,1, 2, 3};
	size_t	i;
	size_t	j;
	size_t	dir;

	i = 0;
	while (i < sizeof(x_margin) / sizeof(int))
	{
		j = 0 ;
		while (j < sizeof(y_margin) / sizeof(int))
		{
			mlx_put_pixel(data->img, data->pos.x + x_margin[i], data->pos.y + y_margin[j], color);
			j++;
		}
		i++;
	}
	dir = 1;
	while (dir < 13)
	{
		mlx_put_pixel(data->img, data->pos.x + data->pos.dx * dir, data->pos.y + data->pos.dy * dir, color);
		dir++;
	}
}

void render_map(char **map, t_cub3d *data)
{
	int scale_factor_x = data->img->width / data->map.map_width;
	int scale_factor_y = data->img->height / data->map.map_height;
	int color;

	for (int i = 0; map[i] != NULL; i++) {
		for (int j = 0; map[i][j] != '\0'; j++) {
			if (map[i][j] == '1')
				color = 0xFF0000FF; // Wall: Blue
			else if (map[i][j] == '0' || ft_strchr("NSEW", map[i][j]))
				color = 0xFFFFFFFF; // Space: White
			else
				color = 0x00000000; // Default: Black (or transparent)
			int x_start = j * scale_factor_y + 1;
			int y_start = i * scale_factor_x + 1;
			int x_end = x_start + scale_factor_x - 1;
			int y_end = y_start + scale_factor_y - 1;

			for (int y = y_start; y < y_end; y++) {
				for (int x = x_start; x < x_end; x++) {
					mlx_put_pixel(data->img, x, y, color);
				}
			}
		}
	}
}

void draw_line(t_cub3d *data, float start_x, float start_y, float angle, float length, int color)
{
	float end_x = start_x + length * cos(angle);
	float end_y = start_y + length * -sin(angle);

	int x1 = (int)start_x;
	int y1 = (int)start_y;
	int x2 = (int)end_x;
	int y2 = (int)end_y;

	int dx = abs(x2 - x1);
	int dy = abs(y2 - y1);
	int sx = x1 < x2 ? 1 : -1;
	int sy = y1 < y2 ? 1 : -1;
	int err = (dx > dy ? dx : -dy) / 2;
	int e2;

	while (1) {
		mlx_put_pixel(data->img, x1, y1, color);
		if (x1 == x2 && y1 == y2)
			break;
		e2 = err;
		if (e2 > -dx) {
			err -= dy;
			x1 += sx;
		}
		if (e2 < dy) {
			err += dx;
			y1 += sy;
		}
	}
}

static void calculate_ray_intersection(t_cub3d *data, float ray_angle, t_ray_data *ray)
{
	ray->angle = ray_angle;
	ray->tan_angle = tan(ray_angle);
	float vx, vy, rx, ry;
	float disV = check_vertical_intersection(data, ray, &vx, &vy);
	float disH = check_horizontal_intersection(data, ray, &rx, &ry);

	// Choose closer intersection
	if (disV < disH)
	{
		ray->hit_x = vx;	//why x to y
		ray->hit_y = vy;	//and y to x? ->somehow fixed
		ray->distance = disV;
		ray->color = 1;
	}
	else
	{
		ray->hit_x = rx;
		ray->hit_y = ry;
		ray->distance = disH;
		ray->color = 0;
	}
}

void	render_single_ray(t_cub3d *data, t_ray_data *ray, unsigned int i, float ra)
{
	float ca = adjust_angle(ra - data->pos.angle); // Angle difference in radians
	float corrected_dist = ray->distance * cos(ca); // Fisheye correction
	draw_wall_slice(data, i, corrected_dist, ray->color, ray->hit_x, ray->hit_y);
	draw_line(data, ray->px, ray->py, ra, ray->distance, 0x00FF00FF);
}

void cast_ray(void *param)
{
	t_cub3d			*data = param;
	t_ray_data ray = {.angle_step = FOV / NUM_RAYS, .px = data->pos.x, .py = data->pos.y};
	float			start_angle;
	unsigned int	i;
	float			ra;

	start_angle = data->pos.angle + (FOV / 2.0); // Start from leftmost ray in radians
	render_map(data->map.map_data, data);
	i = 0;
	while (i < NUM_RAYS)
	{
		ra = start_angle - (i * ray.angle_step);
		calculate_ray_intersection(data, ra, &ray);
		render_single_ray(data, &ray, i, ra);
		i++;
	}
}

static uint32_t get_pixel_color(mlx_texture_t *texture, uint32_t tex_x, uint32_t tex_y)
{
	uint8_t *pixel = &texture->pixels[(texture->width * tex_y + tex_x) * 4];
	return (pixel[0] << 24) | (pixel[1] << 16) | (pixel[2] << 8) | pixel[3];
}

static t_wall_data calculate_wall_dimensions(t_cub3d *data, double distance)
{
	t_wall_data wall_data;
	wall_data.height = (int)(10 * HEIGHT / distance); // Adjust wall height calculation
	if (wall_data.height > HEIGHT)
		wall_data.height = HEIGHT;
	wall_data.step = 1.0 * (double)data->texture[2]->height / (double)wall_data.height;
	wall_data.line_top = (HEIGHT - wall_data.height) / 2;
	wall_data.line_bottom = wall_data.line_top + wall_data.height;
	if (wall_data.line_bottom >= HEIGHT)
		wall_data.line_bottom = HEIGHT - 1;
	return (wall_data);
}

static uint32_t get_texture_x(float pos, u_int32_t tex_width)
{
    float relative_pos = fmod(pos, cell_size);
    uint32_t tex_x = (uint32_t)((relative_pos / cell_size) * tex_width);
    tex_x %= tex_width;
    return (tex_x < 0) ? tex_x + tex_width : tex_x;
}

void draw_wall_slice(t_cub3d *data, int x, double distance_to_wall, int is_vertical, float rx, float ry)
{
    mlx_texture_t *texture = data->texture[2];
    if (!texture)
        return;

    t_wall_data wall = calculate_wall_dimensions(data, distance_to_wall);
    wall.texture = texture;
    uint32_t tex_x = get_texture_x(is_vertical ? ry : rx, texture->width);
    double tex_pos = 0;

    for (int y = wall.line_top; y <= wall.line_bottom; y++) 
    {
        uint32_t tex_y = (uint32_t)tex_pos % texture->height;
        tex_y = (tex_y >= texture->height) ? texture->height - 1 : tex_y;

        uint32_t pixel_color = get_pixel_color(texture, tex_x, tex_y);
        if (!is_vertical) {
            uint32_t r = ((pixel_color >> 24) & 0xFF);
            uint32_t g = ((pixel_color >> 16) & 0xFF);
            uint32_t b = ((pixel_color >> 8) & 0xFF);
            pixel_color = (r << 24) | (g << 16) | (b << 8) | 0xFF;
        }
        mlx_put_pixel(data->img2, x, y, pixel_color);
        tex_pos += wall.step;
    }
}

int32_t	main(int ac, char *av[])
{
	char	*path;
	t_cub3d	data;

	if (ac == 2)
	{
		path = av[1];
		map_initialising(&data.map);
		if (parsed_map(path, &data))
		{
			// MLX allows you to define its core behaviour before startup.
			// mlx_set_setting(MLX_MAXIMIZED, true);
			data.mlx = mlx_init(WIDTH, HEIGHT, "42Balls", true);
			if (!data.mlx)
			{
				printf("Error: mlx_init failed\n");
				ft_error();
			}
			/* Do stuff */
			// Create and display the image.
			cub3d_initialising(&data);
			data.texture[2] = mlx_load_png("images/texture2.png");
			data.texture[1] = mlx_load_png(data.map.EA_texture);
			// data.texture[2] = mlx_load_png(data.map.WE_texture);
			data.texture[3] = mlx_load_png(data.map.SO_texture);
			data.img = mlx_new_image(data.mlx, data.iwidth, data.iheight);
			if (!data.img || (mlx_image_to_window(data.mlx, data.img, 0, 0) < 0))
			{
				printf("Error: mlx_new_image failed\n");
				ft_error();
			}
			data.img2 = mlx_new_image(data.mlx, 912, 640);
			if (!data.img2 || (mlx_image_to_window(data.mlx, data.img2, data.map.map_width*cell_size, 0) < 0))
				ft_error();
			// Even after the image is being displayed, we can still modify the buffer.
			render_map(data.map.map_data, &data);
			put_pixel_box(&data, 0xF000000F);
			// Register a hook and pass mlx as an optional param.
			// NOTE: Do this before calling mlx_loop!
			// mlx_loop_hook(data.mlx, ft_hook, &data);
			mlx_key_hook(data.mlx, &my_keyhook, &data);
			mlx_loop(data.mlx);
			mlx_terminate(data.mlx);
			ft_free_map(data.map);
		}
		else
			return (EXIT_SUCCESS);
	}
	else
		printf("Error: Invalid input!\n./cub3D [MAP.cub]\n");
	return (EXIT_SUCCESS);
}
