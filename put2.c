/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   put2.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hchang <hchang@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/13 15:56:13 by jinypark          #+#    #+#             */
/*   Updated: 2022/10/18 16:00:41 by hchang           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"

t_point	get_cap_point(t_point center, double height, t_vec normal, double sign)
{
	t_vec	ccprime;

	ccprime = vec_multi_double(vec_multi_double(normal, sign), height / 2);
	return (vec_add(center, ccprime));
}

void	obj_add(t_object **list, t_object *new)
{
	t_object	*cur;

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

void	put_a(t_info *info, char **argv, int argc)
{
	double		brightness;
	static int	count_a;
	t_color		*color;

	if (count_a > 0)
		ft_strerror("Error \nmultiple ambients in .rt file");
	if (argc != 3)
		ft_strerror("Error \ninvalid argument count(ambient)");
	brightness = ft_atod(argv[1]);
	color = my_calloc(1, sizeof(t_color));
	*color = vec_div_double(ft_atovec(argv[2], RGB), 255);
	*color = vec_multi_double(*color, brightness);
	info->ambient = color;
	++count_a;
}

void	put_c(t_info *info, char **argv, int argc)
{
	t_camera	*new;
	t_point		coor;
	t_vec		normal;
	int			fov;

	if (argc != 4)
		ft_strerror("Error \ninvalid argument count(camera)");
	coor = ft_atovec(argv[1], XYZ);
	normal = ft_atovec(argv[2], UNIT);
	fov = my_atoi(argv[3]);
	if (fov < 0 || fov > 180)
		ft_strerror("Error \ninvalid fov");
	new = camera_init(coor, normal, fov);
	camera_add(&(info->camera), new);
}

void	put_l(t_info *info, char **argv, int argc)
{
	t_light	*new;
	t_vec	origin;
	double	brightness;
	t_color	color;

	if (argc != 4)
		ft_strerror("Error \ninvalid argument count(light)");
	origin = ft_atovec(argv[1], XYZ);
	brightness = ft_atod(argv[2]);
	color = vec_div_double(ft_atovec(argv[3], RGB), 255);
	new = light_init(origin, color, brightness);
	light_add(&(info->light), new);
}
