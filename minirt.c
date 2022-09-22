#include "minirt.h"

int		put_info(t_info *info, int type)
{
	if (type == -1)
		exit(1);
	if (type == A)
	{
		
	}
	if (type == C);
	if (type == L);
	if (type == SP);
	if (type == PL);
	if (type == CY);
		
}

t_info	info_init(t_info info, char *file)
{
	char		**split;
	int			fd;
	char		*line;
	double		ka;

	fd = open(file, O_RDONLY);
	if (fd < 0)
		exit(1);
	line = get_next_line(fd);
	if (line == NULL)
		exit(1);
	while(line)
	{
		split = ft_split(line, ' ');
		if (split == NULL)
			exit(1);
		put_info(&info, check_format(split[0]));
		free(line);
		line = get_next_line(fd);
	}
	info.canvas = canvas_init(WIN_W, WIN_H);
    // info.camera = camera_init(info.canvas, vec_init(0, 0 ,0));
	// obj = object_init(SP, sphere_init(vec_init(-2, 0, -5), 2), vec_init(0.5, 0, 0));
	
	// obj_add(&obj, object_init(SP, sphere_init(vec_init(0, -1000, 0), 995), vec_init(1, 1, 1)));
	// obj_add(&obj, object_init(SP, sphere_init(vec_init(2, 0, -5), 2), vec_init(0, 0.5, 0)));
	// // obj_add(&obj, object_init(SP, sphere_init(vec_init(0, 0, -5), 1), vec_init(1, 1, 1)));
	// info.obj = obj;
	// // light = object_init(LIGHT_POINT, light_point(vec_init(0, 20, 5), vec_init(1, 1, 1), 0.5), vec_init(0, 0, 0));
	// light = object_init(LIGHT_POINT, light_point(vec_init(0, 20, 0), vec_init(1, 1, 1), 0.5), vec_init(0, 0, 0)); // 더미 albedo

	// info.light = light;
	// ka = 0.1;
	// info.ambient = vec_multi_double(vec_init(1, 1, 1), ka);
	return (info);
}

int	key_press(int keycode)
{
	if (keycode == KEY_ESC)
		exit(0);
	return (0);
}

int main(int argc, char **argv)
{
	t_info	info;
	t_mlx	mlx;

	if (argc != 2)
		return (0); // 에러메시지 출력 후 종료
	info = info_init(info, argv[1]);
	mlx.ptr = mlx_init();
	mlx.win = mlx_new_window(mlx.ptr, WIN_W, WIN_H, "HojinyRT");
	mlx.img.img_ptr = mlx_new_image(mlx.ptr, IMG_W, IMG_H);
	mlx.img.addr = (int *)mlx_get_data_addr(mlx.img.img_ptr, \
		&(mlx.img.bits_per_pixel), &(mlx.img.line_length), &(mlx.img.endian));
	mlx_hook(mlx.win, EVENT_KEY_PRESS, 0, key_press, 0);
	mlx_loop(mlx.ptr);
	return (0);
}