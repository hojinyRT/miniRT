/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   object_init.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jinypark <jinypark@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/13 15:58:19 by jinypark          #+#    #+#             */
/*   Updated: 2022/10/14 20:00:36 by jinypark         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"

t_object	*object_init(t_object_type type, void *element, int checker)
{
	t_object	*new;

	new = (t_object *)my_calloc(1, sizeof(t_object));
	new->type = type;
	new->element = element;
	new->checker = checker;
	new->next = NULL;
	return (new);
}

t_sphere	*sphere_init(t_point center, double radius)
{
	t_sphere	*new;

	new = (t_sphere *)my_calloc(1, sizeof(t_sphere));
	new->center = center;
	new->radius = radius;
	new->radius2 = radius * radius;
	return (new);
}

t_plane	*plane_init(t_point center, t_vec normal, double radius)
{
	t_plane	*new;

	new = (t_plane *)my_calloc(1, sizeof(t_plane));
	new->center = center;
	new->normal = normal;
	new->radius = radius;
	return (new);
}

t_cylinder	*cylinder_init(t_point center, double radius, \
			double height, t_vec normal)
{
	t_cylinder	*new;

	new = (t_cylinder *)my_calloc(1, sizeof(t_cylinder));
	new->center = center;
	new->radius = radius;
	new->radius2 = radius * radius;
	new->height = height;
	new->normal = normal;
	return (new);
}

t_cone	*cone_init(t_point center, double radius, double height, t_vec normal)
{
	t_cone	*new;

	new = (t_cone *)my_calloc(1, sizeof(t_cone));
	new->center = center;
	new->radius = radius;
	new->radius2 = radius * radius;
	new->height = height;
	new->normal = normal;
	return (new);
}
