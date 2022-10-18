/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils2.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hchang <hchang@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/13 15:49:40 by jinypark          #+#    #+#             */
/*   Updated: 2022/10/18 14:50:03 by hchang           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"

t_vec	convert_color_to_normal(int color)
{
	t_vec	res;

	res.x = color >> 16 & 0xFF;
	res.y = color >> 8 & 0xFF;
	res.z = color & 0xFF;
	if ((res.x == 0 && res.y == 0 && res.z == 0))
		ft_strerror("Error \ninvalid bump map");
	res = vec_div_double(res, 255);
	res = vec_sub(vec_multi_double(res, 2), vec_init(1, 1, 1));
	return (res);
}

int	convert_color(t_vec clr)
{
	return (((int)clr.x << 16) + ((int)clr.y << 8) + (int)clr.z);
}

void	set_face_normal(t_ray ray, t_hit_record *rec)
{
	if (vec_dot(ray.dir, rec->normal) > 0)
		rec->normal = vec_multi_double(rec->normal, -1);
	return ;
}

t_vec	convert_int_to_rgb(int color)
{
	t_vec	res;

	res.x = color >> 16 & 0xFF;
	res.y = color >> 8 & 0xFF;
	res.z = color & 0xFF;
	return (res);
}

t_vec	texture_rgb(t_object *obj, t_hit_record *rec)
{
	int		x;
	int		y;
	t_vec	tmp;

	x = (int)(rec->u * (obj->texture->width - 1));
	y = (int)(rec->v * (obj->texture->height - 1));
	tmp = convert_int_to_rgb(*(unsigned int *)(obj->texture->addr + \
		(obj->texture->line_length * y) + \
		(x * obj->texture->bits_per_pixel >> 3)));
	tmp = vec_div_double(tmp, 255);
	return (tmp);
}
