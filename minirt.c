#include "minirt.h"

void	info_init(t_info *info, char *file)
{
	char		**split;
	char		*line;
	int			fd;

	fd = open(file, O_RDONLY);
	if (fd < 0)
		ft_strerror("Error : fail to open file");
	line = get_next_line(fd);
	if (line == NULL)
		ft_strerror("Error : attempt to open empty file");
	while (line)
	{
		if (line[0] == '#')
		{
			free(line);
			line = get_next_line(fd);
			continue ;
		}
		split = ft_split(line, ' ');
		if (split == NULL)
			ft_strerror("Error : split fail");
		put_info(info, split);
		split_free(split);
		free(line);
		line = get_next_line(fd);
	}
	if (!info->light || !info->ambient || !info->camera)
		ft_strerror("invalid .rt file");
}

void  my_mlx_pixel_put(t_img *img, int x, int y, t_color color)
{
	char	*dst;

	dst = img->addr +  y * img->line_length + x * (img->bits_per_pixel / 8);
	*(unsigned int *)dst = convert_color(color);
}

void ft_render(t_info *info, t_mlx *mlx)
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
	mlx_put_image_to_window(mlx->ptr, mlx->win, mlx->img.img_ptr, 0, 0);
}

void	main_loop(t_info *info, t_mlx *mlx, int key)
{
	(void)info;
	mlx_destroy_image(mlx->ptr, mlx->img.img_ptr);
	mlx_clear_window(mlx->ptr, mlx->win);
	mlx->img.img_ptr = mlx_new_image(mlx->ptr, WIN_W, WIN_H);
	mlx->img.addr = mlx_get_data_addr(mlx->img.img_ptr, \
		&(mlx->img.bits_per_pixel), &(mlx->img.line_length), &(mlx->img.endian));
	if (key == 8)
		info->camera = info->camera->next;
	ft_render(info, mlx);
}

int	key_press(int keycode, void *param)
{
	t_info *info;
	
	info = param;
	if (keycode == KEY_ESC)
		exit(0);
	else if (keycode == 8)
	{
		printf("Camera Changed\n");
		main_loop(info, &info->mlx, keycode);
	}
	return (0);
}

int main(int argc, char **argv)
{
	t_info	info;

	if (argc != 2)
		ft_strerror("Error : invalid argument count(excute)");
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
	mlx_pixel_put(info.mlx.ptr, info.mlx.win, 5,5, 0xFFFFFF);
	mlx_hook(info.mlx.win, EVENT_KEY_PRESS, 0, key_press, &info);
	mlx_loop(info.mlx.ptr);
	return (0);
}