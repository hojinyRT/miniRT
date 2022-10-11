#include "minirt.h"

t_vec	convert_color_to_normal(int	color)
{
	t_vec	res;

	res.x = color >> 16 & 0xFF;
	res.y = color >> 8 & 0xFF;
	res.z = color & 0xFF;
	if ((res.x == 0 && res.y == 0 && res.z == 0))
		ft_strerror("범프맵 이상함");
	res = vec_div_double(res, 255);
	res = vec_sub(vec_multi_double(res, 2), vec_init(1, 1, 1));
	return (res);
}

int	convert_color(t_vec clr)
{
	int tmp = ((int)clr.x * 16 * 16 * 16 * 16) + ((int)clr.y * 16 * 16) + (int)(clr.z);
	return (tmp);
}

void    set_face_normal(t_ray ray, t_hit_record *rec)
{
    rec->front_face = vec_dot(ray.dir, rec->normal) < 0;
	if (rec->front_face == 0)
		rec->normal = vec_multi_double(rec->normal, -1);
	//front_face를 현재 쓰는 곳이 없음 리팩토링할 때 판단하기 바람.
    return ;
}

t_vec	convert_int_to_rgb(int	color)
{
	t_vec	res;

	res.x = color >> 16 & 0xFF;
	res.y = color >> 8 & 0xFF;
	res.z = color & 0xFF;
	return (res);
}

t_vec	tex_rgb(t_object *obj, t_hit_record *rec)
{
	int x;
	int y;
	t_vec tmp;

	x = (int)(rec->u * obj->tex->width);
	y = (int)(rec->v * obj->tex->height);
	tmp = convert_int_to_rgb(*(unsigned int *)(obj->tex->addr + obj->tex->line_length * y + x * obj->tex->bits_per_pixel / 8));
	tmp = vec_div_double(tmp, 255);
	return (tmp);
}