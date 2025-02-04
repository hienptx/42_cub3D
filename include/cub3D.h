#ifndef CUB3D_H
# define CUB3D_H

# include "../MLX/include/MLX42/MLX42.h"
# include "../include/typedef.h"
# include "../libft/includes/get_next_line.h"
# include "../libft/includes/libft.h"
# include <fcntl.h>
# include <math.h>
# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <time.h>
# include <unistd.h>

# define WIDTH 1280
# define HEIGHT 640
# define SCREEN_WIDTH WIDTH
# define SCREEN_HEIGHT HEIGHT
// # define cell_size 32
# define NUM_RAYS SCREEN_WIDTH
# define MOVING_SPEED 4
# define FOV M_PI / 3.0
# define MAX_MAP_WIDTH 240
# define MAX_MAP_HEIGHT 240
# define ERR_MSG_INVALID_MAP "Error: Invalid map\n"
# define ERR_MSG_INVALID_ARG "Error: Invalid input!\n./cub3D [MAP.cub]\n"

//initialization.c
void		map_initialising(t_user_map *map);
void		cub3d_initialising(t_cub3d *data);
void		ft_free_map(t_user_map map);
void		ft_delete_texture(t_cub3d *data);

//validation.c
bool		validate_map(t_user_map *map);
bool		validate_maze(char **maze, t_user_map *map);
bool		validate_player_pos(char c, unsigned int j, unsigned int i,
				t_user_map *map);
bool		validate_color_values(int *color, size_t size);

//parsing.c
bool		parsed_map(char *map_path, t_cub3d *data);

//cub3D_utils.c
int			error_sms(char *s);
void		ft_malloc(void **data, size_t size);
void		load_png_texture(t_cub3d *data);
void		ft_error(void);
uint32_t	get_pixel_color(mlx_texture_t *texture, uint32_t tex_x,
				uint32_t tex_y);

//parsing_utils.c
bool		check_for_wall(char *line);
bool		get_player(char **maze, int i, size_t *longest_line,
				t_user_map *map);
void		parse_player_dir(int dx, int dy, t_user_map *map);
void		convert_to_data(t_user_map *map, char *join);
void		save_texture(char **dir, char *path);
void		copy_color(t_user_map *map, int *color_arr, size_t size,
				char *line);

//movements_and_rotation.c
void		handle_movement_keys(t_cub3d *data);
void		my_keyhook(mlx_key_data_t keydata, void *param);
float		adjust_angle(float angle);
void		rotate(t_cub3d *data, float unit_degree);
void		cast_ray(void *param);
void		render_map(char **map, t_cub3d *data);
void		draw_line(t_cub3d *data, float start_x, float start_y, float angle,
				float length, int color);

//main.c
void		rotate(t_cub3d *data, float unit_degree);
void		put_pixel_box(t_cub3d *data, u_int32_t color);
void		cast_ray(void *param);
void		draw_wall_slice(t_cub3d *data, int x, double distance_to_wall,
				t_ray_data *ray);
void		render_single_ray(t_cub3d *data, t_ray_data *ray, unsigned int i,
				float ra);

//ray_direction.c
void		init_ray_up(t_intersection *inter, t_ray_data *ray,
				float cell_size);
void		init_ray_down(t_intersection *inter, t_ray_data *ray,
				float cell_size);
void		init_ray_right(t_intersection *inter, t_ray_data *ray,
				float cell_size);
void		init_ray_left(t_intersection *inter, t_ray_data *ray,
				float cell_size);

//get_ray_distance.c
float		check_horizontal_intersection(t_cub3d *data, t_ray_data *ray,
				float *rx, float *ry);
float		check_vertical_intersection(t_cub3d *data, t_ray_data *ray,
				float *vx, float *vy);
// weapon.c
void		put_weapon(t_cub3d *data);
void		update_game_state(void *param);
void		trigger_flash(t_cub3d *data);
void		reset_trigger(t_cub3d *data);
void		put_object(t_cub3d *data, mlx_texture_t *obj, float obj_x,
				float obj_y);

// rendering_utils.c
uint32_t	get_texture_x(float pos, u_int32_t tex_width, float cell_size);
int			get_rgba(int r, int g, int b, int a);

#endif