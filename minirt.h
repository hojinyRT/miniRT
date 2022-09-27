#ifndef MINIRT_H
# define MINIRT_H

# include <stdio.h>
# include <limits.h>
# include <math.h>
# include <fcntl.h>
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

# define X 0
# define Y 1
# define U 0
# define V 1

# define IMG_H 900
# define IMG_W 1600

# define WIN_H 900
# define WIN_W 1600

# define A 0
# define C 1
# define L 2
# define SP 3
# define PL 4
# define CY 5

# define RGB 0
# define XYZ 1
# define UNIT 2

# define EPSILON 1e-6
# define LUMEN 3

typedef int t_object_type;

typedef struct  s_img
{
	void	*img_ptr;
	int		*addr;
	int		bits_per_pixel;
	int		line_length;
	int		endian;
}			t_img;

typedef struct	s_mlx
{
	void	*ptr;
	void	*win;
	t_img	img;
}			t_mlx;

typedef struct s_vec
{
	double	x;
	double	y;
	double	z;
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
	double	viewport_h;
	double	viewport_w;
	t_vec	horizontal;
	t_vec	vertical;
	double	focal_len;
	t_point	start_point;
	int		fov;
	t_vec	normal;
}			    t_camera;

typedef struct s_sphere
{
	t_point	center;
	double	radius;
	double	radius2;
}			t_sphere;

typedef struct s_plane
{
	t_point	center;
	t_vec	normal;
}			t_plane;

typedef struct s_cylinder
{
	t_point	center;
	double	radius;
	double	radius2;
	double	height;
	t_vec	normal;
}			t_cylinder;

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
    double		tmin;
    double		tmax;
    double		t;
    int			front_face;
	t_vec		albedo;
}				t_hit_record;

typedef struct  s_light
{
    t_vec	origin;
    t_vec	light_color;
    double	brightness;
	void            *next;
}			t_light;

typedef struct s_info
{
    t_camera		camera;
    t_object		*obj;
    t_light			*light;
    t_color			ambient;
    t_ray			ray;
    t_hit_record	rec;
}					t_info;

t_vec 	vec_min(t_vec vec1, t_vec vec2);
t_vec	vec_add(t_vec u, t_vec v);
t_vec	vec_sub(t_vec u, t_vec v);
t_vec	vec_multi(t_vec u, t_vec v);
t_vec	vec_div(t_vec u, t_vec v);
t_vec	vec_multi_double(t_vec u, double n);
t_vec	vec_div_double(t_vec u, double n);
double	vec_dot(t_vec u, t_vec v);
t_vec	vec_cross(t_vec u, t_vec v);
double	vec_len(t_vec u);
double	vec_len_sqr(t_vec u);
t_vec	vec_unit(t_vec u);
t_vec	vec_init(double x, double y, double z);


t_object    *object_init(t_object_type type, void *element, t_vec albedo);
t_sphere	*sphere_init(t_point center, double radius);
t_plane	*plane_init(t_point center, t_vec normal);
t_cylinder	*cylinder_init(t_point center, double radius, double height, t_vec normal);
t_light     *light_init(t_vec light_origin, t_vec light_color, double brightness);


// ---------utils.c---------//
void	is_sign(char *str, int *idx, double *sign);
double	ft_atod(char *str);
void	check_unit(double *x, double *y, double *z, int flag);
t_vec	ft_atovec(char *str, int flag);
void	ft_strerror(char *err);
void	split_free(char **split);


// ---------object.c---------//
t_hit_record	record_init(void);
int				hit_sphere(t_object *obj, t_ray ray, t_hit_record *rec);
int				hit_obj(t_object *obj, t_ray ray, t_hit_record *rec);
int				hit(t_object *obj, t_ray ray, t_hit_record *rec);
t_vec			point_light_get(t_info *info, t_light *light);
t_vec			phong_lighting(t_info info);
t_point			ray_at(t_ray ray, double t);


#endif
