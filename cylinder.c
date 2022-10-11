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
	// debugPrintVec("rec", &rec->p);
	// debugPrintDouble("u", "v", rec->u, rec->v);
	rec->v = 1 - rec->v;
	rec->u = fmod(rec->u, size) / size;
	rec->v = fmod(rec->v, size) / size;
}

int	hit_cylinder(t_object *obj, t_ray ray, t_hit_record *rec)
{
	t_cylinder	*cy;
	t_vec		oc;
	double		a;
	double		half_b;
	double		c;
	double		dis;
	double		sqrtd;
	double		root;
	t_vec		cp, cq;
	double		cq_val;

	cy = (t_cylinder *)obj->element;
	oc = vec_sub(ray.orig, cy->center);
	a = vec_dot(ray.dir, ray.dir) - vec_dot(ray.dir, cy->normal) * vec_dot(ray.dir, cy->normal);
	half_b = vec_dot(oc, ray.dir) - ((vec_dot(ray.dir, cy->normal)) *  vec_dot(oc, cy->normal));
	c = vec_len_sqr(oc) - vec_dot(oc, cy->normal) * vec_dot(oc, cy->normal) - (cy->radius) * (cy->radius);
	dis = half_b * half_b - a * c;
	if (dis < 0)
		return (FALSE);
	sqrtd = sqrt(dis);
	root = (-half_b - sqrtd) / a;
	t_hit_record tmp;
	tmp.t = root;
	tmp.p = ray_at(ray, root);
	// rec->t = root;
	// rec->p = ray_at(ray, root);
	if ((vec_dot(vec_sub(tmp.p, cy->center), cy->normal) > cy->height) || (vec_dot(vec_sub(tmp.p, cy->center), cy->normal) < 0) || (root < rec->tmin || rec->tmax < root))
	{
		root = (-half_b + sqrtd) / a;
		if (root < rec->tmin || rec->tmax < root)
			return (FALSE);
		tmp.t = root;
		tmp.p = ray_at(ray, root);
	}
    if (root < rec->tmin || rec->tmax < root)
		return (FALSE);
	cp = vec_sub(tmp.p, cy->center);
	cq_val = vec_dot(cp, cy->normal);
	cq = vec_multi_double(cy->normal, cq_val);
	if ((vec_dot(vec_sub(tmp.p, cy->center), cy->normal) > cy->height) || (vec_dot(vec_sub(tmp.p, cy->center), cy->normal) < 0) || (root < rec->tmin || rec->tmax < root))
		return (FALSE);
	// if (0 > vec_dot(vec_sub(tmp.p, cy->center), cy->normal) || vec_dot(vec_sub(tmp.p, cy->center), cy->normal) > cy->height)
	// 	return (FALSE);
	rec->albedo = obj->albedo;
	rec->normal = vec_unit(vec_sub(cp, cq));
	rec->t = root;
	rec->p = ray_at(ray, root);
	set_face_normal(ray, rec); // rec의 법선벡터와 광선의 방향벡터를 비교해서 앞면인지 뒷면인지 t_bool 값으로 저장.
	get_cylinder_uv(rec, cy->center, cy->normal, 1, cy->radius);
	if (obj->bump->file_name)
	{
		if (obj->tex->img_ptr)
			rec->albedo = tex_rgb(obj, rec);
		rec->normal = bump_normal(obj, rec);
	}
	set_face_normal(ray, rec); // rec의 법선벡터와 광선의 방향벡터를 비교해서 앞면인지 뒷면인지 t_bool 값으로 저장.
    return (TRUE);
}