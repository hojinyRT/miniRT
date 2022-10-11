#include "minirt.h"

void info_init(t_info *info, char *file)
{
	char		**split;
	int			fd;
	char		*line;

	fd = open(file, O_RDONLY);
	if (fd < 0)
		ft_strerror("파일 못열었음");
	line = get_next_line(fd);
	if (line == NULL)
		ft_strerror("읽을 거 없음");
	while(line)
	{
		if (line[0] == '#')
		{
			free(line);
			line = get_next_line(fd);
			continue ;
		}
		split = ft_split(line, ' ');
		if (split == NULL)
			ft_strerror("스플릿 실패(할당 실패)");
		put_info(info, split);
		split_free(split);
		free(line);
		line = get_next_line(fd);
	}
}

void  my_mlx_pixel_put(t_img *img, int x, int y, t_color color)
{
	char	*dst;

	// *(unsigned int *)(img->addr + y * img->line_length + x * (img->bits_per_pixel / 8)) = convert_color(color);
	dst = img->addr +  y * img->line_length + x * (img->bits_per_pixel / 8);
	*(unsigned int *)dst = convert_color(color);

}

void ft_draw(t_info *info, t_mlx *mlx)
{
	int			idx[2];
	double		vdx[2];
	t_color		color;

	idx[Y] = WIN_H - 1;
	while (idx[Y] >= 0)
	{
		idx[X] = 0;
		while (idx[X] < WIN_W)
		{
			vdx[U] = (double)idx[X] / (WIN_W - 1);
			vdx[V] = (double)idx[Y] / (WIN_H - 1);
			ray_primary(&info->ray, info->camera, vdx[U], vdx[V]);
			color = ray_color(info);
			my_mlx_pixel_put(&mlx->img, idx[X], (WIN_H - 1 - idx[Y]), color);
			idx[X]++;
		}
		idx[Y]--;
	}
}

void	main_loop(t_info *info, t_mlx *mlx, int key)
{
	mlx_destroy_image(mlx->ptr, mlx->img.img_ptr);
	mlx_clear_window(mlx->ptr, mlx->win);
	mlx->img.img_ptr = mlx_new_image(mlx->ptr, WIN_W, WIN_H);
	mlx->img.addr = mlx_get_data_addr(mlx->img.img_ptr, \
		&(mlx->img.bits_per_pixel), &(mlx->img.line_length), &(mlx->img.endian));
	if (key == 8)
		info->camera = info->camera->next;
	ft_draw(info, mlx);
	mlx_put_image_to_window(mlx->ptr, mlx->win, mlx->img.img_ptr, 0, 0);
}

int	key_press(int keycode, void *param)
{
	t_info *const	info = param;

	if (keycode == KEY_ESC)
		exit(0);
	else if (keycode == 8)
	{
		printf("C clicked\n");
		main_loop(info, &info->mlx, keycode);
	}
	return (0);
}

int main(int argc, char **argv)
{
	t_info	info;

	// atexit(ae);
	if (argc != 2)
		ft_strerror("인자 잘못넣음");
	ft_memset(&info, 0, sizeof(t_info));
	info.mlx.ptr = mlx_init();
	info.mlx.win = mlx_new_window(info.mlx.ptr, WIN_W, WIN_H, "HojinySesiMinsukiR2");

	info.mlx.img.img_ptr = mlx_new_image(info.mlx.ptr, IMG_W, IMG_H);

	info.mlx.img.addr = mlx_get_data_addr(info.mlx.img.img_ptr, \
											&(info.mlx.img.bits_per_pixel), \
											&(info.mlx.img.line_length), \
											&(info.mlx.img.endian));
	info_init(&info, argv[1]);
	// print_obj(info.obj);
	ft_draw(&info, &info.mlx);
	mlx_put_image_to_window(info.mlx.ptr, info.mlx.win, info.mlx.img.img_ptr, 0, 0);
	mlx_hook(info.mlx.win, EVENT_KEY_PRESS, 0, key_press, &info);
	mlx_loop(info.mlx.ptr);
	return (0);
}