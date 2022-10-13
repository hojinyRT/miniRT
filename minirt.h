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
# define RADIUS 0
# define HEIGHT 1

# define IMG_H 720
# define IMG_W 1280

# define WIN_H 720
# define WIN_W 1280

enum e_material_type
{
	A,
	C,
	L,
	SP,
	PL,
	CY,
	CN,
	CAP
};

# define RGB 0
# define XYZ 1
# define UNIT 2

# define EPSILON 1e-6
# define LUMEN 3

typedef int			t_object_type;

typedef struct s_formula
{
	double		a;
	double		c;
	double		dis;
	double		half_b;
	double		sqrtd;
	double		root;
}				t_formula;

typedef struct s_vec
{
	double	x;
	double	y;
	double	z;
}			t_vec;

typedef t_vec		t_color;
typedef t_vec		t_point;

typedef struct s_img
{
	void	*img_ptr;
	char	*file_name;
	char	*addr;
	int		bits_per_pixel;
	int		line_length;
	int		endian;
	int		width;
	int		height;
}			t_img;

typedef struct s_mlx
{
	void	*ptr;
	void	*win;
	t_img	img;
}			t_mlx;

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
	double	focal_len;
	t_vec	horizontal;
	t_vec	vertical;
	t_point	start_point;
	t_vec	normal;
	void	*next;
	int		fov;
}			t_camera;

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
	double	radius;
}			t_plane;



typedef struct s_object
{
	t_object_type	type;
	void			*element;
	void			*next;
	t_color			color;
	t_img			*bump;
	t_img			*texture;
	int				checker;
}					t_object;

typedef struct s_cylinder
{
	t_point		center;
	double		radius;
	double		radius2;
	double		height;
	t_vec		normal;
	t_object	*cap;
}				t_cylinder;

typedef t_cylinder	t_cone;

typedef struct s_hit_record
{
	t_point	p;
	t_vec	normal;
	double	tmin;
	double	tmax;
	double	t;
	int		front_face;
	int		type;
	t_vec	color;
	double	u;
	double	v;
	t_vec	e1;
	t_vec	e2;
	int		checker;
}			t_hit_record;

typedef struct s_light
{
	t_vec	origin;
	t_vec	light_color;
	double	brightness;
	void	*next;
}			t_light;

typedef struct s_info
{
	t_mlx			mlx;
	t_img			bump;
	t_camera		*camera;
	t_object		*obj;
	t_light			*light;
	t_color			*ambient;
	t_ray			ray;
	t_hit_record	rec;
}					t_info;

// ---------info_init.c--------////
char		*check_first_file_name(int fd);
void		info_init(t_info *info, char *file);
int			check_format(char *format);
void		put_info(t_info *info, char **argv);

// ---------object_init.c--------//
t_object	*object_init(t_object_type type, void *element, int checker);
t_sphere	*sphere_init(t_point center, double radius);
t_plane		*plane_init(t_point center, t_vec normal, double radius);
t_cylinder	*cylinder_init(t_point center, double radius, \
						double height, t_vec normal);
t_cone		*cone_init(t_point center, double radius, \
						double height, t_vec normal);

// ---------light.c--------//
t_light		*light_init(t_vec light_origin, t_vec light_color, \
						double brightness);
t_vec		reflect(t_vec v, t_vec n);
t_vec		phong_lighting(t_info *info);
t_vec		point_light_get(t_info *info, t_light *light);

// ---------hit.c--------//
void		record_init(t_hit_record *rec);
int			in_shadow(t_object *objs, t_ray light_ray, double light_len);
int			hit_obj(t_object *obj, t_ray ray, t_hit_record *rec);
int			hit(t_object *obj, t_ray ray, t_hit_record *rec);

// ---------cone.c--------////
int			is_valid(t_formula *fo, t_hit_record *rec, \
					t_cylinder *cy, int flag);
int			check_validation(t_formula *fo, t_hit_record rec, \
					t_cylinder *obj, t_ray *ray);
