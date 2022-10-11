#include "minirt.h"

void	get_sphere_uv(t_hit_record *rec, t_point center, double size)
{
	double			phi;
	double			theta;
	const t_vec		n = rec->normal;
	t_vec			e1;
	t_vec			e2;

	if ((n.x == 0 && n.y == 1 && n.z == 0))
		e1 = vec_unit(vec_cross(vec_init(0, 0, -1), n));
	else if ((n.x == 0 && n.y == -1 && n.z == 0))
		e1 = vec_unit(vec_cross(vec_init(0, 0, 1), n));
	else
		e1 = vec_unit(vec_cross(vec_init(0, 1, 0), n));
	e2 = vec_unit(vec_cross(n, e1));
	rec->e1 = e1;
	rec->e2 = e2;
	theta = atan2((rec->p.x - center.x), (rec->p.z - center.z));
	phi = acos((rec->p.y - center.y) / vec_len(vec_sub(rec->p, center)));
	rec->u = (theta / (M_PI));
	rec->v = phi / M_PI;
	if (rec->u < 0)
		rec->u += 1;
	rec->u = fmod(rec->u, size) / size;
	rec->v = fmod(rec->v, size) / size;
}

int	hit_sphere(t_object *obj, t_ray ray, t_hit_record *rec)
{
	t_sphere	*sp;
	t_vec		oc;
	double		a;
	double		c;
	double		dis;
	double		half_b;
	double		sqrtd;
	double		root;

	sp = (t_sphere *)obj->element;
	oc = vec_sub(ray.orig, ((t_sphere*)obj->element)->center);
	a = vec_len_sqr(ray.dir);
	half_b = vec_dot(oc, ray.dir);
	c = vec_len_sqr(oc) - ((t_sphere*)obj->element)->radius2;
	dis = half_b * half_b - a * c;
	if (dis < 0)
		return (FALSE);
	sqrtd = sqrt(dis);
	root = (-half_b - sqrtd) / a;
	if (root < rec->tmin || rec->tmax < root)
		root = (-half_b + sqrtd) / a;
    if (root < rec->tmin || rec->tmax < root)
		return (FALSE);
	rec->albedo = obj->albedo;
	rec->t = root;
	rec->p = ray_at(ray, root);
	rec->normal = vec_div_double(vec_sub(rec->p, ((t_sphere*)obj->element)->center), ((t_sphere*)obj->element)->radius);
	get_sphere_uv(rec, sp->center, 1); //조정 바람
	if (obj->bump->file_name)
	{
		// rec->albedo = tex_rgb(obj, rec);
		if (obj->tex->img_ptr)
			rec->albedo = tex_rgb(obj, rec);
		rec->normal = bump_normal(obj, rec);
	}
	set_face_normal(ray, rec);
	return (TRUE);
}