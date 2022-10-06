
#include "minirt.h"

void debugPrintVec(char *str, t_vec *vector)
{
	printf("%s : x = %lf ",  str, vector->x);
	printf("%s : y = %lf ",  str, vector->y);
	printf("%s : z = %lf\n", str, vector->z);
}

void	print_cam(t_camera *cam) // 지워야함
{
	t_camera	*curr;

	curr = cam;
	printf("==========print_cam start==========\n");
	while (curr)
	{
		printf("-------CAM------\n");
		printf("orign x : %lf ", (curr->orig.x));
		printf("orign y : %lf ", (curr->orig.y));
		printf("orign z : %lf \n", (curr->orig.z));
		printf("viewport_w : %lf \n", (curr->viewport_w));
		printf("viewport_h : %lf \n", (curr->viewport_h));
		printf("normal x : %lf ", curr->normal.x);
		printf("normal y : %lf ", curr->normal.y);
		printf("normal z : %lf \n", curr->normal.z);
		if (curr->next == cam)
			break ;
		curr = curr->next;
	}
	printf("==========print_cam end==========\n");
}


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

// t_camera    *camera_init(t_point coor, t_vec normal, int fov)
// {
//     t_camera    *init;

//     if (!(init = (t_camera *)calloc(1, sizeof(t_camera))))
//         return (NULL);
// 	init->orig = coor;
// 	init->normal = normal;
// 	init->viewport_w = tan((double)fov / 2 * M_PI / 180) * 2;
// 	init->viewport_h = init->viewport_w * WIN_H / WIN_W;
// 	// init->horizontal = vec_init(init->viewport_w, 0, 0);
// 	init->horizontal = vec_init(0, 0, init->viewport_w);
// 	init->vertical = vec_init(0, init->viewport_h, 0);
// 	init->start_point = vec_sub(vec_sub(vec_sub(init->orig, vec_div_double(init->horizontal, 2)),
//                                 vec_div_double(init->vertical, 2)), normal);
//     return (init);
// }
t_camera    *camera_init(t_point coor, t_vec normal, int fov)
{
    t_camera    *init;

    if (!(init = (t_camera *)calloc(1, sizeof(t_camera))))
        return (NULL);
	init->orig = coor;
	init->normal = normal;
	init->viewport_w = tan((double)fov / 2 * M_PI / 180) * 2;
	init->viewport_h = init->viewport_w * WIN_H / WIN_W;
	if ((normal.x == 0 && normal.y == 1 && normal.z == 0))
		init->horizontal = vec_multi_double(vec_unit(vec_cross(normal, vec_init(0, 0, 1))), init->viewport_w);
	else if((normal.x == 0 && normal.y == -1 && normal.z == 0))
		init->horizontal = vec_multi_double(vec_unit(vec_cross(normal, vec_init(0, 0, -1))), init->viewport_w);
	else
		init->horizontal = vec_multi_double(vec_unit(vec_cross(normal, vec_init(0, 1, 0))), init->viewport_w); // RT파일에서 불가능한 회전
	debugPrintVec("hor", &init->horizontal);
	init->vertical =  vec_multi_double(vec_unit(vec_cross(init->horizontal, normal)), init->viewport_h);
	debugPrintVec("ver", &init->vertical);
	// init->start_point = vec_sub(vec_sub(vec_sub(init->orig, vec_div_double(init->horizontal, 2)),
    //                             vec_div_double(init->vertical, 2)), normal);
	init->start_point = vec_sub(vec_sub(vec_sub(init->orig, vec_div_double(init->horizontal, 2)),
                                vec_div_double(init->vertical, 2)), vec_multi_double(vec_multi_double(normal, -1),1));
    return (init);
}

void    camera_add(t_camera **list, t_camera *new)
{
    t_camera    *cur;

    if (list == NULL)
        return ;
    if (*list == NULL)
    {
        *list = new;
		(*list)->next = *list;
        return ;
    }
    cur = *list;
    while (cur->next && cur->next != *list)
        cur = cur->next;
    cur->next = new;
	new->next = *list;
}

