/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   light.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hchang <hchang@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/14 09:39:18 by jinypark          #+#    #+#             */
/*   Updated: 2022/10/17 14:38:11 by hchang           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"

t_light	*light_init(t_vec light_origin, t_vec light_color, double brightness)
{
	t_light	*light;

	light = (t_light *)my_calloc(1, sizeof(t_light));
	light->origin = light_origin;
	light->light_color = light_color;
	light->brightness = brightness;
	light->next = 0;
	return (light);
}

t_vec	reflect(t_vec v, t_vec n)
{
	return (vec_sub(v, vec_multi_double(n, vec_dot(v, n) * 2)));
}

t_color	get_specular(t_info *info, t_light *light, t_vec *light_dir)
{
	t_vec	view_dir;
	t_vec	reflect_dir;
	double	spec;

	view_dir = vec_unit(vec_multi_double(info->ray.dir, -1));
	reflect_dir = reflect(vec_multi_double(*light_dir, -1), info->rec.normal);
	spec = pow(fmax(vec_dot(view_dir, reflect_dir), 0.0), KSN);
	return (vec_multi_double(vec_multi_double(light->light_color, KS), spec));
}

t_vec	point_light_get(t_info *info, t_light *light)
{
	t_vec	light_dir;
	double	brightness;
	t_color	diffuse;
	double	kd;
	t_color	specular;

	light_dir = vec_sub(light->origin, info->rec.p);
	if (in_shadow(info, light_dir))
		return (vec_init(0, 0, 0));
	light_dir = vec_unit(light_dir);
	kd = fmax(vec_dot(info->rec.normal, light_dir), 0.0);
	diffuse = vec_multi_double(light->light_color, kd);
	specular = get_specular(info, light, &light_dir);
	brightness = light->brightness * LUMEN;
	return (vec_multi_double(vec_add(diffuse, specular), brightness));
}

t_vec	phong_lighting(t_info *info)
{
	t_color	light_color;
	t_light	*lights;
	t_color	color;

	light_color = vec_init(0, 0, 0);
	lights = info->light;
	while (lights)
	{
		light_color = vec_add(light_color, point_light_get(info, lights));
		lights = lights->next;
	}
	light_color = vec_add(light_color, *info->ambient);
	if (info->rec.checker)
		color = checkerboard_value((info->rec));
	else
		color = info->rec.color;
	return (vec_multi_double(vec_min(vec_multi(light_color, color), \
			vec_init(1, 1, 1)), 255));
}
