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

void	put_a(t_info *info, char **argv, int cnt)
{
	double	brightness;
	t_color	color;

	if (cnt != 3)
		ft_strerror("a 인자 개수 안맞음");
	brightness = ft_atod(argv[1]);
	color = vec_div_double(ft_atovec(argv[2], RGB), 255);
	info->ambient = vec_multi_double(color, brightness);
}

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

void	put_c(t_info *info, char **argv, int cnt)
{
	t_point	coor;
	t_vec	normal;
	int		fov;
	t_camera *tmp;

	if (cnt != 4)
		ft_strerror("c인자 개수 안맞음");
	coor = ft_atovec(argv[1], XYZ);
	// normal = ft_atovec(argv[2], UNIT);
	normal = ft_atovec(argv[2], UNIT);
	fov = ft_atoi(argv[3]); //잘못들어오면 exit해야함
	if (fov < 0 || fov > 180)
		ft_strerror("카메라 앵글 잘못됨"); //에러메시지 출력

	tmp = camera_init(coor, normal, fov);
	camera_add(&(info->camera), tmp);
}

void	put_l(t_info *info, char **argv, int cnt)
{
	t_light	*tmp;
	t_vec	origin;
	double	brightness;
	t_color	color;

	if (cnt != 4)
		ft_strerror("l인자 개수 안맞음");
	origin = ft_atovec(argv[1], XYZ);
	brightness = ft_atod(argv[2]);
	color = vec_div_double(ft_atovec(argv[3], RGB), 255);

	tmp = light_init(origin, color, brightness); // 더미 albedo
	light_add(&(info->light), tmp);
}

void	put_sp(t_info *info, char **argv, int cnt)
{
	t_object	*tmp;
	t_vec		origin;
	double		radius;
	t_color		color;
	int			checker;

	if (cnt == 4 || cnt == 5)
	{
		checker = (ft_strlen(argv[0]) == 5);
		origin = ft_atovec(argv[1], XYZ);
		radius = ft_atod(argv[2]) / 2;
		color = ft_atovec(argv[3], RGB);
		tmp = object_init(SP, sphere_init(origin, radius), vec_div_double(color, 255), checker);
		tmp->bump = ft_calloc(1, sizeof(t_img));
		tmp->tex = ft_calloc(1, sizeof(t_img));
		if (cnt == 5)
		{
			tmp->bump->file_name = ft_strdup(argv[4]);
			get_bump_addr(tmp, &info->mlx);
			// get_texture_addr(tmp, &info->mlx);
		}
		obj_add(&(info->obj), tmp);
	}
	else
		ft_strerror("sp인자 개수 안맞음");
}
void	put_pl(t_info *info, char **argv, int cnt)
{
	t_object	*tmp;
	t_vec		origin;
	t_vec		normal;
	t_color		color;
	int			checker;

	if (cnt == 4 || cnt == 5)
	{
		checker = (ft_strlen(argv[0]) == 5);
		origin = ft_atovec(argv[1], XYZ);
		normal = ft_atovec(argv[2], UNIT);
		color = ft_atovec(argv[3], RGB);
		tmp = object_init(PL, plane_init(origin, normal, 0), vec_div_double(color, 255), checker);
		tmp->bump = ft_calloc(1, sizeof(t_img));
		if (cnt == 5)
		{
			tmp->bump->file_name = ft_strdup(argv[4]);
			get_bump_addr(tmp, &info->mlx);
		}
		obj_add(&(info->obj), tmp);
	}
	else
		ft_strerror("pl인자 개수 안맞음");
}

t_point	get_cap_point(t_point center, double height, t_vec normal, double sign)
{
	t_vec	ccprime;

	ccprime = vec_multi_double(vec_multi_double(normal, sign), height / 2);
	return (vec_add(center, ccprime));
}

