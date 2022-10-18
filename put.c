/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   put.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hchang <hchang@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/13 15:56:22 by jinypark          #+#    #+#             */
/*   Updated: 2022/10/18 14:49:41 by hchang           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"

void	put_sp(t_info *info, char **argv, int argc)
{
	t_object	*new;
	t_sphere	sp;
	int			checker;

	if (argc == 4 || argc == 5)
	{
		checker = (ft_strlen(argv[0]) == 5);
		sp.center = ft_atovec(argv[1], XYZ);
		sp.radius = ft_atod(argv[2]) / 2;
		new = object_init(SP, sphere_init(sp.center, sp.radius), checker);
		new->color = vec_div_double(ft_atovec(argv[3], RGB), 255);
		if (argc == 5)
			get_image_addr(new, &info->mlx, argv[4]);
		obj_add(&(info->obj), new);
	}
	else
		ft_strerror("Error \ninvalid argument count(sphere)");
}

void	put_pl(t_info *info, char **argv, int argc)
{
	t_object	*new;
	t_plane		pl;
	int			checker;

	if (argc == 4 || argc == 5)
	{
		checker = (ft_strlen(argv[0]) == 5);
		pl.center = ft_atovec(argv[1], XYZ);
		pl.normal = ft_atovec(argv[2], UNIT);
		new = object_init(PL, plane_init(pl.center, pl.normal, 0), checker);
		new->color = vec_div_double(ft_atovec(argv[3], RGB), 255);
		if (argc == 5)
			get_image_addr(new, &info->mlx, argv[4]);
		obj_add(&(info->obj), new);
	}
	else
		ft_strerror("Error \ninvalid argument count(plane)");
}

void	put_cap(t_object **new, t_cylinder *obj, int type)
{
	t_object	*new_cap;

	new_cap = object_init(CAP, plane_init(get_cap_point(obj->center, \
	obj->height, obj->normal, -1), obj->normal, obj->radius), (*new)->checker);
	new_cap->color = (*new)->color;
	obj_add(&((t_cylinder *)(*new)->element)->cap, new_cap);
	if (type == CY)
	{
		new_cap = object_init(CAP, plane_init(get_cap_point(obj->center, \
		obj->height, obj->normal, 1), obj->normal, obj->radius), \
		(*new)->checker);
		new_cap->color = (*new)->color;
		obj_add(&((t_cylinder *)(*new)->element)->cap, new_cap);
	}
}

void	put_cy(t_info *info, char **argv, int argc)
{
	t_object	*new;
	t_cylinder	cy;
	int			checker;

	if (argc == 6 || argc == 7)
	{
		checker = (ft_strlen(argv[0]) == 5);
		cy.center = ft_atovec(argv[1], XYZ);
		cy.normal = ft_atovec(argv[2], UNIT);
		cy.radius = ft_atod(argv[3]) / 2;
		cy.height = ft_atod(argv[4]);
		new = object_init(CY, cylinder_init(get_cap_point(cy.center, \
		cy.height, cy.normal, -1), cy.radius, cy.height, cy.normal), checker);
		new->color = vec_div_double(ft_atovec(argv[5], RGB), 255);
		if (argc == 7)
			get_image_addr(new, &info->mlx, argv[6]);
		obj_add(&(info->obj), new);
		put_cap(&new, &cy, CY);
	}
	else
		ft_strerror("Error \ninvalid argument count(cylinder)");
}

void	put_cn(t_info *info, char **argv, int argc)
{
	t_object	*new;
	t_cone		cn;
	int			checker;

	if (argc == 6 || argc == 7)
	{
		checker = (ft_strlen(argv[0]) == 5);
		cn.center = ft_atovec(argv[1], XYZ);
		cn.normal = ft_atovec(argv[2], UNIT);
		cn.radius = ft_atod(argv[3]) / 2;
		cn.height = ft_atod(argv[4]);
		new = object_init(CN, cone_init(get_cap_point(cn.center, cn.height, \
					cn.normal, -1), cn.radius, cn.height, cn.normal), checker);
		new->color = vec_div_double(ft_atovec(argv[5], RGB), 255);
		if (argc == 7)
			get_image_addr(new, &info->mlx, argv[6]);
		obj_add(&(info->obj), new);
		put_cap(&new, &cn, CN);
	}
	else
		ft_strerror("Error \ninvalid argument count(cone)");
}
