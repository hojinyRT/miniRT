/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   texture.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hchang <hchang@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/13 15:51:48 by jinypark          #+#    #+#             */
/*   Updated: 2022/10/18 14:50:16 by hchang           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"

void	get_texture_addr(t_object *obj, t_mlx *mlx)
{
	int		format[2];
	int		idx[2];
	char	*texture;

	ft_bzero(idx, sizeof(idx));
	texture = ft_strjoin("texture_", obj->bump->file_name);
	obj->texture->img_ptr = mlx_png_file_to_image(mlx->ptr, \
		texture, &format[0], &format[1]);
	if (!obj->texture->img_ptr)
	{
		free(texture);
		return ;
	}
	obj->texture->addr = mlx_get_data_addr(obj->texture->img_ptr, \
											&(obj->texture->bits_per_pixel), \
											&(obj->texture->line_length), \
											&(obj->texture->endian));
	obj->texture->width = format[0];
	obj->texture->height = format[1];
	free(texture);
}

void	get_bump_addr(t_object *obj, t_mlx *mlx)
{
	int		format[2];
	int		idx[2];

	ft_bzero(idx, sizeof(idx));
	obj->bump->img_ptr = mlx_png_file_to_image(mlx->ptr, obj->bump->file_name, \
							&format[0], &format[1]);
	if (!obj->bump->img_ptr)
		ft_strerror("Error \nno such file or directory");
	obj->bump->addr = mlx_get_data_addr(obj->bump->img_ptr, \
											&(obj->bump->bits_per_pixel), \
											&(obj->bump->line_length), \
											&(obj->bump->endian));
	obj->bump->width = format[0];
	obj->bump->height = format[1];
}

void	get_image_addr(t_object *new, t_mlx *mlx, char *file)
{
	new->bump = my_calloc(1, sizeof(t_img));
	new->bump->file_name = ft_strdup(file);
	get_bump_addr(new, mlx);
	new->texture = my_calloc(1, sizeof(t_img));
	get_texture_addr(new, mlx);
}

t_color	checkerboard_value(t_hit_record rec)
{
	const int		width = 10;
	const int		height = 10;
	const double	u2 = floor(rec.u * width);
	const double	v2 = floor(rec.v * height);

	if (fmod(u2 + v2, 2.) == 0)
		return (rec.color);
	return (vec_init(1, 1, 1));
}

t_vec	bump_normal(t_object *obj, t_hit_record *rec)
{
	int		format[2];
	t_vec	tmp;
	t_vec	ul;
	t_vec	vl;
	t_vec	zl;

	format[X] = (int)(rec->u * (double)(obj->bump->width - 1));
	format[Y] = (int)(rec->v * (double)(obj->bump->height - 1));
	tmp = convert_color_to_normal(*(unsigned int *)(obj->bump->addr + \
			obj->bump->line_length * format[Y] + \
			format[X] * obj->bump->bits_per_pixel / 8));
	ul = vec_multi_double(rec->e1, tmp.x);
	vl = vec_multi_double(rec->e2, tmp.y);
	zl = vec_multi_double(rec->normal, tmp.z);
	tmp = vec_add(vec_add(ul, vl), zl);
	tmp = vec_unit(tmp);
	return (tmp);
}
