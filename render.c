/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   render.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hchang <hchang@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/17 15:25:42 by hchang            #+#    #+#             */
/*   Updated: 2022/10/18 15:09:31 by hchang           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"

int	destroy_win(void)
{
	printf(C_RED"[The End]\n"C_RESET);
	exit(0);
}

int	get_thread_index(int i, int idx[2])
{
	idx[Y] = (WIN_H / THREAD_I) * i;
	if (i == THREAD_I - 1)
		return (WIN_H - 1);
	else
		return (idx[Y] + (WIN_H / THREAD_I) - 1);
}

void	my_mlx_draw_mode(t_info *info, int idx[3])
{
	t_color	color;

	color = ray_color(info);
	my_mlx_pixel_put(&info->mlx.img, idx[X], (WIN_H - 1 - idx[2]), color);
	if (info->flag & 1)
	{
		my_mlx_pixel_put(&info->mlx.img, idx[X] + 1, \
						(WIN_H - 1 - idx[2]), color);
		my_mlx_pixel_put(&info->mlx.img, idx[X] + 2, \
						(WIN_H - 1 - idx[2]), color);
		my_mlx_pixel_put(&info->mlx.img, idx[X] + 3, \
						(WIN_H - 1 - idx[2]), color);
		idx[X] += 3;
	}
}

void	*render_thread(void *param)
{
	int			idx[3];
	double		vdx[2];
	t_info		*info;

	info = (t_info *)param;
	idx[2] = get_thread_index(info->idx, idx);
	while (idx[2] >= idx[Y])
	{
		idx[X] = 0;
		while (idx[X] < WIN_W)
		{
			vdx[U] = (double)idx[X] / (WIN_W - 1);
			vdx[V] = (double)idx[2] / (WIN_H - 1);
			ray_primary(&info->ray, info->camera, vdx[U], vdx[V]);
			my_mlx_draw_mode(info, idx);
			++idx[X];
		}
		--idx[2];
	}
	return (NULL);
}

void	ft_render(t_info *info, t_mlx *mlx)
{
	int			i;
	pthread_t	tid[THREAD_I];
	t_info		*tmp_info;

	i = 0;
	tmp_info = ft_calloc(THREAD_I, sizeof(t_info));
	while (i < THREAD_I)
	{
		tmp_info[i] = *info;
		tmp_info[i].idx = i;
		if (!(info->flag & 2))
			pthread_create(&tid[i], NULL, render_thread, (void *)&tmp_info[i]);
		else
			pthread_create(&tid[i], NULL, \
				render_thread_anti, (void *)&tmp_info[i]);
		++i;
	}
	i = 0;
	while (i < THREAD_I)
	{
		pthread_join(tid[i], NULL);
		++i;
	}
	free(tmp_info);
	mlx_put_image_to_window(mlx->ptr, mlx->win, mlx->img.img_ptr, 0, 0);
}
