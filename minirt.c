/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minirt.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hchang <hchang@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/13 15:59:19 by jinypark          #+#    #+#             */
/*   Updated: 2022/10/18 16:07:51 by hchang           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"

void	my_mlx_pixel_put(t_img *img, int x, int y, t_color color)
{
	char	*dst;

	dst = img->addr + y * img->line_length + x * (img->bits_per_pixel >> 3);
	*(unsigned int *)dst = convert_color(color);
}

void	key_press_move2(int keycode, t_info *info)
{
	if (keycode == KEY_DOWN)
	{
		printf(C_YELLOW"[Camera] Camera origin y -= 0.1");
		info->camera->orig.y -= 0.1;
	}
	else if (keycode == KEY_W)
	{
		printf(C_YELLOW"[Camera] Camera origin z += 0.1");
		info->camera->orig.z += 0.1;
	}
	else if (keycode == KEY_S)
	{
		printf(C_YELLOW"[Camera] Camera origin z -= 0.1");
		info->camera->orig.z -= 0.1;
	}
}

void	key_press_move(int keycode, t_info *info)
{
	if (keycode == KEY_D)
	{
		printf(C_YELLOW"[Camera] Camera origin x += 0.1");
		info->camera->orig.x += 0.1;
	}
	else if (keycode == KEY_A)
	{
		printf(C_YELLOW"[Camera] Camera origin x -= 0.1");
		info->camera->orig.x -= 0.1;
	}
	else if (keycode == KEY_UP)
	{
		printf(C_YELLOW"[Camera] Camera origin y += 0.1");
		info->camera->orig.y += 0.1;
	}
	else if (keycode == KEY_DOWN || keycode == KEY_W || keycode == KEY_S)
		key_press_move2(keycode, info);
	printf("   camera origin x : %.2lf | y : %.2lf | z : %.2lf\n"C_RESET, \
			info->camera->orig.x, info->camera->orig.y, info->camera->orig.z);
	ft_render(info, &info->mlx);
}

int	key_press(int keycode, void *param)
{
	t_info	*info;

	info = param;
	if (keycode == KEY_ESC)
	{
		printf(C_RED"[The End]\n"C_RESET);
		exit(0);
	}
	else if (keycode == KEY_C)
	{
		printf(C_CYAN"[Camera Changed]\n");
		info->camera = info->camera->next;
		printf("   camera origin x : %.2lf | y : %.2lf | z : %.2lf\n"C_RESET, \
			info->camera->orig.x, info->camera->orig.y, info->camera->orig.z);
		ft_render(info, &info->mlx);
	}
	else if (keycode == KEY_E || keycode == KEY_R)
		change_mode(keycode, info);
	else if (keycode == KEY_D || keycode == KEY_A
		|| keycode == KEY_S || keycode == KEY_W
		|| keycode == KEY_UP || keycode == KEY_DOWN)
		key_press_move(keycode, info);
	return (0);
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
