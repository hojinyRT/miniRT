#ifndef MINIRT_H
# define MINIRT_H

# include <stdio.h>
# include <math.h>
# include "libft/libft.h"
# include "mlx/mlx.h"

# define EVENT_KEY_PRESS 2
# define EVENT_MOUSE_CLICK 4
# define EVENT_CLICK_EXIT 17
# define KEY_ESC 53
# define KEY_UP 126
# define KEY_DOWN 125
# define KEY_LEFT 123
# define KEY_RIGHT 124
# define KEY_A 0
# define KEY_S 1
# define KEY_D 2
# define KEY_Q 12
# define KEY_W 13
# define KEY_E 14
# define KEY_R 15
# define KEY_I 34
# define KEY_TAB 48

# define TRUE 1
# define FALSE 0

# define WIN_H 1000
# define WIN_W 2000

# define MLX_H 1000
# define MLX_W 2000

# define SP 0
# define LIGHT_POINT 1

# define EPSILON 1e-6

typedef int t_object_type;

typedef struct  s_img
{
	void	*ptr;
	int		*addr;
	int		bits_per_pixel;
	int		line_length;
	int		endian;
}			t_img;

typedef struct	s_info
{
	void	*mlx;
	void	*win;
}			t_info;

typedef struct s_vec
{
	float	x;
	float	y;
	float	z;
}		    t_vec;

typedef t_vec	t_color;
typedef t_vec	t_point;

typedef struct s_ray
{
	t_point	orig;
	t_vec	dir;
}			t_ray;

typedef struct s_camera
{
	t_point	orig;
	float	viewport_h;
	float	viewport_w;
	t_vec	horizontal;
	t_vec	vertical;
	float	focal_len;
	t_point	start_point;
}			    t_camera;

typedef struct s_canvas
{
	int		width;
	int		height;
	float	aspect_ratio;
}			t_canvas;

typedef struct s_sphere
{
	t_point	center;
	float	radius;
	float	radius2;
}			t_sphere;

typedef struct  s_object
{
    t_object_type   type;
    void            *element;
    void            *next;
	t_vec			albedo;
}                   t_object;


typedef struct s_hit_record
{
    t_point		p;
    t_vec		normal;
    float		tmin;
    float		tmax;
    float		t;
    int			front_face;
	t_vec		albedo;
}				t_hit_record;

typedef struct  s_light
{
    t_vec	origin;
    t_vec	light_color;
    float	bright_ratio;
}			t_light;

typedef struct s_scene
{
    t_canvas		canvas;
    t_camera		camera;
    t_object		*obj;
    t_object		*light;
    t_color			ambient; // 8.4에서 설명할 요소
    t_ray			ray;
    t_hit_record	rec;
}					t_scene;



t_vec	vec_add(t_vec u, t_vec v);
t_vec	vec_sub(t_vec u, t_vec v);
t_vec	vec_multi(t_vec u, t_vec v);
t_vec	vec_div(t_vec u, t_vec v);
t_vec	vec_multi_float(t_vec u, float n);
t_vec	vec_div_float(t_vec u, float n);

float	vec_dot(t_vec u, t_vec v);
t_vec	vec_cross(t_vec u, t_vec v);

float	vec_len(t_vec u);
float	vec_len_sqr(t_vec u);
t_vec	vec_unit(t_vec u);
t_vec	vec_init(float x, float y, float z);

//------material.c-------//
t_ray		ray_init(t_point orig, t_vec dir);
t_point		ray_at(t_ray ray, float t);
t_ray		ray_primary(t_camera cam, double u, double v);
t_color		ray_color(t_scene *scene);
t_canvas	canvas_init(int  width, int height);
t_camera	camera_init(t_canvas canvas, t_point orig);
t_sphere	*sphere_init(t_point center, float radius);
void		set_face_normal(t_ray ray, t_hit_record *rec);


// ---------object.c---------//
t_object    *object_init(t_object_type type, void *element, t_vec albedo);
int			hit(t_object *obj, t_ray ray, t_hit_record *rec);
int			hit_obj(t_object *obj, t_ray ray, t_hit_record *rec);
int			hit_sphere(t_object *obj, t_ray ray, t_hit_record *rec);
void		obj_add(t_object **list, t_object *new);
t_object	*obj_last(t_object *list);


// ---------light.c---------//
t_light     *light_point(t_vec light_origin, t_vec light_color, float bright_ratio);
t_vec		phong_lighting(t_scene *scene);


#endif