void	put_c(t_info *info, char **argv)
{
	t_point	coor;
	t_vec	normal;
	int		fov;
	t_camera *tmp;

	coor = ft_atovec(argv[1], XYZ);
	// normal = ft_atovec(argv[2], UNIT);
	normal = ft_atovec(argv[2], UNIT);
	fov = ft_atoi(argv[3]);
	if (fov < 0 || fov > 180)
		ft_strerror("카메라 앵글 잘못됨"); //에러메시지 출력

	tmp = camera_init(coor, normal, fov);
	camera_add(&(info->camera), tmp);
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
	color = ft_atovec(argv[3], RGB);

	tmp = object_init(PL, plane_init(origin, normal, 0), vec_div_double(color, 255));
	tmp->info = info;
	obj_add(&(info->obj), tmp);
}

t_point	get_cap_point(t_point center, double height, t_vec normal, double sign)
{
	t_vec	ccprime;

	ccprime = vec_multi_double(vec_multi_double(normal, sign), height / 2);
	return (vec_add(center, ccprime));
}

void	put_cy(t_info *info, char **argv)
{
	t_object	*tmp;
	t_point		center;
	t_vec		normal;
	double		format[2];
	t_color		color;

	center = ft_atovec(argv[1], XYZ);
	normal = ft_atovec(argv[2], UNIT);
	format[0] = ft_atod(argv[3]) / 2;
	format[1] = ft_atod(argv[4]);
	color = ft_atovec(argv[5], RGB);
	tmp = object_init(CY, cylinder_init(get_cap_point(center, format[1], normal, -1), format[0], format[1], normal), vec_div_double(color, 255));
	obj_add(&(info->obj), tmp);
	tmp = object_init(CAP,plane_init(get_cap_point(center, format[1], normal, -1), normal, format[0]), vec_div_double(color, 255));
	obj_add(&(info->obj), tmp);
	tmp = object_init(CAP,plane_init(get_cap_point(center, format[1], normal, -1), normal, format[0]), vec_div_double(color, 255));
	obj_add(&(info->obj), tmp);
}

void	put_cn(t_info *info, char **argv)
{
	t_object	*tmp;
	t_point		center;
	t_vec		normal;
	double		format[2];
	t_color		color;

	center = ft_atovec(argv[1], XYZ);
	normal = ft_atovec(argv[2], UNIT);
	format[0] = ft_atod(argv[3]) / 2;
	format[1] = ft_atod(argv[4]);
	color = ft_atovec(argv[5], RGB);
	tmp = object_init(CN, cone_init(get_cap_point(center, format[1], normal, -1), format[0], format[1], normal), vec_div_double(color, 255));
	obj_add(&(info->obj), tmp);
	tmp = object_init(CAP,plane_init(get_cap_point(center, format[1], normal, -1), normal, format[0]), vec_div_double(color, 255));
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
	else if (!ft_strncmp(format, "cn", 3))
		return (CN);
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
	static void	(*run[7])(t_info *, char **) = {put_a, put_c, put_l, put_sp, put_pl, put_cy, put_cn};
	int			type;

	type = check_format(argv[0]);
	if (type == -1)
		return ;
	run[type](info, argv);
}

