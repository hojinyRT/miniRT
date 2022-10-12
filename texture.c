#include "minirt.h"

void	get_texture_addr(t_object *obj, t_mlx *mlx)
{
	int		format[2];
	int		idx[2];
	char	*texture;

	ft_bzero(idx, sizeof(idx));
	texture = ft_strjoin("t", obj->bump->file_name);
	obj->tex->img_ptr = mlx_png_file_to_image(mlx->ptr, texture, &format[0], &format[1]);
	obj->tex->addr = mlx_get_data_addr(obj->tex->img_ptr, \
											&(obj->tex->bits_per_pixel), \
											&(obj->tex->line_length), \
											&(obj->tex->endian));
	obj->tex->width = format[0];
	obj->tex->height = format[1];
	free(texture);
}

void	get_bump_addr(t_object *obj, t_mlx *mlx)
{
	int		format[2];
	int		idx[2];

	ft_bzero(idx, sizeof(idx));
	obj->bump->img_ptr = mlx_png_file_to_image(mlx->ptr, obj->bump->file_name, &format[0], &format[1]);
	if (!obj->bump->img_ptr)
		ft_strerror("Error : no such file or directory");
	obj->bump->addr = mlx_get_data_addr(obj->bump->img_ptr, \
											&(obj->bump->bits_per_pixel), \
											&(obj->bump->line_length), \
											&(obj->bump->endian));
	obj->bump->width = format[0];
	obj->bump->height = format[1];
}

t_color	checkerboard_value(t_hit_record rec)
{
	const int		width = 10;
	const int		height = 10;
	const double	u2 = floor(rec.u * width);
	const double	v2 = floor(rec.v * height);
	if (fmod(u2 + v2, 2.) == 0)
		return (rec.albedo);
	return (vec_init(1, 1, 1));
}

t_vec	bump_normal(t_object *obj, t_hit_record *rec)
{
	int x;
	int y;
	t_vec tmp;
	t_vec ul, vl, zl;

	// Local = t * UL + b * VL + n * ZL
	x = (int)(rec->u * (double)(obj->bump->width - 1));
	y = (int)(rec->v * (double)(obj->bump->height - 1));
	tmp = convert_color_to_normal(*(unsigned int *)(obj->bump->addr + obj->bump->line_length * y + x * obj->bump->bits_per_pixel / 8));
	ul = vec_multi_double(rec->e1, tmp.x);
	vl = vec_multi_double(rec->e2, tmp.y);
	zl = vec_multi_double(rec->normal, tmp.z);
	tmp = vec_add(vec_add(ul, vl), zl);
	tmp = vec_unit(tmp);
	return (tmp);
}