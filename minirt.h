#ifndef MINIRT_H
# define MINIRT_H

# include <stdio.h>
# include <math.h>
# include "libft/libft.h"
# include "mlx/mlx.h"

typedef struct  s_img
{
  void	*ptr;
  int	*addr;
  int	bits_per_pixel;
  int	line_length;
  int	endian;
}               t_img;

typedef struct s_info
{
	void		*mlx;
	void		*win;
}				t_info;

typedef struct s_vec
{
	float	x;
	float	y;
	float	z;
}		t_vec;

typedef t_vec	t_color;
typedef t_vec	t_point;

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

#endif