void info_init(t_info *info, char *file)
{
	char		**split;
	int			fd;
	char		*line;
	// double		ka;

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


t_vec	convert_color_to_normal(int	color)
{
	t_vec	res;

	res.x = color >> 16 & 0xFF;
	res.y = color >> 8 & 0xFF;
	res.z = color & 0xFF;
	if (res.x == 0 && res.y == 0 && res.z == 0)
		ft_strerror("범프맵 이상함");
	res = vec_div_double(res, 255);
	res = vec_sub(vec_multi_double(res, 2), vec_init(1, 1, 1));
	// debugPrintVec("normal", &res);
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
//win_W 5
//win_H 2
//00000 00000

void	ray_primary(t_ray *ray, t_camera *cam, double u, double v)
{
    ray->orig = cam->orig;
    ray->dir =  vec_unit(vec_sub(vec_add(vec_add(cam->start_point, \
							vec_multi_double(cam->horizontal, u)), \
							vec_multi_double(cam->vertical, v)), cam->orig));

}
// left_bottom + u * horizontal + v * vertical - origin 의 단위 벡터.

t_color    ray_color(t_info *info)
{
    double			t;

	// print_cam(info.camera);
	record_init(&(info->rec));
	if (hit(info->obj, info->ray, &(info->rec)))
		return (phong_lighting(info));
	// if (hit(info->obj, info->ray, &(info->rec)))
	// 	return (vec_multi_double(vec_init(0, 0, 255), 0.8));
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
    // 광선의 방향벡터와 교점의 법선벡터의 내적이 음수이면 광선은 앞면(객체의)에 hit 한 것이다
    rec->front_face = vec_dot(ray.dir, rec->normal) < 0;
    // 광선의 앞면에 hit 면 그대로 아니면 법선을 반대로 뒤집는다.
	// (항상 광선 방향벡터와 법선 벡터를 반대인 상태로 사용하기위해)
	if (rec->front_face == 0)
		rec->normal = vec_multi_double(rec->normal, -1);
    return ;
}

// void ft_draw(t_info *info)
// {
// 	int		idx[2];
// 	int		vdx[2];
// 	t_color	color;

// 	idx[Y] = WIN_H - 1;
// 	while (idx[Y] >= 0)
// 	{
// 		idx[X] = 0;
// 		while (idx[X] < WIN_W)
// 		{
// 			vdx[U] = (double)idx[X] / (WIN_W - 1);
// 			vdx[V] = (double)idx[Y] / (WIN_H - 1);
// 			info->ray = ray_primary(info->camera, vdx[U], vdx[V]);
// 			// color = vec_init(((double)idx[Y] / (WIN_H - 1)) * 255 , ((double)idx[X] / (WIN_W - 1)) * 255, 0.25 * 255);
// 			color = ray_color(*info);
// 			my_mlx_pixel_put(&(info->mlx.img), idx[X], WIN_H - 1 - idx[Y], color);
// 			idx[X]++;
// 		}
// 		idx[Y]--;
// 	}
// }

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
	// print_cam(info->camera);
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

// void	ae()
// {
// 	system("leaks miniRT");
// }

void	get_bump(t_info *info)
{
	t_vec	tmp;
	int		format[2];
	int		idx[2];
	char	*dst;

	ft_bzero(idx, sizeof(idx));
	info->bump.img_ptr = mlx_png_file_to_image(info->mlx.ptr, "coor.png" , &format[0], &format[1]);
	info->bump.addr = mlx_get_data_addr(info->bump.img_ptr, \
											&(info->bump.bits_per_pixel), \
											&(info->bump.line_length), \
											&(info->bump.endian));
	while (idx[0] < format[1])
	{
		idx[1] = 0;
		while (idx[1] < format[0])
		{
			dst = info->bump.addr + (idx[0] * info->bump.line_length + idx[1] * (info->bump.bits_per_pixel / 8));
			tmp = convert_color_to_normal(*(unsigned int *)dst);
			idx[1]++;
		}
		idx[0]++;
	}
	// for (int i = 0; i < 100; ++i)
	// 	printf("idx : %d .  val : %#x\n", i, ((int *)info->bump.addr)[i]);
}

int main(int argc, char **argv)
{
	t_info	info;

	// atexit(ae);

	if (argc != 2)
		ft_strerror("인자 잘못넣음");
	ft_memset(&info, 0, sizeof(t_info));
	info_init(&info, argv[1]);
	info.mlx.ptr = mlx_init();
	info.mlx.win = mlx_new_window(info.mlx.ptr, WIN_W, WIN_H, "HojinySesiMinsukiR2");

	info.mlx.img.img_ptr = mlx_new_image(info.mlx.ptr, IMG_W, IMG_H);

	info.mlx.img.addr = mlx_get_data_addr(info.mlx.img.img_ptr, \
											&(info.mlx.img.bits_per_pixel), \
											&(info.mlx.img.line_length), \
											&(info.mlx.img.endian));
	get_bump(&info);
	ft_draw(&info, &info.mlx);
	mlx_put_image_to_window(info.mlx.ptr, info.mlx.win, info.mlx.img.img_ptr, 0, 0);
	mlx_hook(info.mlx.win, EVENT_KEY_PRESS, 0, key_press, &info);
	mlx_loop(info.mlx.ptr);
	return (0);
}

	// for (int i = 0; i < 100; i++)
	// {
	// 	my_mlx_pixel_put(&(mlx->img), WIN_W / 2, WIN_H / 2 + i, vec_init(255, 0, 0));
	// }
	// for (int i = 0; i < 100; i++)
	// {
	// 	my_mlx_pixel_put(&(mlx->img), WIN_W / 2 + i, WIN_H / 2, vec_init(255, 0, 0));
	// }
