#include "minirt.h"

t_ray	ray_init(t_point orig, t_vec dir)
{
	t_ray init;

	init.orig = orig;
	init.dir = vec_unit(dir);
	return (init);
}

t_point	ray_at(t_ray ray, double t)
{
	t_point at;

	at = vec_add(ray.orig, vec_multi_double(ray.dir, t));
	return (at);
}

void	ray_primary(t_ray *ray, t_camera *cam, double u, double v)
{
    ray->orig = cam->orig;
    ray->dir =  vec_unit(vec_sub(vec_add(vec_add(cam->start_point, \
							vec_multi_double(cam->horizontal, u)), \
							vec_multi_double(cam->vertical, v)), cam->orig));

}

t_color    ray_color(t_info *info)
{
    double			t;

	record_init(&(info->rec));
	if (hit(info->obj, info->ray, &(info->rec)))
	{
		return (phong_lighting(info));
	}
	else
	{
		t = 0.5 * (info->ray.dir.y + 1.0);
		return (vec_add(vec_multi_double(vec_init(255, 255, 255), 1.0 - t), vec_multi_double(vec_init(128, 178, 255), t)));
	}
}
