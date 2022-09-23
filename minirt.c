#include "minirt.h"

void    obj_add(t_object **list, t_object *new)
{
    t_object    *cur;

    if (list == NULL)
        return ;
    if (*list == NULL)
    {
        *list = new;
        return ;
    }
    cur = *list;
    while (cur->next)
        cur = cur->next;
    cur->next = new;
}

void    light_add(t_light **list, t_light *new)
{
    t_light    *cur;

    if (list == NULL)
        return ;
    if (*list == NULL)
    {
        *list = new;
        return ;
    }
    cur = *list;
    while (cur->next)
        cur = cur->next;
    cur->next = new;
}

void	is_sign(char *str, int *idx, double *sign)
{
	*idx = -1;
	*sign = 1.0;
	if (str[++(*idx)] == '+' || str[*idx] == '-')
	{
		if (str[*idx] == '-')
			*sign *= -1.0;
        return ;
	}
    (*idx)--;
}

double	ft_atod(char *str)
{
	int		idx;
	int		flag;
	double	sign;
	double	res;
	double	decimal;
	
	flag = 0;
    res = 0.0;
	is_sign(str, &idx, &sign);
	while (str[++idx])
	{
		if ((flag++ == 0 && str[idx] == '.') || ((!ft_isdigit(str[idx]) && str[idx] != '.')))
			exit(1);
		if (str[idx] == '.')
		{
			idx++;
			break ;
		}
		res = (str[idx] - '0') + (res * 10);
		if ((sign == 1 &&res > 2147483647) || (sign == -1 && res > 2147483648))
			exit(2);
	}
	flag = 0;
	decimal = 0.1;
    if (str[idx - 1] == '.' && !str[idx])
        exit(2);
	while(str[idx])
	{
		if (!ft_isdigit(str[idx]))
			exit(3);
		if (flag++ > 6)
        {
            idx++;
			continue ;
        }
		res = res + (str[idx] - '0') * decimal;
		decimal /= 10;
		idx++;
	}
    if ((sign == -1 && res > 2147483648) || (sign == 1 && res > 2147483647))
        exit(10);
	return (sign * res);
}

int	check_unit(double x, double y, double z, int flag)
{
	if (flag == 1)
	{
		if (x > 1 || x < -1 || y > 1 || y < -1 || z > 1 || z < -1)
			exit(1);
	}
	else if (flag == 2)
	{
		if (x > 255 || x < 0 || y > 255 || y < 0 || z > 255 || z < 0)
			exit(1);
	}
	else if (flag == 3)
	{
		if (x > 2147483647 || x < -2147483648 || y > 2147483647 \
		|| y < -2147483648 || z > 2147483647 || z < -2147483648)
			exit(1);
	}
}

t_vec	ft_atovec(char *str, int flag)
{
	t_vec	res;
	char	**vec;
	int		i;
	int		cnt;

	i = -1;
	while (str[++i])
	{
		if (str[i] == ',')
			cnt++;
	}
	if (cnt != 2)
		exit(1);
	vec = ft_split(str, ',');
	res.x = ft_atod(vec[0]);
	res.y = ft_atod(vec[1]);
	res.z = ft_atod(vec[2]);
	check_unit(res.x, res.y, res.z, flag);
	return (res);
}

void	put_a(t_info *info, char **argv)
{
	double	brightness;
	t_color	color;

	brightness = ft_atod(argv[1]);
	color = ft_atovec(argv[2], RGB);

	info->ambient = vec_multi_double(color, brightness);
}
void	put_c(t_info *info, char **argv)
{
	t_point	coor;
	t_vec	normal;
	int		fov;

	coor = ft_atovec(argv[1], XYZ);
	normal = ft_atovec(argv[2], UNIT);
	fov = ft_atoi(argv[3]);
	if (fov < 0 || fov > 180)
		exit(1); //에러메시지 출력
	info->camera.orig = coor;
	info->camera.normal = normal;
	info->camera.viewport_w = tan(fov * M_PI / 2) * 2;
	info->camera.viewport_h = info->camera.viewport_w * WIN_H / WIN_W;
	info->camera.horizontal = vec_init(info->camera.viewport_w, 0, 0);
	info->camera.vertical = vec_init(0, info->camera.viewport_h, 0);
	info->camera.start_point = vec_sub(vec_sub(vec_sub(info->camera.orig, vec_div_double(info->camera.horizontal, 2)),
                                vec_div_double(info->camera.vertical, 2)), vec_init(0, 0, 1.0));

}
void	put_l(t_info *info, char **argv)
{
	t_light	*tmp;
	t_vec	origin;
	double	brightness;
	t_color	color;

	origin = ft_atovec(argv[1], XYZ);
	brightness = ft_atod(argv[2]);
	color = ft_atovec(argv[3], RGB);

	tmp = object_init(L, light_point(origin, color, brightness), vec_init(0, 0, 0)); // 더미 albedo
	light_add(&(info->light), tmp);
}

void	put_sp(t_info *info, char **argv)
{
	t_object	*tmp;
	t_vec		origin;
	double		diameter;
	t_color		color;

	origin = ft_atovec(argv[1], XYZ);
	diameter = ft_atod(argv[2]);
	color = ft_atovec(argv[3], RGB);

	tmp = object_init(SP, sphere_init(origin, diameter / 2), vec_div_double(color, 255));
	obj_add(&(info->obj), tmp);
}
void	put_pl(t_info *info, char **argv)
{
	t_object	*tmp;
	t_vec		origin;
	t_vec		normal;
	t_color		color;

	origin = ft_atovec(argv[1], XYZ);
	normal = ft_atovec(argv[2], UNIT);
	color = ft_atovec(argv[3], RGB);

	tmp = object_init(PL, plane_init(origin, normal), vec_div_double(color, 255));
	obj_add(&(info->obj), tmp);
}
void	put_cy(t_info *info, char **argv)
{
	t_object	*tmp;
	t_vec		origin;
	t_vec		normal;
	double		height;
	t_color		color;

	origin = ft_atovec(argv[1], XYZ);
	normal = ft_atovec(argv[2], UNIT);
	height = ft_atod(argv[3]);
	color = ft_atovec(argv[4], RGB);

	tmp = object_init(CY, cylinder_init(origin, normal, height), vec_div_double(color, 255));
	obj_add(&(info->obj), tmp);
}

int		put_info(t_info *info, char **argv)
{
	static void	(*run[6])(t_info *, char **);
	int			type;
	
	type = check_format(argv[0]);
	if (type == -1)
	{
		exit(1);
		// 에러메시지 출력
	}
	run[type](info, argv);
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
		put_info(&info, split);
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