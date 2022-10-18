/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   anti.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hchang <hchang@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/17 16:51:08 by hchang            #+#    #+#             */
/*   Updated: 2022/10/18 14:52:51 by hchang           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"

void	change_mode(int keycode, t_info *info)
{
	if (keycode == KEY_E)
	{
		printf(C_GREEN"[EDIT MODE]\n"C_RESET);
		info->flag &= 0B01;
		info->flag ^= 0B01;
		ft_render(info, &info->mlx);
	}
	else if (keycode == KEY_R)
	{
		printf(C_MAGENTA"[ANTI ALLIASING MODE]\n"C_RESET);
		info->flag &= 0B10;
		info->flag ^= 0B10;
		ft_render(info, &info->mlx);
	}
}

double	random_double(void)
{
	return ((double)rand() / (RAND_MAX + 1.0));
}

t_color	antialliasing(t_info *info, int idx[3])
{
	t_color	color;
	double	vdx[2];
	int		sample;

	sample = 0;
	color = vec_init(0, 0, 0);
	while (sample < SAMPLE_PER_PIXEL)
	{
		vdx[U] = ((double)idx[X] + random_double()) / (WIN_W - 1);
		vdx[V] = ((double)idx[2] + random_double()) / (WIN_H - 1);
		ray_primary(&info->ray, info->camera, vdx[U], vdx[V]);
		color = vec_add(color, ray_color(info));
		++sample;
	}
	return (vec_div_double(color, SAMPLE_PER_PIXEL));
}

void	*render_thread_anti(void *param)
{
	int			idx[3];
	t_info		*info;
	t_color		color;
	int			sample;

	info = (t_info *)param;
	sample = 0;
	idx[2] = get_thread_index(info->idx, idx);
	while (idx[2] >= idx[Y])
	{
		idx[X] = 0;
		while (idx[X] < WIN_W)
		{
			color = antialliasing(info, idx);
			my_mlx_pixel_put(&info->mlx.img, idx[X], \
							(WIN_H - 1 - idx[2]), color);
			++idx[X];
		}
		--idx[2];
	}
	return (NULL);
}
