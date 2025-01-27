#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <math.h>
#include "../includes/cub3D.h"
#include "../MLX/include/MLX42/MLX42.h"
#include "../MLX/include/MLX42/MLX42_Int.h"
#include <assert.h>

#define DEG2RAD(angle_in_degrees) ((angle_in_degrees) * M_PI / 180.0)
#define RAD2DEG(angle_in_radians) ((angle_in_radians) * 180.0 / M_PI)
#define MOVING_SPEED 4
#define M_PI_16 M_PI_4 / 4.0
// #define cell_size 32

void	rotate(t_cub3d *data, float unit_degree);
void	put_pixel_box(t_cub3d *data, u_int32_t color);
void	cast_ray(void *param);
void draw_wall_slice(t_cub3d *data, int x, double distance_to_wall, int color, float rx, float ry);

// void map_initialising(t_user_map *map)
// {
// 	map->map_width = 0;
// 	map->map_height = 0;
// 	map->pw = 0;
// 	map->ph = 0;
// 	map->map_data = NULL;
// 	map->NO_texture = NULL;
// 	map->SO_texture = NULL;
// 	map->WE_texture = NULL;
// 	map->NO_texture = NULL;
// 	map->ceiling = NULL;
// 	map->floor = NULL;
// 	map->player_count = 0;
// }

// Exit the program as failure.
static void ft_error(void)
{
	fprintf(stderr, "%s", mlx_strerror(mlx_errno));
	exit(EXIT_FAILURE);
}

int wall_collision(t_cub3d *data, float new_x, float new_y)
{
    // Define an offset value to detect collisions beforehand
    float offset = 2;

    // Calculate the new positions with offset in the direction of movement
    float check_x1 = new_x + data->pos.dx * offset;
    float check_y1 = new_y + data->pos.dy * offset;
    float check_x2 = new_x - data->pos.dx * offset;
    float check_y2 = new_y - data->pos.dy * offset;

    // Calculate the map grid positions
    int ipx1 = check_x1 / data->map.pw;
    int ipy1 = check_y1 / data->map.ph;
    int ipx2 = check_x2 / data->map.pw;
    int ipy2 = check_y2 / data->map.ph;

    // Check for collision in the new positions with offset
    if (data->map.map_data[ipy1][ipx1] == '1' ||
        data->map.map_data[ipy2][ipx1] == '1' ||
        data->map.map_data[ipy1][ipx2] == '1' ||
        data->map.map_data[ipy2][ipx2] == '1')
    {
        return 1; // Collision detected
    }

    return 0; // No collision
}

void move_forward(t_cub3d *data, int dir)
{
    // Calculate the new position
    float new_x = data->pos.x + dir * data->pos.dx * MOVING_SPEED;
    float new_y = data->pos.y + dir * data->pos.dy * MOVING_SPEED;

    // Check for collisions
    if (!wall_collision(data, new_x, new_y))
    {
        // Update the position if no collision
        data->pos.x = new_x;
        data->pos.y = new_y;
    }
}

void move_left_right(t_cub3d *data, int dir)
{
    // Calculate the perpendicular direction
    float perp_dx = -data->pos.dy;
    float perp_dy = data->pos.dx;

    // Normalize the perpendicular direction
    float length = sqrt(perp_dx * perp_dx + perp_dy * perp_dy);
    perp_dx /= length;
    perp_dy /= length;

    // Calculate the new position
    float new_x = data->pos.x + dir * perp_dx * MOVING_SPEED;
    float new_y = data->pos.y + dir * perp_dy * MOVING_SPEED;

    // Check for collisions
    if (!wall_collision(data, new_x, new_y))
    {
        // Update the position if no collision
        data->pos.x = new_x;
        data->pos.y = new_y;
    }
}

void handle_key_action(t_cub3d *data, int *key_pressed)
{
    memset(data->img2->pixels, 255, data->img2->width * data->img2->height * sizeof(int32_t));
    put_pixel_box(data, 0xFFFFFFFF);
    *key_pressed = true;
}

void handle_key_press(t_cub3d *data, int dir, void (*move_func)(t_cub3d *, int), int *key_pressed)
{
    move_func(data, dir);
    handle_key_action(data, key_pressed);
}

void handle_rotation(t_cub3d *data, float angle, int *key_pressed)
{
    rotate(data, angle);
    handle_key_action(data, key_pressed);
}

