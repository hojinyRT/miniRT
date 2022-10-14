/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cone.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jinypark <jinypark@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/13 11:46:34 by jinypark          #+#    #+#             */
/*   Updated: 2022/10/14 20:00:36 by jinypark         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"

int	is_valid(t_formula *fo, t_hit_record *rec, t_cylinder *cy, int flag)
{
	if ((flag == 0 || flag == 2)
		&& ((vec_dot(vec_sub(rec->p, cy->center), cy->normal) > cy->height)
			|| (vec_dot(vec_sub(rec->p, cy->center), cy->normal) < 0)))
		return (FALSE);
	if ((flag == 1 || flag == 2)
		&& (fo->root < rec->tmin || rec->tmax < fo->root))
		return (FALSE);
	return (TRUE);
}

int	check_validation(t_formula *fo, t_hit_record rec, \
					t_cylinder *obj, t_ray *ray)
{
	if (!is_valid(fo, &rec, obj, 2))
	{
		fo->root = (-fo->half_b + fo->sqrtd) / fo->a;
		if (!is_valid(fo, &rec, obj, 1))
			return (FALSE);
		rec.t = fo->root;
		rec.p = ray_at(*ray, fo->root);
	}
	return (is_valid(fo, &rec, obj, 0));
}

static int	get_cone_root(t_formula *fo, t_ray ray, t_hit_record *rec, \
			t_cone *cn)
{
	double	m;
	t_vec	co;
	double	v_n_pow;
	double	v_n_dot_w_n;
	double	w_n_pow;

	co = vec_sub(ray.orig, cn->center);
	m = cn->radius2 / (cn->height * cn->height);
	v_n_pow = (vec_dot(ray.dir, cn->normal)) * (vec_dot(ray.dir, cn->normal));
	v_n_dot_w_n = (vec_dot(ray.dir, cn->normal) * vec_dot(co, cn->normal));
	w_n_pow = vec_dot(co, cn->normal) * (vec_dot(co, cn->normal));
	fo->a = vec_dot(ray.dir, ray.dir) - ((m + 1) * v_n_pow);
	fo->half_b = vec_dot(ray.dir, co) - ((m + 1) * v_n_dot_w_n) + \
				(m * cn->height * vec_dot(ray.dir, cn->normal));
	fo->c = vec_dot(co, co) - ((m + 1) * w_n_pow) + \
				(2 * m * cn->height * vec_dot(co, cn->normal)) - cn->radius2;
	fo->dis = fo->half_b * fo->half_b - fo->a * fo->c;
	if (fo->dis < 0)
		return (FALSE);
	fo->sqrtd = sqrt(fo->dis);
	fo->root = (-fo->half_b - fo->sqrtd) / fo->a;
	rec->t = fo->root;
	rec->p = ray_at(ray, fo->root);
	return (check_validation(fo, *rec, cn, &ray));
}

int	hit_cone_body(t_object *obj, t_ray ray, t_hit_record *rec)
{
	t_cone		*cn;
	t_formula	fo;
	double		ac;

	cn = (t_cone *)obj->element;
	if (!get_cone_root(&fo, ray, rec, cn))
		return (FALSE);
	rec->t = fo.root;
	rec->p = ray_at(ray, fo.root);
	ac = cn->height - (vec_len_sqr(vec_sub(vec_sub(rec->p, cn->center), \
				vec_multi_double(cn->normal, cn->height))) / (cn->height - \
				vec_dot(vec_sub(rec->p, cn->center), cn->normal)));
	rec->normal = vec_unit(vec_sub(rec->p, vec_add(cn->center, \
						vec_multi_double(cn->normal, ac))));
	rec->color = obj->color;
	get_cylinder_uv(rec, cn, 1);
	if (obj->bump)
	{
		if (obj->texture->img_ptr)
			rec->color = texture_rgb(obj, rec);
		rec->normal = bump_normal(obj, rec);
	}
	set_face_normal(ray, rec);
	return (TRUE);
}

int	hit_cone(t_object *obj, t_ray ray, t_hit_record *rec)
{
	int				hit_anything;
	t_hit_record	temp_rec;
	t_cone			*cn;

	temp_rec = *rec;
	cn = (t_cone *)obj->element;
	hit_anything = FALSE;
	if (hit_cone_body(obj, ray, &temp_rec))
	{
		hit_anything = TRUE;
		rec->tmax = temp_rec.t;
		rec->checker = obj->checker;
		*rec = temp_rec;
	}
	if (hit_cap(cn->cap, ray, &temp_rec, obj))
	{
		hit_anything = TRUE;
		rec->tmax = temp_rec.t;
		rec->checker = obj->checker;
		*rec = temp_rec;
	}
	return (hit_anything);
}
