/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minirt.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hchang <hchang@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/13 15:59:19 by jinypark          #+#    #+#             */
/*   Updated: 2022/10/17 15:20:03 by hchang           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"
#include <pthread.h>

void	my_mlx_pixel_put(t_img *img, int x, int y, t_color color)
{
	char	*dst;

	dst = img->addr + y * img->line_length + x * (img->bits_per_pixel >> 3);
	*(unsigned int *)dst = convert_color(color);
}

int	get_thread_index(int i, int idx[2])
{
	idx[Y] = (WIN_H / THREAD_I) * i;
	if (i == THREAD_I - 1)
		return (WIN_H - 1);
	else
		return (idx[Y] + (WIN_H / THREAD_I) - 1);
}

void *render_thread(void *param)
{
	int			idx[2];
	double		vdx[2];
	t_info		*info;
	int			end_idx;
	t_color		color;
	
	info = (t_info *)param;
	end_idx = get_thread_index(info->idx, idx);
	while (end_idx >= idx[Y])
	{
		idx[X] = 0;
		while (idx[X] < WIN_W)
		{
			vdx[U] = (double)idx[X] / (WIN_W - 1);
			vdx[V] = (double)end_idx / (WIN_H - 1);
			ray_primary(&info->ray, info->camera, vdx[U], vdx[V]);
			color = ray_color(info);
			my_mlx_pixel_put(&info->mlx.img, idx[X], (WIN_H - 1 - end_idx), color);
			if (info->flag & 1)
			{
				my_mlx_pixel_put(&info->mlx.img, idx[X] + 1, (WIN_H - 1 - end_idx), color);
				my_mlx_pixel_put(&info->mlx.img, idx[X] + 2, (WIN_H - 1 - end_idx), color);
				my_mlx_pixel_put(&info->mlx.img, idx[X] + 3, (WIN_H - 1 - end_idx), color);
				idx[X] += 3;
			}
			++idx[X];
		}
		--end_idx;
	}
	return(NULL);
}

void	ft_render(t_info *info, t_mlx *mlx)
{
	int			i;
	pthread_t	tid[THREAD_I];
	t_info		*tmp_info;

	tmp_info = ft_calloc(THREAD_I, sizeof(t_info));
	i = 0;
	while (i < THREAD_I)
	{
		tmp_info[i] = *info;
		tmp_info[i].idx = i;
		pthread_create(&tid[i], NULL, render_thread, (void *)&tmp_info[i]);
		++i;
	}
	i = 0;
	while(i < THREAD_I)
	{
		pthread_join(tid[i], NULL);
		++i;
	}
	mlx_put_image_to_window(mlx->ptr, mlx->win, mlx->img.img_ptr, 0, 0);
}

void	main_loop(t_info *info, t_mlx *mlx, int key)
{
	if (key == 8)
		info->camera = info->camera->next;
	ft_render(info, mlx);
}

int	key_press(int keycode, void *param)
{
	t_info	*info;

	info = param;
	if (keycode == KEY_ESC)
		exit(0);
	else if (keycode == KEY_C)
	{
		printf("Camera Changed\n");
		main_loop(info, &info->mlx, keycode);
	}
	else if (keycode == KEY_R)
	{
		printf("EDIT MODE\n");
		info->flag = ~info->flag;
		main_loop(info, &info->mlx, keycode);
	}
	else if (keycode == KEY_D)
	{
		printf("D clicked\n");
		info->camera->orig.x += 0.1;
		main_loop(info, &info->mlx, keycode);
	}
	else if (keycode == KEY_A)
	{
		printf("A clicked\n");
		info->camera->orig.x -= 0.1;
		main_loop(info, &info->mlx, keycode);
	}
	else if (keycode == KEY_W)
	{
		printf("W clicked\n");
		info->camera->orig.z -= 0.1;
		main_loop(info, &info->mlx, keycode);
	}
	else if (keycode == KEY_S)
	{
		printf("S clicked\n");
		info->camera->orig.z += 0.1;
		main_loop(info, &info->mlx, keycode);
	}
	return (0);
}

int	destroy_win(void)
{
	printf("The End\n");
	exit(0);
}

int	main(int argc, char **argv)
{
	t_info	info;

	if (argc != 2)
		ft_strerror("Error \ninvalid argument count(excute)");
	ft_memset(&info, 0, sizeof(t_info));
	info.mlx.ptr = mlx_init();
	info.mlx.win = mlx_new_window(info.mlx.ptr, WIN_W, WIN_H, "HojinyRT");
	info.mlx.img.img_ptr = mlx_new_image(info.mlx.ptr, IMG_W, IMG_H);
	info.mlx.img.addr = mlx_get_data_addr(info.mlx.img.img_ptr, \
											&(info.mlx.img.bits_per_pixel), \
											&(info.mlx.img.line_length), \
											&(info.mlx.img.endian));
	info_init(&info, argv[1]);
	ft_render(&info, &info.mlx);
	mlx_hook(info.mlx.win, KEY_EXIT, 0, &destroy_win, 0);
	mlx_hook(info.mlx.win, EVENT_KEY_PRESS, 0, key_press, &info);
	mlx_loop(info.mlx.ptr);
	return (0);
}
