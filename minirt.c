
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

void	put_a(t_info *info, char **argv)
{
	double	brightness;
	t_color	color;

	brightness = ft_atod(argv[1]);
	color = vec_div_double(ft_atovec(argv[2], RGB), 255);
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
		ft_strerror("카메라 앵글 잘못됨"); //에러메시지 출력
	info->camera.orig = coor;
	info->camera.normal = normal;
	info->camera.viewport_w = tan((double)fov / 2 * M_PI / 180) * 2;
	info->camera.viewport_h = info->camera.viewport_w * WIN_H / WIN_W;
	info->camera.horizontal = vec_init(info->camera.viewport_w, 0, 0);
	info->camera.vertical = vec_init(0, info->camera.viewport_h, 0);
	info->camera.start_point = vec_sub(vec_sub(vec_sub(info->camera.orig, vec_div_double(info->camera.horizontal, 2)),
                                vec_div_double(info->camera.vertical, 2)), normal);

}
void	put_l(t_info *info, char **argv)
{
	t_light	*tmp;
	t_vec	origin;
	double	brightness;
	t_color	color;

	origin = ft_atovec(argv[1], XYZ);
	brightness = ft_atod(argv[2]);
	color = vec_div_double(ft_atovec(argv[3], RGB), 255);

	tmp = light_init(origin, color, brightness); // 더미 albedo
	light_add(&(info->light), tmp);
}

void	put_sp(t_info *info, char **argv)
{
	t_object	*tmp;
	t_vec		origin;
	double		radius;
	t_color		color;

	origin = ft_atovec(argv[1], XYZ);
	radius = ft_atod(argv[2]) / 2;
	color = ft_atovec(argv[3], RGB);

	tmp = object_init(SP, sphere_init(origin, radius), vec_div_double(color, 255));
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
	color = ft_atovec(argv[4], RGB);

	tmp = object_init(PL, plane_init(origin, normal), vec_div_double(color, 255));
	obj_add(&(info->obj), tmp);
}
void	put_cy(t_info *info, char **argv)
{
	t_object	*tmp;
	t_vec		origin;
	t_vec		normal;
	double		format[2];
	t_color		color;

	origin = ft_atovec(argv[1], XYZ);
	normal = ft_atovec(argv[2], UNIT);
	format[0] = ft_atod(argv[3]) / 2;
	format[1] = ft_atod(argv[4]);
	color = ft_atovec(argv[5], RGB);
	tmp = object_init(CY, cylinder_init(origin, format[0], format[1], normal), vec_div_double(color, 255));
	obj_add(&(info->obj), tmp);
}

int 	check_format(char *format)
{
	if (!format)
		return (-1);
	if (!ft_strncmp(format, "sp", 3))
		return (SP);
	else if (!ft_strncmp(format, "pl", 3))
		return (PL);
	else if (!ft_strncmp(format, "cy", 3))
		return (CY);
	else if (!ft_strncmp(format, "A", 2))
		return (A);
	else if (!ft_strncmp(format, "C", 2))
		return (C);
	else if (!ft_strncmp(format, "L", 2)) //맨대토리 따로 작성 L
		return (L);
	else
		ft_strerror("포맷 잘못받음");
	return (-1);
}

void	put_info(t_info *info, char **argv)
{
	static void	(*run[6])(t_info *, char **) = {put_a, put_c, put_l, put_sp, put_pl, put_cy};
	int			type;

	type = check_format(argv[0]);
	if (type == -1)
		return ;
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
		put_info(&info, split);
		split_free(split);
		free(line);
		line = get_next_line(fd);
	}
	return (info);
}

int	key_press(int keycode, t_info *info)
{
	if (keycode == KEY_ESC)
		exit(0);
	return (0);
}

