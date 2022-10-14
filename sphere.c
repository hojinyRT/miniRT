/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sphere.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jinypark <jinypark@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/13 15:54:43 by jinypark          #+#    #+#             */
/*   Updated: 2022/10/14 20:00:36 by jinypark         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"

static void	get_sphere_uv(t_hit_record *rec, t_sphere *sp, double size)
{
	t_vec	n;
	double	phi;
	double	theta;

	n = rec->normal;
	if ((n.x == 0 && n.y == 1 && n.z == 0))
		rec->e1 = vec_unit(vec_cross(vec_init(0, 0, -1), n));
	else if ((n.x == 0 && n.y == -1 && n.z == 0))
		rec->e1 = vec_unit(vec_cross(vec_init(0, 0, 1), n));
	else
		rec->e1 = vec_unit(vec_cross(vec_init(0, 1, 0), n));
	rec->e2 = vec_unit(vec_cross(n, rec->e1));
	theta = atan2((rec->p.x - sp->center.x), (rec->p.z - sp->center.z));
	phi = acos((rec->p.y - sp->center.y) / \
				vec_len(vec_sub(rec->p, sp->center)));
	rec->u = (theta / (M_PI));
	if (rec->u < 0)
		rec->u += 1;
	rec->u = fmod(rec->u, size) / size;
	rec->v = fmod(phi / M_PI, size) / size;
}

static int	get_sphere_root(t_formula *fo, t_ray ray, \
							t_hit_record *rec, t_sphere *sp)
{
	t_vec		oc;

	oc = vec_sub(ray.orig, sp->center);
	fo->a = vec_len_sqr(ray.dir);
	fo->half_b = vec_dot(oc, ray.dir);
	fo->c = vec_len_sqr(oc) - sp->radius2;
	fo->dis = fo->half_b * fo->half_b - fo->a * fo->c;
	if (fo->dis < 0)
		return (FALSE);
	fo->sqrtd = sqrt(fo->dis);
	fo->root = (-fo->half_b - fo->sqrtd) / fo->a;
	if ((fo->root < rec->tmin || rec->tmax < fo->root))
		fo->root = (-fo->half_b + fo->sqrtd) / fo->a;
	if (fo->root < rec->tmin || rec->tmax < fo->root)
		return (FALSE);
	return (TRUE);
}

int	hit_sphere(t_object *obj, t_ray ray, t_hit_record *rec)
{
	t_sphere	*sp;
	t_formula	fo;	

	sp = (t_sphere *)obj->element;
	if (!get_sphere_root(&fo, ray, rec, sp))
		return (FALSE);
	rec->color = obj->color;
	rec->t = fo.root;
	rec->p = ray_at(ray, fo.root);
	rec->normal = vec_div_double(vec_sub(rec->p, sp->center), sp->radius);
	get_sphere_uv(rec, sp, 1);
	if (obj->bump)
	{
		if (obj->texture->img_ptr)
			rec->color = texture_rgb(obj, rec);
		rec->normal = bump_normal(obj, rec);
	}
	set_face_normal(ray, rec);
	return (TRUE);
}
