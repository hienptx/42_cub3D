#ifndef CUB3D_H
#define CUB3D_H

#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <math.h>
#include <fcntl.h>
#include "../include/typedef.h"
#include "../MLX/include/MLX42/MLX42.h"
#include "../libft/includes/libft.h"
#include "../libft/includes/get_next_line.h"

#define WIDTH 1280
#define HEIGHT 640
#define SCREEN_WIDTH 912
#define SCREEN_HEIGHT 640
#define cell_size 32
#define NUM_RAYS SCREEN_WIDTH
#define MOVING_SPEED 4
#define FOV M_PI / 3.0

//initialization.c
void map_initialising(t_user_map *map);
void player_initilising(t_player *player);
void cub3d_initialising(t_cub3d *data);
void ft_free_map(t_user_map map);

//validation.c
bool validate_map(t_user_map *map);
bool validate_maze(char **maze, t_user_map *map);
bool validate_player_pos(char c, unsigned int j, unsigned int i, t_user_map *map);
bool validate_color_values(int *color);

//parsing.c
bool parsed_map(char *map_path, t_cub3d *data);


//cub3D_utils.c
int error_sms(char *s, int ret);
void ft_malloc(void **data, size_t size);

//parsing_utils.c
bool check_for_wall(char *line);
bool get_player(char **maze, int i, int *longest_line, t_user_map *map);
void parse_player_dir(int dx, int dy, t_user_map *map);
void convert_to_data(t_user_map *map, char *join);
void save_texture(char **dir, char *path);

//movements_and_rotation.c
void handle_movement_keys(t_cub3d *data);
void my_keyhook(mlx_key_data_t keydata, void* param);
float adjust_angle(float angle);
void rotate(t_cub3d *data, float unit_degree);
void cast_ray(void *param);
void draw_wall_slice(t_cub3d *data, int x, double distance_to_wall, int color, float rx, float ry);
void render_map(char **map, t_cub3d *data);
void draw_line(t_cub3d *data, float start_x, float start_y, float angle, float length, int color);

//main.c
void	rotate(t_cub3d *data, float unit_degree);
void	put_pixel_box(t_cub3d *data, u_int32_t color);
void	cast_ray(void *param);
void	draw_wall_slice(t_cub3d *data, int x, double distance_to_wall, int color, float rx, float ry);

//ray_direction.c
void init_ray_up(t_intersection *inter, t_ray_data *ray);
void init_ray_down(t_intersection *inter, t_ray_data *ray);
void init_ray_right(t_intersection *inter, t_ray_data *ray);
void init_ray_left(t_intersection *inter, t_ray_data *ray);

//get_ray_distance.c
float check_horizontal_intersection(t_cub3d *data, t_ray_data *ray, float *rx, float *ry);
float check_vertical_intersection(t_cub3d *data, t_ray_data *ray, float *vx, float *vy);

#endif