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

t_vec	convert_color_to_normal(int	color)
{
	t_vec	res;

	res.x = color >> 16 & 0xFF;
	res.y = color >> 8 & 0xFF;
	res.z = color & 0xFF;
	if ((res.x == 0 && res.y == 0 && res.z == 0))
		ft_strerror("범프맵 이상함");
	res = vec_div_double(res, 255);
	res = vec_sub(vec_multi_double(res, 2), vec_init(1, 1, 1));
	return (res);
}

int	convert_color(t_vec clr)
{
	int tmp = ((int)clr.x * 16 * 16 * 16 * 16) + ((int)clr.y * 16 * 16) + (int)(clr.z);
	return (tmp);
}

void  my_mlx_pixel_put(t_img *img, int x, int y, t_color color)
{
	char	*dst;

	// *(unsigned int *)(img->addr + y * img->line_length + x * (img->bits_per_pixel / 8)) = convert_color(color);
	dst = img->addr +  y * img->line_length + x * (img->bits_per_pixel / 8);
	*(unsigned int *)dst = convert_color(color);

}

void	ray_primary(t_ray *ray, t_camera *cam, double u, double v)
{
    ray->orig = cam->orig;
    ray->dir =  vec_unit(vec_sub(vec_add(vec_add(cam->start_point, \
							vec_multi_double(cam->horizontal, u)), \
							vec_multi_double(cam->vertical, v)), cam->orig));

}

t_color    ray_color(t_info *info)
{
    double			t;

	record_init(&(info->rec));
	if (hit(info->obj, info->ray, &(info->rec)))
	{
		return (phong_lighting(info));
	}
	else
	{
		t = 0.5 * (info->ray.dir.y + 1.0);
		return (vec_add(vec_multi_double(vec_init(255, 255, 255), 1.0 - t), vec_multi_double(vec_init(128, 178, 255), t)));
	}
}

t_ray	ray_init(t_point orig, t_vec dir)
{
	t_ray init;

	init.orig = orig;
	init.dir = vec_unit(dir);
	return (init);
}

void    set_face_normal(t_ray ray, t_hit_record *rec)
{
    rec->front_face = vec_dot(ray.dir, rec->normal) < 0;
	if (rec->front_face == 0)
		rec->normal = vec_multi_double(rec->normal, -1);
	//front_face를 현재 쓰는 곳이 없음 리팩토링할 때 판단하기 바람.
    return ;
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
	ft_draw(&info, &info.mlx);
	mlx_put_image_to_window(info.mlx.ptr, info.mlx.win, info.mlx.img.img_ptr, 0, 0);
	mlx_hook(info.mlx.win, EVENT_KEY_PRESS, 0, key_press, &info);
	mlx_loop(info.mlx.ptr);
	return (0);
}