int			hit_cone(t_object *obj, t_ray ray, t_hit_record *rec);

// ---------cylinder.c--------////

void		get_cylinder_uv(t_hit_record *rec, t_cylinder *cy, double size);
int			hit_cylinder(t_object *obj, t_ray ray, t_hit_record *rec);

// ---------plane.c--------////
int			hit_plane(t_object *obj, t_ray ray, t_hit_record *rec);
int			hit_cap(t_object *obj, t_ray ray, t_hit_record *rec, t_object *body);

// ---------scene.c--------////
void		light_add(t_light **list, t_light *new);
t_camera	*camera_init(t_point coor, t_vec normal, int fov);
void		camera_add(t_camera **list, t_camera *new);

// ---------put.c--------////
void		put_sp(t_info *info, char **argv, int cnt);
void		put_pl(t_info *info, char **argv, int cnt);
void		put_cy(t_info *info, char **argv, int cnt);
void		put_cn(t_info *info, char **argv, int cnt);
void		put_cap(t_object **new, t_cylinder *obj, int type);


// ---------put2.c--------////
void		put_c(t_info *info, char **argv, int cnt);
void		put_l(t_info *info, char **argv, int cnt);
void		put_a(t_info *info, char **argv, int cnt);
void		obj_add(t_object **list, t_object *new);


// ---------ray.c--------////
t_ray		ray_init(t_point orig, t_vec dir);
t_point		ray_at(t_ray ray, double t);
void		ray_primary(t_ray *ray, t_camera *cam, double u, double v);
t_color		ray_color(t_info *info);

// ---------sphere.c--------////
int			hit_sphere(t_object *obj, t_ray ray, t_hit_record *rec);

// ---------texture.c--------////
void		get_texture_addr(t_object *obj, t_mlx *mlx);
void		get_bump_addr(t_object *obj, t_mlx *mlx);
void		get_image_addr(t_object *new, t_mlx *mlx, char *file);
t_color		checkerboard_value(t_hit_record rec);
t_vec		bump_normal(t_object *obj, t_hit_record *rec);

// ---------utils.c--------////
void		split_free(char **split);
void		is_sign(char *str, int *idx, double *sign);
double		ft_atod(char *str);
void		check_unit(double *x, double *y, double *z, int flag);
t_vec		ft_atovec(char *str, int flag);

// ---------utils2.c--------////
t_vec		convert_color_to_normal(int color);
int			convert_color(t_vec clr);
void		set_face_normal(t_ray ray, t_hit_record *rec);
t_vec		convert_int_to_rgb(int color);
t_vec		texture_rgb(t_object *obj, t_hit_record *rec);

// ---------utils3.c--------////
void		*my_calloc(size_t count, size_t size);
int			my_atoi(const char *str);
t_point		get_cap_point(t_point center, double height, t_vec normal, \
							double sign);
void		ft_strerror(char *err);


// ---------vector.c--------////
t_vec		vec_init(double x, double y, double z);
t_vec		vec_add(t_vec u, t_vec v);
t_vec		vec_sub(t_vec u, t_vec v);
t_vec		vec_multi(t_vec u, t_vec v);
t_vec		vec_multi_double(t_vec u, double n);

// ---------vector2.c--------////
t_vec		vec_div_double(t_vec u, double n);
t_vec		vec_min(t_vec vec1, t_vec vec2);
double		vec_dot(t_vec u, t_vec v);
t_vec		vec_cross(t_vec u, t_vec v);
double		vec_len(t_vec u);

// ---------vector3.c--------////
t_vec		vec_unit(t_vec u);
double		vec_len_sqr(t_vec u);

// ---------tmp--------//
void		print_obj(t_object *obj);
void		print_cam(t_camera *cam);
void		debugPrintVec(char *str, t_vec *vector);
void		ae();
void		debugPrintDouble(char *str1, char *str2, double a, double b);

#endif