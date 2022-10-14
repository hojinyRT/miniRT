/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   scene.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jinypark <jinypark@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/13 15:54:51 by jinypark          #+#    #+#             */
/*   Updated: 2022/10/14 20:00:36 by jinypark         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"

void	light_add(t_light **list, t_light *new)
{
	t_light	*cur;

	if (list == NULL)
		return ;
	if (*list == NULL)
	{
		*list = new;
		return ;
	}
	cur = *list;
	while (cur->next)
		cur = cur->next;
	cur->next = new;
}

t_camera	*camera_init(t_point coor, t_vec normal, int fov)
{
	t_camera	*new;

	new = (t_camera *)my_calloc(1, sizeof(t_camera));
	new->orig = coor;
	new->normal = normal;
	new->viewport_w = tan((double)fov / 2 * M_PI / 180) * 2;
	new->viewport_h = new->viewport_w * WIN_H / WIN_W;
	if ((normal.x == 0 && normal.y == 1 && normal.z == 0))
		new->horizontal = vec_multi_double(vec_unit(vec_cross(normal, \
							vec_init(0, 0, 1))), new->viewport_w);
	else if ((normal.x == 0 && normal.y == -1 && normal.z == 0))
		new->horizontal = vec_multi_double(vec_unit(vec_cross(normal, \
							vec_init(0, 0, -1))), new->viewport_w);
	else
		new->horizontal = vec_multi_double(vec_unit(vec_cross(normal, \
							vec_init(0, 1, 0))), new->viewport_w);
	new->vertical = vec_multi_double(vec_unit(vec_cross(new->horizontal, \
						normal)), new->viewport_h);
	new->start_point = vec_sub(vec_sub(vec_sub(new->orig, \
						vec_div_double(new->horizontal, 2)), \
						vec_div_double(new->vertical, 2)), \
						(vec_multi_double(normal, -1)));
	return (new);
}

void	camera_add(t_camera **list, t_camera *new)
{
	t_camera	*cur;

	if (list == NULL)
		return ;
	if (*list == NULL)
	{
		*list = new;
		(*list)->next = *list;
		return ;
	}
	cur = *list;
	while (cur->next && cur->next != *list)
		cur = cur->next;
	cur->next = new;
	new->next = *list;
}
