/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hipham <hipham@student.42heilbronn.de>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/27 22:22:22 by dongjle2          #+#    #+#             */
/*   Updated: 2025/02/04 19:02:02 by hipham           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/cub3D.h"
#include "../MLX/include/MLX42/MLX42_Int.h"

#define DEG2RAD(angle_in_degrees) ((angle_in_degrees) * M_PI / 180.0)
#define RAD2DEG(angle_in_radians) ((angle_in_radians) * 180.0 / M_PI)


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
	data->pos.angle = atan2(-data->pos.dy, data->pos.dx);
	data->pos.angle = adjust_angle(data->pos.angle);
	data->pos.angle += unit_degree;
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
	int px;
	int py;
	
	i = 0;
	while (i < sizeof(x_margin) / sizeof(int))
	{
		j = 0 ;
		while (j < sizeof(y_margin) / sizeof(int))
		{
			px = data->pos.x + x_margin[i];
			py = data->pos.y + y_margin[j];
			if (px >= 0 && px < (int)data->img->width && py >= 0 && py < (int)data->img->height)
				mlx_put_pixel(data->img, px, py, color);
			// mlx_put_pixel(data->img, data->pos.x + x_margin[i], data->pos.y + y_margin[j], color);
			j++;
		}
		i++;
	}
	dir = 1;
	while (dir < 13)
	{
		if (px >= 0 && px < (int)data->img->width && py >= 0 && py < (int)data->img->height)
			mlx_put_pixel(data->img, px, py, color);
		// mlx_put_pixel(data->img, data->pos.x + data->pos.dx * dir, data->pos.y + data->pos.dy * dir, color);
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
			int x_start = j * scale_factor_y;
			int y_start = i * scale_factor_x;
			int x_end = x_start + scale_factor_x;
			int y_end = y_start + scale_factor_y;

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
	draw_wall_slice(data, i, corrected_dist, ray);
	draw_line(data, ray->px, ray->py, ra, ray->distance, 0x00FF00FF);
}

void cast_ray(void *param)
{
	t_cub3d			*data = param;
	t_ray_data ray = {.angle_step = FOV / NUM_RAYS, .px = data->pos.x, .py = data->pos.y};;
	float			start_angle;
	unsigned int	i;
	float			ra;

	render_map(data->map.map_data, data);
	start_angle = data->pos.angle + (FOV / 2.0); // Start from leftmost ray in radians
	i = 0;
	while (i < NUM_RAYS)
	{
		ra = start_angle - (i * ray.angle_step);
		calculate_ray_intersection(data, ra, &ray);
		render_single_ray(data, &ray, i, ra);
		i++;
	}
	put_weapon(data);
}

int32_t	main(int ac, char *av[])
{
	char	*path;
	t_cub3d	data;

	if (ac != 2)
	{
		error_sms(ERR_MSG_INVALID_ARG);
	}
	path = av[1];
	map_initialising(&data.map);
	if (parsed_map(path, &data) == 0)
	{
		error_sms(ERR_MSG_INVALID_MAP);
	}
	// MLX allows you to define its core behaviour before startup.
	// mlx_set_setting(MLX_MAXIMIZED, true);
	data.mlx = mlx_init(WIDTH, HEIGHT, "42Balls", true);
	if (!data.mlx)
	{
		ft_error();
	}
	/* Do stuff */
	// Create and display the image.
	cub3d_initialising(&data);
	load_png_texture(&data);
	reset_trigger(&data);
	data.img = mlx_new_image(data.mlx, data.iwidth, data.iheight);
	if (!data.img || (mlx_image_to_window(data.mlx, data.img, 0, 0) < 0))
		ft_error();
	data.img2 = mlx_new_image(data.mlx, SCREEN_WIDTH, SCREEN_HEIGHT);
	if (!data.img2 || (mlx_image_to_window(data.mlx, data.img2, 0, 0) < 0))
		ft_error();
	// Even after the image is being displayed, we can still modify the buffer.
	render_map(data.map.map_data, &data);
	put_pixel_box(&data, 0xF000000F);
	// Register a hook and pass mlx as an optional param.
	// NOTE: Do this before calling mlx_loop!
	// mlx_loop_hook(data.mlx, ft_hook, &data);
	mlx_key_hook(data.mlx, &my_keyhook, &data);
	mlx_loop_hook(data.mlx, update_game_state, &data);
	mlx_loop(data.mlx);
	mlx_delete_image(data.mlx, data.img);
	mlx_delete_image(data.mlx, data.img2);
	ft_delete_texture(&data);
	mlx_terminate(data.mlx);
	ft_free_map(data.map);
}
