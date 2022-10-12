#include "minirt.h"

int	hit_cone(t_object *obj, t_ray ray, t_hit_record *rec)
{
	t_cylinder	*cy;
	double		a;
	double		half_b;
	double		c;
	double		dis;
	double		sqrtd;
	double		root;
	double	m;
	t_vec	w;

	cy = (t_cylinder *)obj->element;
	w = vec_sub(ray.orig, cy->center);
	m = cy->radius2 / (cy->height * cy->height);
	double	v_n_pow = (vec_dot(ray.dir, cy->normal)) * (vec_dot(ray.dir, cy->normal));
	double	v_n_dot_w_n = (vec_dot(ray.dir, cy->normal) * vec_dot(w, cy->normal));
	double	w_n_pow = vec_dot(w, cy->normal) * (vec_dot(w, cy->normal));
	a = vec_dot(ray.dir, ray.dir) - ((m + 1) * v_n_pow);
	half_b = vec_dot(ray.dir, w) - ((m + 1) * v_n_dot_w_n) + (m * cy->height * vec_dot(ray.dir, cy->normal));
	c = vec_dot(w, w) - ((m + 1) * w_n_pow) + (2 * m * cy->height * vec_dot(w, cy->normal)) - cy->radius2;
	dis = half_b * half_b - a * c;
	if (dis < 0)
		return (FALSE);
	sqrtd = sqrt(dis);
	root = (-half_b - sqrtd) / a;
	rec->t = root;
	rec->p = ray_at(ray, root);
	if (vec_dot(vec_sub(rec->p, cy->center), cy->normal) < 0
        || vec_dot(vec_sub(rec->p, cy->center), cy->normal) > cy->height
		|| (root < rec->tmin || rec->tmax < root))
	{
		root = (-half_b + sqrtd) / a;
		if (root < rec->tmin || rec->tmax < root)
			return (FALSE);
		rec->t = root;
		rec->p = ray_at(ray, root);
	}
    if (root < rec->tmin || rec->tmax < root)
		return (FALSE);
	double	nom = vec_len_sqr(vec_sub(vec_sub(rec->p, cy->center), vec_multi_double(cy->normal, cy->height)));
	double	denom = cy->height - vec_dot(vec_sub(rec->p, cy->center), cy->normal);
	double	a_c = cy->height - (nom / denom);
	rec->normal = vec_unit(vec_sub(rec->p, vec_add(cy->center, vec_multi_double(cy->normal, a_c))));
	rec->color = obj->color;
	set_face_normal(ray, rec);
	if (0 <= vec_dot(vec_sub(rec->p, cy->center), cy->normal) &&
		vec_dot(vec_sub(rec->p, cy->center), cy->normal) <= cy->height && (root >= rec->tmin || rec->tmax >= root))
	{
		get_cylinder_uv(rec, cy, 1);
		if (obj->bump)
		{
			if (obj->texture->img_ptr)
				rec->color = texture_rgb(obj, rec);
			rec->normal = bump_normal(obj, rec);
			set_face_normal(ray, rec);
		}
		return (TRUE);
	}
    return (FALSE);
}