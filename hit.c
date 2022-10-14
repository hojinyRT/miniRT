/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   hit.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jinypark <jinypark@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/13 15:59:36 by jinypark          #+#    #+#             */
/*   Updated: 2022/10/14 20:00:36 by jinypark         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"

void	record_init(t_hit_record *rec)
{
	ft_memset(rec, 0, sizeof(t_hit_record));
	rec->tmin = EPSILON;
	rec->tmax = INFINITY;
}

int	in_shadow(t_info *info, t_vec light_dir)
{
	double			light_len;
	t_ray			light_ray;
	t_hit_record	rec;

	light_len = vec_len(light_dir);
	light_ray = ray_init(vec_add(info->rec.p, \
				vec_multi_double(light_dir, EPSILON)), light_dir);
	rec.tmin = EPSILON;
	rec.tmax = light_len;
	if (hit(info->obj, light_ray, &rec))
		return (TRUE);
	return (FALSE);
}

int	hit_obj(t_object *obj, t_ray ray, t_hit_record *rec)
{
	int	hit_result;

	hit_result = FALSE;
	if (obj->type == SP)
		hit_result = hit_sphere(obj, ray, rec);
	else if (obj->type == PL)
		hit_result = hit_plane(obj, ray, rec);
	else if (obj->type == CY)
		hit_result = hit_cylinder(obj, ray, rec);
	else if (obj->type == CN)
		hit_result = hit_cone(obj, ray, rec);
	return (hit_result);
}

int	hit(t_object *obj, t_ray ray, t_hit_record *rec)
{
	int				hit_anything;
	t_hit_record	temp_rec;
	t_object		*curr;

	curr = obj;
	temp_rec = *rec;
	hit_anything = FALSE;
	while (curr)
	{	
		if (hit_obj(curr, ray, &temp_rec))
		{
			hit_anything = TRUE;
			temp_rec.tmax = temp_rec.t;
			temp_rec.checker = curr->checker;
			*rec = temp_rec;
		}
		curr = curr->next;
	}
	return (hit_anything);
}
