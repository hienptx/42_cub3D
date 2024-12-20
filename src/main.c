#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <math.h>
#include "../typedef.h"
#include "../MLX/include/MLX42/MLX42.h"
#define WIDTH 128
#define HEIGHT 128

void	rotate(float angle, t_cub3d *data);
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
		put_pixel_box(data, 0xFF000000);
		move_forward(data, 1);
	}
	if (mlx_is_key_down(mlx, MLX_KEY_S))
	{
		put_pixel_box(data, 0xFF000000);
		move_forward(data, -1);
	}
	if (mlx_is_key_down(mlx, MLX_KEY_A))
	{
		put_pixel_box(data, 0xFF000000);
		rotate(-0.1, data);
	}
	if (mlx_is_key_down(mlx, MLX_KEY_D))
	{
		put_pixel_box(data, 0xFF000000);
		rotate(0.1, data);
	}
	// 	if (x, y in range)
	put_pixel_box(data, 0xFF0000FF);
}

void rotate(float angle, t_cub3d *data)
{
	float old_dx = data->pos.dx;
	float old_dy = data->pos.dy;

	data->pos.dx = (old_dx * cos(angle) - old_dy * sin(angle));
	data->pos.dy = (old_dx * sin(angle) + old_dy * cos(angle));
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

int32_t	main(void)
{
	t_cub3d	data;

	data.pos.x = 100;
	data.pos.y = 100;
	data.pos.dx = 0;
	data.pos.dy = -1;
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
	put_pixel_box(&data, 0xFF0000FF);
	mlx_put_pixel(data.img, data.pos.x, data.pos.y, 0xFF0000FF);

	// Register a hook and pass mlx as an optional param.
	// NOTE: Do this before calling mlx_loop!
	mlx_key_hook(data.mlx, &my_keyhook, &data);
	mlx_loop_hook(data.mlx, ft_hook, &data);
	mlx_loop(data.mlx);
	mlx_terminate(data.mlx);
	return (EXIT_SUCCESS);
}