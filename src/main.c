#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <math.h>
#include "../typedef.h"
#include "../MLX/include/MLX42/MLX42.h"
#define WIDTH 128
#define HEIGHT 128

void	rotate(t_cub3d *data, int unit_degree);
void	put_pixel_box(t_cub3d *data, u_int32_t color);

// Exit the program as failure.
static void ft_error(void)
{
	fprintf(stderr, "%s", mlx_strerror(mlx_errno));
	exit(EXIT_FAILURE);
}

// Print the window width and height.
static void ft_hook(void* param)
{
	t_cub3d	*data = param;
	// const mlx_t* mlx = param;

	printf("x: %d | y: %d | dx: %f | dy: %f\n", data->pos.x, data->pos.y, data->pos.dx, data->pos.dy);
	// mlx_put_pixel(data->img, data->pos.x, data->pos.y, 0xFF0000FF);
}

void move_forward(t_cub3d *data, int dir)
{
	//speed is 4
	if (dir == 1)
	{
		data->pos.y += data->pos.dy * 4;
		data->pos.x += data->pos.dx * 4;
	}
	else if (dir == -1)
	{
		data->pos.y -= data->pos.dy * 4;
		data->pos.x -= data->pos.dx * 4;
	}
}

void my_keyhook(mlx_key_data_t keydata, void* param)
{
	t_cub3d	*data = param;
	mlx_t* mlx = data->mlx;
	(void) keydata;

	if (mlx_is_key_down(mlx, MLX_KEY_ESCAPE))
		mlx_close_window(mlx);
	if (mlx_is_key_down(mlx, MLX_KEY_W))
	{
		put_pixel_box(data, 0xFFFFFFFF);
		move_forward(data, 1);
	}
	if (mlx_is_key_down(mlx, MLX_KEY_S))
	{
		put_pixel_box(data, 0xFFFFFFFF);
		move_forward(data, -1);
	}
	if (mlx_is_key_down(mlx, MLX_KEY_A))
	{
		put_pixel_box(data, 0xFFFFFFFF);
		rotate(data, 5);
	}
	if (mlx_is_key_down(mlx, MLX_KEY_D))
	{
		put_pixel_box(data, 0xFFFFFFFF);
		rotate(data, -5);
	}
	// 	if (x, y in range)
	put_pixel_box(data, 0xFF0000FF);
}


void rotate(t_cub3d *data, int unit_degree)
{
	data->pos.angle += unit_degree;
	if (359 < data->pos.angle)
		data->pos.angle -= 360;
	if (data->pos.angle < 0)
		data->pos.angle += 360;
	data->pos.dx = cos(data->pos.angle * M_PI / 180);
	data->pos.dy = -sin(data->pos.angle * M_PI / 180);
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

void render_map(int *map, int map_width, int map_height, int img_width, int img_height, t_cub3d	*data) {
	int scale_factor_x = img_width / map_width;
	int scale_factor_y = img_height / map_height;

	for (int i = 0; i < map_height; i++) {
		for (int j = 0; j < map_width; j++) {
			int color = (map[i * map_width + j] == 1) ? 0xFF0000FF : 0xFFFFFFFF; // Wall or space
			int x_start = j * scale_factor_x + 1;
			int y_start = i * scale_factor_y + 1;
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

int32_t	main(void)
{
	t_cub3d	data;
	// int	map_width = 8;
	int	map_input[] = {
		1,1,1,1,1,1,1,1,
		1,0,1,0,0,0,0,1,
		1,0,1,0,0,0,0,1,
		1,0,1,0,0,0,0,1,
		1,0,0,0,0,0,0,1,
		1,0,0,0,0,1,0,1,
		1,0,0,0,0,0,0,1,
		1,1,1,1,1,1,1,1
	};

	data.pos.x = 700;
	data.pos.y = 700;
	data.pos.dx = 0;
	data.pos.dy = -1;
	data.pos.angle = 0;

	// MLX allows you to define its core behaviour before startup.
	mlx_set_setting(MLX_MAXIMIZED, true);
	data.mlx = mlx_init(WIDTH, HEIGHT, "42Balls", true);
	if (!data.mlx)
		ft_error();

	/* Do stuff */
	// Create and display the image.
	data.img = mlx_new_image(data.mlx, 1395, 1395);
	if (!data.img || (mlx_image_to_window(data.mlx, data.img, 0, 0) < 0))
		ft_error();

	// Even after the image is being displayed, we can still modify the buffer.
	render_map(map_input, 8, 8, 1395, 1395, &data);
	mlx_put_pixel(data.img, data.pos.x, data.pos.y, 0xFF0000FF);

	// Register a hook and pass mlx as an optional param.
	// NOTE: Do this before calling mlx_loop!
	mlx_key_hook(data.mlx, &my_keyhook, &data);
	mlx_loop_hook(data.mlx, ft_hook, &data);
	mlx_loop(data.mlx);
	mlx_terminate(data.mlx);
	return (EXIT_SUCCESS);
}

// void render_map(int *map, int map_width, int map_height, int img_width, int img_height, t_cub3d	*data) {
// 	int scale_factor_x = img_width / map_width;
// 	int scale_factor_y = img_height / map_height;

// 	for (int i = 0; i < map_height; i++) {
// 		for (int j = 0; j < map_width; j++) {
// 			int color;
// 			int x_start = j * scale_factor_x;
// 			int y_start = i * scale_factor_y;
// 			int x_end = x_start + scale_factor_x;
// 			int y_end = y_start + scale_factor_y;

// 			if (map[i * map_width + j] == 1) {
// 				// Wall: Fill entire cell
// 				color = 0xFF0000FF; // Red
// 				for (int y = y_start; y < y_end; y++) {
// 					for (int x = x_start; x < x_end; x++) {
// 						mlx_put_pixel(data->img, x, y, color);
// 					}
// 				}
// 			} else if (map[i * map_width + j] == 2) {
// 				// Player: Smaller size
// 				color = 0xFFFF00FF; // Yellow
// 				int player_padding_x = scale_factor_x / 8; // 1/4th padding on each side
// 				int player_padding_y = scale_factor_y / 8; // 1/4th padding on each side
// 				int px_start = x_start + player_padding_x;
// 				int py_start = y_start + player_padding_y;
// 				int px_end = x_end - player_padding_x;
// 				int py_end = y_end - player_padding_y;

// 				for (int y = py_start; y < py_end; y++) {
// 					for (int x = px_start; x < px_end; x++) {
// 						mlx_put_pixel(data->img, x, y, color);
// 					}
// 				}
// 				mlx_put_pixel(data->img, data->pos.x, data->pos.y, 0xFF0000FF);
// 			} else {
// 				// Space: Leave empty or set background
// 				color = 0xFFFFFFFF; // White
// 				for (int y = y_start; y < y_end; y++) {
// 					for (int x = x_start; x < x_end; x++) {
// 						mlx_put_pixel(data->img, x, y, color);
// 					}
// 				}
// 			}
// 		}
// 	}
// }

// void rotate(float angle, t_cub3d *data)
// {
// 	float old_dx = data->pos.dx;
// 	float old_dy = data->pos.dy;

// 	data->pos.dx = (old_dx * cos(angle) - old_dy * sin(angle));
// 	data->pos.dy = (old_dx * sin(angle) + old_dy * cos(angle));
// }