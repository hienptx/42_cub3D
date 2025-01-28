/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dongjle2 <dongjle2@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/27 22:22:22 by dongjle2          #+#    #+#             */
/*   Updated: 2025/01/28 03:30:50 by dongjle2         ###   ########.fr       */
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

void cast_ray(void *param)
{
    t_cub3d *data = param;
    float angle = data->pos.angle;
    float px = data->pos.x;
    float py = data->pos.y;
    int num_rays = 912;
    float ray_angle = FOV / num_rays; // Step size between rays in radians
    float start_angle = angle + (FOV / 2.0); // Start from leftmost ray in radians
	float disT = 0;
	int color = 0;
	float ra = start_angle;
	// printf("%f\n", ra);
    int i = 0;
    render_map(data->map.map_data, data);
    while (i < num_rays) {
		float vx, vy, rx, ry;
		float ca = adjust_angle(ra - angle); // Angle difference in radians

		float disV = check_vertical_intersection(data, ra, px, py, &vx, &vy);
		float disH = check_horizontal_intersection(data, ra, px, py, &rx, &ry);

		// Determine closest intersection
		if (disV < disH) {
            rx = vx;
            ry = vy;
            disT = disV;
            color = 1;
        } else {
            disT = disH;
            color = 0;
        }

		float corrected_dist = disT * cos(ca); // Fisheye correction

		// Draw wall slice first, then ray line
		draw_wall_slice(data, i, corrected_dist, color, rx, ry);
		draw_line(data, px, py, ra, disT, 0x00FF00FF);
		
		ra = adjust_angle(ra - ray_angle); // Increment angle in radians
		i++;
	}
}

void draw_wall_slice(t_cub3d *data, int x, double distance_to_wall, int color, float rx, float ry)
{
    // Updated to use color to select texture index if needed
    int TEXTURE_HEIGHT = data->texture[2]->height;
    int TEXTURE_WIDTH = data->texture[2]->width;

    int wall_height = (int)(10 * HEIGHT / distance_to_wall); // Adjust wall height calculation
    if (wall_height > HEIGHT) wall_height = HEIGHT;
    double step = 1.0 * (double)TEXTURE_HEIGHT / (double)wall_height;

    int line_top = (HEIGHT - wall_height) / 2;
    int line_bottom = line_top + wall_height;
    double texPos = 0;

    if (line_bottom >= HEIGHT) line_bottom = HEIGHT - 1;

	float relative_pos;
    if (color == 0) { // Horizontal wall
        relative_pos = fmod(rx, cell_size);
    } else { // Vertical wall
        relative_pos = fmod(ry, cell_size);
    }
    // Map the relative position to texture X coordinate
    int texX = (int)((relative_pos / cell_size) * TEXTURE_WIDTH);
    texX %= TEXTURE_WIDTH; // Ensure texX wraps around if necessary

    // Ensure texX is positive
    if (texX < 0) texX += TEXTURE_WIDTH;
    if (color == 0) { // Horizontal wall
        texX = (int)(rx * TEXTURE_WIDTH / cell_size) % TEXTURE_WIDTH;
    } else { // Vertical wall
        texX = (int)(ry * TEXTURE_WIDTH / cell_size) % TEXTURE_WIDTH;
    }
    // Ensure texX is positive
    if (texX < 0) texX += TEXTURE_WIDTH;

    for (int y = line_top; y <= line_bottom; y++) {
		int texY = (int)texPos % TEXTURE_HEIGHT; // Wrap texY within texture height
        // int texY = (int)texPos;
		texPos += step;

		// Clamp texY to texture height
        if (texY >= TEXTURE_HEIGHT) texY = TEXTURE_HEIGHT - 1;
        if (texY < 0) texY = 0;
        uint8_t *pixel = &data->texture[2]->pixels[(TEXTURE_WIDTH * texY + texX) * 4];
        uint32_t pixel_color = (pixel[0] << 24) | (pixel[1] << 16) | 
                                (pixel[2] << 8) | pixel[3];

        if (color == 0) {
            uint32_t r = ((pixel_color >> 24) & 0xFF);
            uint32_t g = ((pixel_color >> 16) & 0xFF);
            uint32_t b = ((pixel_color >> 8) & 0xFF);
            pixel_color = (r << 24) | (g << 16) | (b << 8) | 0xFF;
        }

        mlx_put_pixel(data->img2, x, y, pixel_color);
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
			// printf("Width = %i, Height = %i\n", data.map.map_width, data.map.map_height);
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