void	print_obj(t_object *obj) // 지워야함
{
	t_object	*curr;

	curr = obj;
	printf("==========print_obj start==========\n");
	while (curr)
	{
		if (curr->type == SP)
		{
			printf("-------type SP------\n");
			printf("x : %lf ", ((t_sphere *)curr->element)->center.x);
			printf("y : %lf ", ((t_sphere *)curr->element)->center.y);
			printf("z : %lf \n", ((t_sphere *)curr->element)->center.z);
			printf("radius : %lf \n", ((t_sphere *)curr->element)->radius);
			printf("radius2 : %lf \n", ((t_sphere *)curr->element)->radius2);
			printf("color  R : %lf ", curr->albedo.x);
			printf("color  G : %lf ", curr->albedo.y);
			printf("color  B : %lf \n", curr->albedo.z);
		}
		else if (curr->type == PL)
		{
			printf("-------type PL------\n");
			printf("x : %lf ", ((t_plane *)curr->element)->center.x);
			printf("y : %lf ", ((t_plane *)curr->element)->center.y);
			printf("z : %lf \n", ((t_plane *)curr->element)->center.z);
			printf("normal x : %lf ", ((t_plane *)curr->element)->normal.x);
			printf("normal y : %lf ", ((t_plane *)curr->element)->normal.y);
			printf("normal z : %lf \n", ((t_plane *)curr->element)->normal.z);
			printf("color  R : %lf ", curr->albedo.x);
			printf("color  G : %lf ", curr->albedo.y);
			printf("color  B : %lf \n", curr->albedo.z);
		}
		else if (curr->type == CY)
		{
			printf("-------type CY------\n");
			printf("x : %lf ", ((t_cylinder *)curr->element)->center.x);
			printf("y : %lf ", ((t_cylinder *)curr->element)->center.y);
			printf("z : %lf \n", ((t_cylinder *)curr->element)->center.z);
			printf("normal x : %lf ", ((t_cylinder *)curr->element)->normal.x);
			printf("normal y : %lf ", ((t_cylinder *)curr->element)->normal.y);
			printf("normal z : %lf \n", ((t_cylinder *)curr->element)->normal.z);
			printf("radius : %lf \n", ((t_cylinder *)curr->element)->radius);
			printf("radius2 : %lf \n", ((t_cylinder *)curr->element)->radius2);
			printf("height : %lf \n", ((t_cylinder *)curr->element)->height);
			printf("color  R : %lf ", curr->albedo.x);
			printf("color  G : %lf ", curr->albedo.y);
			printf("color  B : %lf \n", curr->albedo.z);
		}
		curr = curr->next;
	}
	printf("==========print_obj end==========\n");
}
int	convert_color(t_vec clr)
{
	int tmp = ((int)clr.x * 16 * 16 * 16 * 16) + ((int)clr.y * 16 * 16) + (int)(clr.z);
	return (tmp);
}

void  my_mlx_pixel_put(t_img *img, int x, int y, t_color color)
{
	img->addr[WIN_W * y + x] = convert_color(color);
}

t_ray	ray_primary(t_camera cam, double u, double v)
{
    t_ray	ray;

    ray.orig = cam.orig;
    // left_bottom + u * horizontal + v * vertical - origin 의 단위 벡터.
    ray.dir = vec_unit(vec_sub(vec_add(vec_add(cam.start_point, vec_multi_double(cam.horizontal, u)), vec_multi_double(cam.vertical, v)), cam.orig));
    return (ray);
}

t_color    ray_color(t_info info)
{
    double			t;

	info.rec = record_init();
	// if (hit(info.obj, info.ray, &(info.rec)))
	// 	return (phong_lighting(&info));
	if (hit(info.obj, info.ray, &(info.rec)))
		return (vec_init(152,124,242));
	else
	{
		t = 0.5 * (info.ray.dir.y + 1.0);
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
    // 광선의 방향벡터와 교점의 법선벡터의 내적이 음수이면 광선은 앞면(객체의)에 hit 한 것이다
    rec->front_face = vec_dot(ray.dir, rec->normal) < 0;
    // 광선의 앞면에 hit 면 그대로 아니면 법선을 반대로 뒤집는다.
	// (항상 광선 방향벡터와 법선 벡터를 반대인 상태로 사용하기위해)
	if (rec->front_face == 0)
		rec->normal = vec_multi_double(rec->normal, -1);
    return ;
}

int main(int argc, char **argv)
{
	t_info	info;
	t_mlx	mlx;
	int		idx[2];
	double	vdx[2];

	ft_memset(&info, 0, sizeof(t_info));
	if (argc != 2)
		ft_strerror("인자 잘못넣음");
	info = info_init(info, argv[1]);
	// print_obj(info.obj);
	mlx.ptr = mlx_init();
	mlx.win = mlx_new_window(mlx.ptr, WIN_W, WIN_H, "HojinySesiMinsukiR2");
	mlx.img.img_ptr = mlx_new_image(mlx.ptr, IMG_W, IMG_H);
	mlx.img.addr = (int *)mlx_get_data_addr(mlx.img.img_ptr, \
		&(mlx.img.bits_per_pixel), &(mlx.img.line_length), &(mlx.img.endian));
	idx[Y] = WIN_H - 1;

	t_color color; // 지워야함
	while (idx[Y] >= 0)
	{
		idx[X] = 0;
		while (idx[X] < WIN_W)
		{
			vdx[U] = (double)idx[X] / (WIN_W - 1);
			vdx[V] = (double)idx[Y] / (WIN_H - 1);
			info.ray = ray_primary(info.camera, vdx[U], vdx[V]);
			// color = vec_init(((double)idx[Y] / (WIN_H - 1)) * 255 , ((double)idx[X] / (WIN_W - 1)) * 255, 0.25 * 255);
			color = ray_color(info);
			my_mlx_pixel_put(&mlx.img, idx[X], WIN_H - 1 - idx[Y], color);
			idx[X]++;
		}
		idx[Y]--;
	}
	mlx_put_image_to_window(mlx.ptr, mlx.win, mlx.img.img_ptr, 0, 0);
	info.mlx = mlx;
	mlx_hook(mlx.win, EVENT_KEY_PRESS, 0, key_press, 0);
	mlx_loop(mlx.ptr);
	return (0);
}