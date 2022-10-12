#include "minirt.h"

void	get_cylinder_uv(t_hit_record *rec, t_cylinder *cy, double size)
{
	double			theta;
	double			p_e1;
	double			p_e2;

	if ((rec->p.x == 0 && rec->p.y == 0 && rec->p.z == 1))
		rec->e1 = vec_unit(vec_cross(vec_init(0, 1, 0), cy->normal));
	else if ((rec->p.x == 0 && rec->p.y == 0 && rec->p.z == -1))
		rec->e1 = vec_unit(vec_cross(vec_init(0, -1, 0), cy->normal));
	else
		rec->e1 = vec_unit(vec_cross(vec_init(0, 0, 1), cy->normal));
	rec->e2 = vec_unit(vec_cross(cy->normal, rec->e1));
	p_e1 = vec_dot(vec_sub(rec->p, cy->center), rec->e1);
	p_e2 = vec_dot(vec_sub(rec->p, cy->center), rec->e2);
	theta = atan2(p_e2, p_e1);
	rec->u = (theta / (M_PI)) + (rec->u < 0);
	rec->v = 1 - fmod(vec_dot(vec_sub(rec->p, cy->center), cy->normal) / \
									(cy->radius * M_PI), 1);
	rec->u = fmod(rec->u, size) / size;
	rec->v = fmod(rec->v, size) / size;
}

static int	is_valid(t_formula *fo, t_hit_record *rec, t_cylinder *cy)
{
	if ((vec_dot(vec_sub(rec->p, cy->center), cy->normal) > cy->height)
		|| (vec_dot(vec_sub(rec->p, cy->center), cy->normal) < 0)
		|| (fo->root < rec->tmin || rec->tmax < fo->root))
		return (TRUE);
	return (FALSE);
}

static int	get_cylinder_root(t_formula *fo, t_ray ray, t_hit_record *rec, \
			t_cylinder *cy)
{
	t_vec		oc;

	oc = vec_sub(ray.orig, cy->center);
	fo->a = vec_dot(ray.dir, ray.dir) - vec_dot(ray.dir, cy->normal) * \
				vec_dot(ray.dir, cy->normal);
	fo->half_b = vec_dot(oc, ray.dir) - ((vec_dot(ray.dir, cy->normal)) * \
				vec_dot(oc, cy->normal));
	fo->c = vec_len_sqr(oc) - vec_dot(oc, cy->normal) * \
				vec_dot(oc, cy->normal) - (cy->radius) * (cy->radius);
	fo->dis = fo->half_b * fo->half_b - fo->a * fo->c;
	if (fo->dis < 0)
		return (FALSE);
	fo->sqrtd = sqrt(fo->dis);
	fo->root = (-fo->half_b - fo->sqrtd) / fo->a;
	rec->p = ray_at(ray, fo->root);
	if (is_valid(fo, rec, cy))
	{
		fo->root = (-fo->half_b + fo->sqrtd) / fo->a;
		rec->p = ray_at(ray, fo->root);
		if (is_valid(fo, rec, cy))
			return (FALSE);
	}
	return (TRUE);
}

int	hit_cylinder(t_object *obj, t_ray ray, t_hit_record *rec)
{
	t_cylinder	*cy;
	t_formula	fo;
	t_vec		cp;
	t_vec		cq;
	double		cq_val;

	cy = (t_cylinder *)obj->element;
	// hit_cap(cy->cap, ray, rec);
	// hit_cap(cy->cap, ray, rec);
	if (!get_cylinder_root(&fo, ray, rec, cy))
		return (FALSE);
	cp = vec_sub(rec->p, cy->center);
	cq_val = vec_dot(cp, cy->normal);
	cq = vec_multi_double(cy->normal, cq_val);
	rec->t = fo.root;
	rec->color = obj->color;
	rec->normal = vec_unit(vec_sub(cp, cq));
	get_cylinder_uv(rec, cy, 1);
	if (obj->bump)
	{
		if (obj->texture->img_ptr)
			rec->color = texture_rgb(obj, rec);
		rec->normal = bump_normal(obj, rec);
	}
	set_face_normal(ray, rec);
	return (TRUE);
}
