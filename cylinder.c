#include "minirt.h"

void	get_cylinder_uv(t_hit_record *rec, t_point center, t_vec normal, double size, double r)
{
	double			theta;
	t_vec			e1;
	t_vec			e2;
	double			p_e1;
	double			p_e2;

	(void)r;
	if ((rec->p.x == 0 && rec->p.y == 0 && rec->p.z == 1))
		e1 = vec_unit(vec_cross(vec_init(0, 1, 0), normal));
	else if ((rec->p.x == 0 && rec->p.y == 0 && rec->p.z == -1))
		e1 = vec_unit(vec_cross(vec_init(0, -1, 0), normal));
	else
		e1 = vec_unit(vec_cross(vec_init(0, 0, 1), normal));
	e2 = vec_unit(vec_cross(normal, e1));
	p_e1 = vec_dot(vec_sub(rec->p, center), e1);
	p_e2 = vec_dot(vec_sub(rec->p, center), e2);
	theta = atan2(p_e2, p_e1);
	rec->e1 = e1;
	rec->e2 = e2;
	rec->u = (theta / (M_PI));
	// rec->v = fmod(vec_dot(vec_sub(rec->p, center), normal) / (r * M_PI), 1);
	rec->v = fmod(vec_dot(vec_sub(rec->p, center), normal) / (r * M_PI), 1);
	if (rec->u < 0)
		rec->u += 1;

	rec->v = 1 - rec->v;
	rec->u = fmod(rec->u, size) / size;
	rec->v = fmod(rec->v, size) / size;
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
	rec->t = fo->root;
	rec->p = ray_at(ray, fo->root);
	if ((vec_dot(vec_sub(rec->p, cy->center), cy->normal) > cy->height)
		|| (vec_dot(vec_sub(rec->p, cy->center), cy->normal) < 0) 
		|| (fo->root < rec->tmin || rec->tmax < fo->root))
		fo->root = (-fo->half_b + fo->sqrtd) / fo->a;
    if (fo->root < rec->tmin || rec->tmax < fo->root)
		return (FALSE);
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
	if(!get_cylinder_root(&fo, ray, rec, cy))
		return (FALSE);
	rec->p = ray_at(ray, fo.root);
	cp = vec_sub(rec->p, cy->center);
	cq_val = vec_dot(cp, cy->normal);
	cq = vec_multi_double(cy->normal, cq_val);
	if ((vec_dot(vec_sub(rec->p, cy->center), cy->normal) > cy->height)
		|| (vec_dot(vec_sub(rec->p, cy->center), cy->normal) < 0)
		|| (fo.root < rec->tmin || rec->tmax < fo.root))
		return (FALSE);
	rec->t = fo.root;
	rec->color = obj->color;
	rec->normal = vec_unit(vec_sub(cp, cq));
	get_cylinder_uv(rec, cy->center, cy->normal, 1, cy->radius);
	if (obj->bump)
	{
		if (obj->texture->img_ptr)
			rec->color = texture_rgb(obj, rec);
		rec->normal = bump_normal(obj, rec);
	}
	set_face_normal(ray, rec);
    return (TRUE);
}