void handle_movement_keys(t_cub3d *data)
{
    mlx_t* mlx = data->mlx;
    int key_pressed = false;

    if (mlx_is_key_down(mlx, MLX_KEY_W))
        handle_key_press(data, 1, move_forward, &key_pressed);
    if (mlx_is_key_down(mlx, MLX_KEY_S))
        handle_key_press(data, -1, move_forward, &key_pressed);
    if (mlx_is_key_down(mlx, MLX_KEY_A))
        handle_key_press(data, -1, move_left_right, &key_pressed);
    if (mlx_is_key_down(mlx, MLX_KEY_D))
        handle_key_press(data, 1, move_left_right, &key_pressed);
    if (mlx_is_key_down(mlx, MLX_KEY_LEFT))
        handle_rotation(data, M_PI_16, &key_pressed);
    if (mlx_is_key_down(mlx, MLX_KEY_RIGHT))
        handle_rotation(data, -M_PI_16, &key_pressed);

    if (key_pressed)
    {
        printf("x = %f, y = %f\n", data->pos.dx, data->pos.dy);
        cast_ray(data);
    }
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
float adjust_angle(float angle) {
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


void draw_line(t_cub3d *data, float start_x, float start_y, float angle, float length, int color) {
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


void cast_ray(void *param) {
    t_cub3d *data = param;
    float angle = data->pos.angle;
    float FOV = M_PI / 3.0;
    // float FOV = DEG2RAD(60.0); // Convert FOV to radians
    float px = data->pos.x;
    float py = data->pos.y;
    int num_rays = 912;
    float ray_angle = FOV / num_rays; // Step size between rays in radians
    float start_angle = angle + (FOV / 2.0); // Start from leftmost ray in radians
    float rx, ry, xo, yo, disV, disH, disT;
    float dof, mx, my;
    int color;

    float ra = start_angle;
	printf("%f\n", ra);
    int i = 0;
    render_map(data->map.map_data, data);
    while (i < num_rays) {
        disV = 1000000;
        disH = 1000000;
        float ca = adjust_angle(ra - angle); // Angle difference in radians

        // Vertical
        dof = 0;
        float tan_ra = tan(ra);
        if (cos(ra) > 0.001) {
            rx = floor(px / cell_size) * cell_size + cell_size;
            ry = (px - rx) * tan_ra + py;
            xo = cell_size;
            yo = -xo * tan_ra;
        } else if (cos(ra) < -0.001) {
            rx = floor(px / cell_size) * cell_size - 0.01;
            ry = (px - rx) * tan_ra + py;
            xo = -cell_size;
            yo = -xo * tan_ra;
        } else {
            rx = px;
            ry = py;
            dof = 8;
        }

        while (dof < 8) {
            mx = floor(rx / cell_size);
            my = floor(ry / cell_size);
            if (mx >= 0 && my >= 0 && mx < data->map.map_width && my < data->map.map_height && 
                data->map.map_data[(int)my][(int)mx] == '1') {
                dof = 8;
                disV = sqrt(pow(rx - px, 2) + pow(ry - py, 2));
            } else {
                rx += xo;
                ry += yo;
                dof += 1;
            }
        }
        float vx = rx, vy = ry;

        // Horizontal
        dof = 0;
        tan_ra = 1.0f / tan_ra;
        if (sin(ra) > 0.001) {
            ry = floor(py / cell_size) * cell_size - 0.01;
            rx = (py - ry) * tan_ra + px;
            yo = -cell_size;
            xo = -yo * tan_ra;
        } else if (sin(ra) < -0.001) {
            ry = floor(py / cell_size) * cell_size + cell_size;
            rx = (py - ry) * tan_ra + px;
            yo = cell_size;
            xo = -yo * tan_ra;
        } else {
            rx = px;
            ry = py;
            dof = 8;
        }

        while (dof < 8) {
            mx = floor(rx / cell_size);
            my = floor(ry / cell_size);
            if (mx >= 0 && my >= 0 && mx < data->map.map_width && my < data->map.map_height && 
                data->map.map_data[(int)my][(int)mx] == '1') {
                dof = 8;
                disH = sqrt(pow(rx - px, 2) + pow(ry - py, 2));
            } else {
                rx += xo;
                ry += yo;
                dof += 1;
            }
        }

        // Use closest intersection
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
    // Select correct texture based on wall orientation
    // Updated to use color to select texture index if needed
    int TEXTURE_HEIGHT = data->texture[2]->height;
    int TEXTURE_WIDTH = data->texture[2]->width;

    int wall_height = (int)(5 * HEIGHT / distance_to_wall); // Adjust wall height calculation
    if (wall_height > HEIGHT) wall_height = HEIGHT;

    int line_top = (HEIGHT - wall_height) / 2;
    int line_bottom = line_top + wall_height;
    double step = 1.0 * (double)TEXTURE_HEIGHT / (double)wall_height;
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
        // texPos += step;

            // printf("x = %d, rx = %f, ry = %f, texX = %d, texY = %d, pixel_color = %u\n", 
            //        x, rx, ry, texX, texY, pixel_color);
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