void	put_cy(t_info *info, char **argv, int cnt)
{
	t_object	*tmp;
	t_point		center;
	t_vec		normal;
	double		format[2];
	t_color		color;
	int			checker;
	t_object	*tmp_obj;

	if (cnt == 6 || cnt == 7)
	{
		checker = (ft_strlen(argv[0]) == 5);
		center = ft_atovec(argv[1], XYZ);
		normal = ft_atovec(argv[2], UNIT);
		format[0] = ft_atod(argv[3]) / 2;
		format[1] = ft_atod(argv[4]);
		color = ft_atovec(argv[5], RGB);
		tmp = object_init(CY, cylinder_init(get_cap_point(center, format[1], normal, -1), format[0], format[1], normal), vec_div_double(color, 255), checker);
		tmp->bump = ft_calloc(1, sizeof(t_img));
		tmp->tex = ft_calloc(1, sizeof(t_img));
		if (cnt == 7)
		{
			tmp->bump->file_name = ft_strdup(argv[6]);
			get_bump_addr(tmp, &info->mlx);
			// get_texture_addr(tmp, &info->mlx);
		}
		obj_add(&(info->obj), tmp);
		tmp_obj = tmp;
		// tmp = object_init(CAP,plane_init(get_cap_point(center, format[1], normal, -1), normal, format[0]), vec_div_double(color, 255), checker);
		// // if (cnt == 7)
		// // 		tmp->bump->file_name = argv[6];
		
		// obj_add(&(info->obj), tmp);
		tmp = object_init(CAP,plane_init(get_cap_point(center, format[1], normal, 1), normal, format[0]), vec_div_double(color, 255), checker);
		if (cnt == 7)
		{
			// tmp->bump->file_name = argv[6];
			tmp->bump = tmp_obj->bump;
			// printf("%s\n", tmp->bump->file_name);
		}
		obj_add(&(info->obj), tmp);
	}
	else
		ft_strerror("cy인자 개수 안맞음");
}

void	put_cn(t_info *info, char **argv, int cnt)
{
	t_object	*tmp;
	t_point		center;
	t_vec		normal;
	double		format[2];
	t_color		color;
	int			checker;
	t_object	*tmp_obj;

	if (cnt == 6 || cnt == 7)
	{
		checker = (ft_strlen(argv[0]) == 5);
		center = ft_atovec(argv[1], XYZ);
		normal = ft_atovec(argv[2], UNIT);
		format[0] = ft_atod(argv[3]) / 2;
		format[1] = ft_atod(argv[4]);
		color = ft_atovec(argv[5], RGB);
		tmp = object_init(CN, cone_init(get_cap_point(center, format[1], normal, -1), format[0], format[1], normal), vec_div_double(color, 255), checker);
		tmp->bump = ft_calloc(1, sizeof(t_img));
		tmp->tex = ft_calloc(1, sizeof(t_img));
		if (cnt == 7)
		{
			tmp->bump->file_name = ft_strdup(argv[6]);
			get_bump_addr(tmp, &info->mlx);
		}
		obj_add(&(info->obj), tmp);
		tmp_obj = tmp;
		tmp = object_init(CAP,plane_init(get_cap_point(center, format[1], normal, -1), normal, format[0]), vec_div_double(color, 255), checker);
		if (cnt == 7)
			tmp->bump = tmp_obj->bump;
		obj_add(&(info->obj), tmp);
	}
	else
		ft_strerror("cn인자 개수 안맞음");
}

int 	check_format(char *format)
{
	if (!format)
		return (-1);
	if (!ft_strncmp(format, "sp", 3) || !ft_strncmp(format, "sp-ch", 6))
		return (SP);
	else if (!ft_strncmp(format, "pl", 3) || !ft_strncmp(format, "pl-ch", 6))
		return (PL);
	else if (!ft_strncmp(format, "cy", 3) || !ft_strncmp(format, "cy-ch", 6))
		return (CY);
	else if (!ft_strncmp(format, "cn", 3) || !ft_strncmp(format, "cn-ch", 6))
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
	static void	(*run[7])(t_info *, char **, int) = {put_a, put_c, put_l, put_sp, put_pl, put_cy, put_cn};
	int			type;
	int			cnt;

	type = check_format(argv[0]);
	if (type == -1)
		return ;
	cnt = 0;
	while (argv[cnt])
		cnt++;
	run[type](info, argv, cnt);
}
