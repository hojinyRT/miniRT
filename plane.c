#include "minirt.h"

static void	get_plane_uv(t_hit_record *rec, t_point center, double size)
{
	const t_vec		p = vec_sub(rec->p, center);
	const t_vec		n = rec->normal;

	if ((n.x == 0 && n.y == 1 && n.z == 0))
		rec->e1 = vec_unit(vec_cross(vec_init(0, 0, -1), n));
	else if ((n.x == 0 && n.y == -1 && n.z == 0))
		rec->e1 = vec_unit(vec_cross(vec_init(0, 0, 1), n));
	else
		rec->e1 = vec_unit(vec_cross(vec_init(0, 1, 0), n));
	rec->e2 = vec_unit(vec_cross(n, rec->e1));
	rec->u = fmod(vec_dot(rec->e1, p), size) / size + (rec->u < 0);
	rec->v = fmod(vec_dot(rec->e2, p), size) / size + (rec->v < 0);
	rec->v = 1 - rec->v;
}

static int	get_plane_root(double *root, t_ray ray, t_hit_record *rec, t_plane *pl)
{
	double	numrator;
	double	denominator;

	denominator = vec_dot(ray.dir, pl->normal);
	if (fabs(denominator) < EPSILON)
		return (FALSE);
	numrator = vec_dot(vec_sub(pl->center, ray.orig), pl->normal);
	*root = numrator / denominator;
	if (*root < rec->tmin || rec->tmax < *root)
		return (FALSE);
	return (TRUE);
}

int	hit_plane(t_object *obj, t_ray ray, t_hit_record *rec)
{
	t_plane	*pl;
	double	root;

	pl = (t_plane *)obj->element;
	if (!get_plane_root(&root, ray, rec, pl))
		return (FALSE);
	rec->t = root;
	rec->p = ray_at(ray, root);
	rec->color = obj->color;
	rec->normal = pl->normal;
	get_plane_uv(rec, pl->center, 10);
	if (obj->bump)
	{
		if (obj->texture->img_ptr)
			rec->color = texture_rgb(obj, rec);
		rec->normal = bump_normal(obj, rec);
	}
	set_face_normal(ray, rec);
	return (TRUE);
}

static void	get_cap_uv(t_hit_record *rec, t_point center, t_vec normal, double size, double r)
{
	double			theta;
	t_vec			n = rec->normal;
	t_vec			e1;
	t_vec			e2;
	double			p_e1;
	double			p_e2;

	if ((n.x == 0 && n.y == 0 && n.z == 1))
		e1 = vec_unit(vec_cross(vec_init(0, 1, 0), normal));
	else if ((n.x == 0 && n.y == 0 && n.z == -1))
		e1 = vec_unit(vec_cross(vec_init(0, -1, 0), normal));
	else
		e1 = vec_unit(vec_cross(vec_init(0, 0, 1), normal));
	e2 = vec_unit(vec_cross(normal, e1));
	rec->e1 = e1;
	rec->e2 = e2;
	p_e1 = vec_dot(vec_sub(rec->p, center), e1);
	p_e2 = vec_dot(vec_sub(rec->p, center), e2);
	theta = atan2(p_e2, p_e1);
	rec->u = (theta / (M_PI));
	if (rec->u < 0)
		rec->u += 1;
	rec->v =  vec_len(vec_sub(rec->p, center)) / r;
	rec->u = fmod(rec->u, size) / size;
	rec->v = fmod(rec->v, size) / size;
}

int	hit_cap(t_object *obj, t_ray ray, t_hit_record *rec, t_object *body)
{
	t_plane	*pl;
	double	root;
	t_vec	pcv;

	pl = (t_plane *)obj->element;
	if (!get_plane_root(&root, ray, rec, pl))
		return (FALSE);
	rec->p = ray_at(ray, root);
	pcv = vec_sub(rec->p, pl->center);
    if (vec_dot(pcv, pcv) > pl->radius * pl->radius)
        return (FALSE);
	rec->t = root;
	rec->p = ray_at(ray, root);
	rec->color = obj->color;
	rec->normal = pl->normal;
	get_cap_uv(rec, pl->center, pl->normal, 1, pl->radius);
	if (body->bump)
	{
		if (body->texture->img_ptr)
			rec->color = texture_rgb(body, rec);
		rec->normal = bump_normal(body, rec);
	}
	rec->tmax = rec->t;
	set_face_normal(ray, rec);
	return (TRUE);
}
