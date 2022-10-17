/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cylinder.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hchang <hchang@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/13 11:46:09 by jinypark          #+#    #+#             */
/*   Updated: 2022/10/17 15:39:10 by hchang           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

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
	rec->u = (theta / (M_PI));
	rec->u += (rec->u < 0);
	rec->v = 1 - fmod(vec_dot(vec_sub(rec->p, cy->center), cy->normal) / \
									(cy->radius * M_PI), 1);
	rec->u = fmod(rec->u, size) / size;
	rec->v = fmod(rec->v, size) / size;
}

static int	get_cylinder_root(t_formula *fo, t_ray ray, t_hit_record *rec, \
			t_cylinder *cy)
{
	t_vec		co;

	co = vec_sub(ray.orig, cy->center);
	fo->a = vec_dot(ray.dir, ray.dir) - vec_dot(ray.dir, cy->normal) * \
				vec_dot(ray.dir, cy->normal);
	fo->half_b = vec_dot(co, ray.dir) - ((vec_dot(ray.dir, cy->normal)) * \
				vec_dot(co, cy->normal));
	fo->c = vec_len_sqr(co) - vec_dot(co, cy->normal) * \
				vec_dot(co, cy->normal) - (cy->radius) * (cy->radius);
	fo->dis = fo->half_b * fo->half_b - fo->a * fo->c;
	if (fo->dis < 0)
		return (FALSE);
	fo->sqrtd = sqrt(fo->dis);
	fo->root = (-fo->half_b - fo->sqrtd) / fo->a;
	rec->p = ray_at(ray, fo->root);
	return (check_validation(fo, *rec, cy, &ray));
}

int	hit_cylinder_body(t_object *obj, t_ray ray, t_hit_record *rec)
{
	t_cylinder	*cy;
	t_formula	fo;
	t_vec		cp;
	t_vec		cq;

	cy = (t_cylinder *)obj->element;
	if (!get_cylinder_root(&fo, ray, rec, cy))
		return (FALSE);
	rec->t = fo.root;
	rec->p = ray_at(ray, fo.root);
	cp = vec_sub(rec->p, cy->center);
	cq = vec_multi_double(cy->normal, vec_dot(cp, cy->normal));
	rec->normal = vec_unit(vec_sub(cp, cq));
	rec->color = obj->color;
	get_cylinder_uv(rec, cy, 1);
	if (obj->bump)
	{
		if (obj->texture->img_ptr)
			rec->color = texture_rgb(obj, rec);
		rec->normal = bump_normal(obj, rec);
	}
	rec->tmax = fo.root;
	set_face_normal(ray, rec);
	return (TRUE);
}

int	hit_cylinder(t_object *obj, t_ray ray, t_hit_record *rec)
{
	t_cylinder	*cy;
	int			hit_anything;

	cy = (t_cylinder *)obj->element;
	hit_anything = FALSE;
	if (hit_cylinder_body(obj, ray, rec))
		hit_anything = TRUE;
	if (hit_cap(cy->cap, ray, rec, obj))
		hit_anything = TRUE;
	if (hit_cap(cy->cap->next, ray, rec, obj))
		hit_anything = TRUE;
	return (hit_anything);
}
