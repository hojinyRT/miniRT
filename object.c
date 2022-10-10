#include "minirt.h"

t_vec	convert_int_to_rgb(int	color)
{
	t_vec	res;

	res.x = color >> 16 & 0xFF;
	res.y = color >> 8 & 0xFF;
	res.z = color & 0xFF;
	return (res);
}

t_vec	tex_rgb(t_object *obj, t_hit_record *rec)
{
	int x;
	int y;
	t_vec tmp;

	x = (int)(rec->u * 512);
	y = (int)(rec->v * 512);
	tmp = convert_int_to_rgb(*(unsigned int *)(obj->tex->addr + obj->tex->line_length * y + x * obj->tex->bits_per_pixel / 8));
	tmp = vec_div_double(tmp, 255);
	return (tmp);
}

t_vec	bump_normal(t_object *obj, t_hit_record *rec)
{
	int x;
	int y;
	t_vec tmp;
	t_vec ul, vl, zl;

	// Local = t * UL + b * VL + n * ZL
	x = (int)(rec->u * (double)obj->bump->width);
	y = (int)(rec->v * (double)obj->bump->height);
	tmp = convert_color_to_normal(*(unsigned int *)(obj->bump->addr + obj->bump->line_length * y + x * obj->bump->bits_per_pixel / 8));
	ul = vec_multi_double(rec->e1, tmp.x);
	vl = vec_multi_double(rec->e2, tmp.y);
	zl = vec_multi_double(rec->normal, tmp.z);
	tmp = vec_add(vec_add(ul, vl), zl);
	tmp = vec_unit(tmp);
	return (tmp);
}

t_point	ray_at(t_ray ray, double t)
{
	t_point at;

	at = vec_add(ray.orig, vec_multi_double(ray.dir, t));
	return (at);
}

void record_init(t_hit_record *rec)
{
	ft_memset(rec, 0, sizeof(t_hit_record));
    rec->tmin = EPSILON;
    rec->tmax = 10000;
}

int in_shadow(t_object *objs, t_ray light_ray, double light_len)
{
    t_hit_record rec;

    rec.tmin = 0;
    rec.tmax = light_len;
    if (hit(objs, light_ray, &rec))
        return (TRUE);
    return (FALSE);
}

void	get_cylinder_uv(t_hit_record *rec, t_point center, t_vec normal, double size, double r)
{
	double			theta;
	t_vec			e1;
	t_vec			e2;
	double			p_e1;
	double			p_e2;

	if ((rec->p.x == 0 && rec->p.y == 0 && rec->p.z == 1))
		e1 = vec_unit(vec_cross(vec_init(0, 1, 0), normal));
	else if ((rec->p.x == 0 && rec->p.y == 0 && rec->p.z == -1))
		e1 = vec_unit(vec_cross(vec_init(0, -1, 0), normal));
	else
		e1 = vec_unit(vec_cross(vec_init(0, 0, 1), normal));
	e2 = vec_unit(vec_cross(normal, e1));
	p_e1 = vec_dot(vec_sub(rec->p, center), e1);
	p_e2 = vec_dot(vec_sub(rec->p, center), e2);
	theta = atan2(p_e2, p_e1);
	rec->u = (theta / (M_PI));
	rec->v = fmod(vec_dot(vec_sub(rec->p, center), normal) / (r * M_PI), 1);
	if (rec->u < 0)
		rec->u += 1;
	// debugPrintVec("rec", &rec->p);
	// debugPrintDouble("u", "v", rec->u, rec->v);
	rec->u = fmod(rec->u, size) / size;
	rec->v = fmod(rec->v, size) / size;
}

int	hit_cylinder(t_object *obj, t_ray ray, t_hit_record *rec)
{
	t_cylinder	*cy;
	t_vec		oc;
	double		a;
	double		half_b;
	double		c;
	double		dis;
	double		sqrtd;
	double		root;
	t_vec		cp, cq;
	double		cq_val;

	cy = (t_cylinder *)obj->element;
	oc = vec_sub(ray.orig, cy->center);
	a = vec_dot(ray.dir, ray.dir) - vec_dot(ray.dir, cy->normal) * vec_dot(ray.dir, cy->normal);
	half_b = vec_dot(oc, ray.dir) - ((vec_dot(ray.dir, cy->normal)) *  vec_dot(oc, cy->normal));
	c = vec_len_sqr(oc) - vec_dot(oc, cy->normal) * vec_dot(oc, cy->normal) - (cy->radius) * (cy->radius);
	dis = half_b * half_b - a * c;
	if (dis < 0)
		return (FALSE);
	sqrtd = sqrt(dis);
	root = (-half_b - sqrtd) / a;
	rec->t = root;
	rec->p = ray_at(ray, root);
	if ((vec_dot(vec_sub(rec->p, cy->center), cy->normal) > cy->height) || (vec_dot(vec_sub(rec->p, cy->center), cy->normal) < 0))
	{
		root = (-half_b + sqrtd) / a;
		if (root < rec->tmin || rec->tmax < root)
			return (FALSE);
		rec->t = root;
		rec->p = ray_at(ray, root);
	}
    if (root < rec->tmin || rec->tmax < root)
		return (FALSE);
	cp = vec_sub(rec->p, cy->center);
	cq_val = vec_dot(cp, cy->normal);
	cq = vec_multi_double(cy->normal, cq_val);
	rec->normal = vec_unit(vec_sub(cp, cq));
	rec->albedo = obj->albedo;
	set_face_normal(ray, rec); // rec의 법선벡터와 광선의 방향벡터를 비교해서 앞면인지 뒷면인지 t_bool 값으로 저장.
	if (0 <= vec_dot(vec_sub(rec->p, cy->center), cy->normal) && vec_dot(vec_sub(rec->p, cy->center), cy->normal) < cy->height)
	{
		get_cylinder_uv(rec, cy->center, cy->normal, 1, cy->radius);
		return (TRUE);
	}
    return (FALSE);
}

int	hit_cone(t_object *obj, t_ray ray, t_hit_record *rec)
{
	t_cylinder	*cy;
	double		a;
	double		half_b;
	double		c;
	double		dis;
	double		sqrtd;
	double		root;
	double	m;
	t_vec	w;

	cy = (t_cylinder *)obj->element;
	w = vec_sub(ray.orig, cy->center);
	m = cy->radius2 / (cy->height * cy->height);
	double	v_n_pow = (vec_dot(ray.dir, cy->normal)) * (vec_dot(ray.dir, cy->normal));
	double	v_n_dot_w_n = (vec_dot(ray.dir, cy->normal) * vec_dot(w, cy->normal));
	double	w_n_pow = vec_dot(w, cy->normal) * (vec_dot(w, cy->normal));
	a = vec_dot(ray.dir, ray.dir) - ((m + 1) * v_n_pow);
	half_b = vec_dot(ray.dir, w) - ((m + 1) * v_n_dot_w_n) + (m * cy->height * vec_dot(ray.dir, cy->normal));
	c = vec_dot(w, w) - ((m + 1) * w_n_pow) + (2 * m * cy->height * vec_dot(w, cy->normal)) - cy->radius2;
	dis = half_b * half_b - a * c;
	if (dis < 0)
		return (FALSE);
	sqrtd = sqrt(dis);
	root = (-half_b - sqrtd) / a;
	rec->t = root;
	rec->p = ray_at(ray, root);
	if (vec_dot(vec_sub(rec->p, cy->center), cy->normal) < 0
        || vec_dot(vec_sub(rec->p, cy->center), cy->normal) > cy->height)
	{
		root = (-half_b + sqrtd) / a;
		if (root < rec->tmin || rec->tmax < root)
			return (FALSE);
		rec->t = root;
		rec->p = ray_at(ray, root);
	}
    if (root < rec->tmin || rec->tmax < root)
		return (FALSE);
	double	nom = vec_len_sqr(vec_sub(vec_sub(rec->p, cy->center), vec_multi_double(cy->normal, cy->height)));
	double	denom = cy->height - vec_dot(vec_sub(rec->p, cy->center), cy->normal);
	double	a_c = cy->height - (nom / denom);
	rec->normal = vec_unit(vec_sub(rec->p, vec_add(cy->center, vec_multi_double(cy->normal, a_c))));
	rec->albedo = obj->albedo;
	set_face_normal(ray, rec);
	if (0 <= vec_dot(vec_sub(rec->p, cy->center), cy->normal) &&
    	vec_dot(vec_sub(rec->p, cy->center), cy->normal) <= cy->height)
	{
		get_cylinder_uv(rec, cy->center, cy->normal, 1, cy->radius);
		return (TRUE);
	}
    return (FALSE);
}

void	get_sphere_uv(t_hit_record *rec, t_point center, double size)
{
	double			phi;
	double			theta;
	// const t_vec		n = rec->normal;
	// t_vec			e1;
	// t_vec			e2;

	// if ((rec->x == 0 && n.y == 1 && n.z == 0))
	// 	e1 = vec_unit(vec_cross(vec_init(0, 0, -1), n));
	// else if ((n.x == 0 && n.y == -1 && n.z == 0))
	// 	e1 = vec_unit(vec_cross(vec_init(0, 0, 1), n));
	// else
	// 	e1 = vec_unit(vec_cross(vec_init(0, 1, 0), n));
	// e2 = vec_unit(vec_cross(n, e1));
	// rec->e1 = e1;
	// rec->e2 = e2;
	theta = atan2((rec->p.x - center.x), (rec->p.z - center.z));
	phi = acos((rec->p.y - center.y) / vec_len(vec_sub(rec->p, center)));
	rec->u = (theta / (M_PI));
	rec->v = phi / M_PI;
	if (rec->u < 0)
		rec->u += 1;
	rec->u = fmod(rec->u, size) / size;
	rec->v = fmod(rec->v, size) / size;
}

int	hit_sphere(t_object *obj, t_ray ray, t_hit_record *rec)
{
	t_sphere	*sp;
	t_vec		oc;
	double		a;
	double		c;
	double		dis;
	double		half_b;
	double		sqrtd;
	double		root;

	sp = (t_sphere *)obj->element;
	oc = vec_sub(ray.orig, ((t_sphere*)obj->element)->center);
	a = vec_len_sqr(ray.dir);
	half_b = vec_dot(oc, ray.dir);
	c = vec_len_sqr(oc) - ((t_sphere*)obj->element)->radius2;
	dis = half_b * half_b - a * c;
	if (dis < 0)
		return (FALSE);
	sqrtd = sqrt(dis);
	root = (-half_b - sqrtd) / a;
	if (root < rec->tmin || rec->tmax < root)
		root = (-half_b + sqrtd) / a;
    if (root < rec->tmin || rec->tmax < root)
		return (FALSE);
	rec->albedo = obj->albedo;
	rec->t = root;
	rec->p = ray_at(ray, root);
	rec->normal = vec_div_double(vec_sub(rec->p, ((t_sphere*)obj->element)->center), ((t_sphere*)obj->element)->radius);
	get_sphere_uv(rec, sp->center, 1); //조정 바람
	if (obj->bump->file_name)
	{
		rec->albedo = tex_rgb(obj, rec);
		rec->normal = bump_normal(obj, rec);
	}
	set_face_normal(ray, rec);
	return (TRUE);
}

void	get_cap_uv(t_hit_record *rec, t_point center, t_vec normal, double size)
{
	double			theta;
	t_vec			n = rec->normal;
	t_vec			e1;
	t_vec			e2;
	double			p_e1;
	double			p_e2;

	(void)n;
	if ((rec->p.x == 0 && rec->p.y == 0 && rec->p.z == 1))
		e1 = vec_unit(vec_cross(vec_init(0, 1, 0), normal));
	else if ((rec->p.x == 0 && rec->p.y == 0 && rec->p.z == -1))
		e1 = vec_unit(vec_cross(vec_init(0, -1, 0), normal));
	else
		e1 = vec_unit(vec_cross(vec_init(0, 0, 1), normal));
	e2 = vec_unit(vec_cross(normal, e1));
	p_e1 = vec_dot(vec_sub(rec->p, center), e1);
	p_e2 = vec_dot(vec_sub(rec->p, center), e2);
	theta = atan2(p_e2, p_e1);
	rec->u = (theta / (M_PI));
	if (rec->u < 0)
		rec->u += 1;
	// debugPrintVec("rec", &rec->p);
	// debugPrintDouble("u", "v", rec->u, rec->v);
	rec->u = fmod(rec->u, size) / size;
	rec->v = 1;
}

int	hit_cap(t_object *obj, t_ray ray, t_hit_record *rec)
{
	t_plane	*pl;
	double	root;
	double	numrator;
	double	denominator;

	pl = (t_plane *)obj->element;
	denominator = vec_dot(ray.dir, pl->normal);
	if (fabs(denominator) < EPSILON)
		return (FALSE);
	numrator = vec_dot(vec_sub(pl->center, ray.orig), pl->normal);
	root = numrator / denominator;
	if (root < rec->tmin || rec->tmax < root)
		return (FALSE);
	rec->t = root;
	rec->p = ray_at(ray, root);
	rec->albedo = obj->albedo;
	rec->normal = pl->normal;
	// debugPrintVec("pl center", &pl->center);
    t_vec pcv = vec_sub(rec->p, pl->center);
    if (vec_dot(pcv, pcv) > pl->radius * pl->radius)
        return (FALSE);
	get_cap_uv(rec, pl->center, pl->normal, 1);
	return (TRUE);
}

void	get_plane_uv(t_hit_record *rec, t_point center, double size)
{
	const t_vec		p = vec_sub(rec->p, center);
	const t_vec		n = rec->normal;
	t_vec			e1;
	t_vec			e2;

	if ((n.x == 0 && n.y == 1 && n.z == 0))
		e1 = vec_unit(vec_cross(vec_init(0, 0, -1), n));
	else if ((n.x == 0 && n.y == -1 && n.z == 0))
		e1 = vec_unit(vec_cross(vec_init(0, 0, 1), n));
	else
		e1 = vec_unit(vec_cross(vec_init(0, 1, 0), n));
	e2 = vec_unit(vec_cross(n, e1));
	rec->e1 = e1;
	rec->e2 = e2;
	rec->u = fmod(vec_dot(e1, p), size) / size;
	rec->v = fmod(vec_dot(e2, p), size) / size;
	rec->u += rec->u < 0;
	rec->v += rec->v < 0;
	rec->v = 1 - rec->v;
}

int	hit_plane(t_object *obj, t_ray ray, t_hit_record *rec)
{
	t_plane	*pl;
	double	root;
	double	numrator;
	double	denominator;

	pl = (t_plane *)obj->element;
	denominator = vec_dot(ray.dir, pl->normal);
	if (fabs(denominator) < EPSILON)
		return (FALSE);
	numrator = vec_dot(vec_sub(pl->center, ray.orig), pl->normal);
	root = numrator / denominator;
	if (root < rec->tmin || rec->tmax < root)
		return (FALSE);
	rec->t = root;
	rec->p = ray_at(ray, root);
	rec->albedo = obj->albedo;
	rec->normal = pl->normal;
	get_plane_uv(rec, pl->center, 10); //조정 바람
	if (obj->bump->file_name)
		rec->normal = bump_normal(obj, rec);
	set_face_normal(ray, rec);
	return (TRUE);
}

int hit_obj(t_object *obj, t_ray ray, t_hit_record *rec)
{
    int  hit_result;

    hit_result = FALSE;
    if (obj->type == SP)
        hit_result = hit_sphere(obj, ray, rec);
	else if (obj->type == PL)
        hit_result = hit_plane(obj, ray, rec);
    else if (obj->type == CY)
        hit_result = hit_cylinder(obj, ray, rec);
	else if (obj->type == CAP)
        hit_result = hit_cap(obj, ray, rec);
	else if (obj->type == CN)
        hit_result = hit_cone(obj, ray, rec);
    return (hit_result);
}

int hit(t_object *obj, t_ray ray, t_hit_record *rec)
{
	int				hit_anything;
    t_hit_record    temp_rec;

    temp_rec = *rec;
    hit_anything = FALSE;
    t_object *tmp = obj;
    while(tmp)
    {
        if (hit_obj(tmp, ray, &temp_rec))
        {
            hit_anything = TRUE;
            temp_rec.tmax = temp_rec.t;
			temp_rec.checker = tmp->checker;
            *rec = temp_rec;
        }
        tmp = tmp->next;
    }
    return (hit_anything);
}

t_vec          reflect(t_vec v, t_vec n)
{
    return (vec_sub(v, vec_multi_double(n, vec_dot(v, n) * 2)));
}

t_vec        point_light_get(t_info *info, t_light *light)
{
    t_color    diffuse;
    t_vec      light_dir;
    double      kd; // diffuse의 강도

    t_color    specular;
    t_vec      view_dir;
    t_vec      reflect_dir;

    double       light_len;
    t_ray       light_ray;

    double      spec;
    double      ksn;
    double      ks;
    double      brightness;


    light_dir = vec_sub(light->origin, info->rec.p);
    light_len = vec_len(light_dir);
    // light_ray = ray_init(vec_add(info->rec.p, vec_multi_double(light_dir, EPSILON)), light_dir);
    light_ray = ray_init(vec_add(info->rec.p, vec_multi_double(info->rec.normal, EPSILON)), light_dir);
    if (in_shadow(info->obj, light_ray, light_len))
        return (vec_init(0,0,0));
    light_dir = vec_unit(light_dir);
    // 추가끝
    // cosΘ는 Θ 값이 90도 일 때 0이고 Θ가 둔각이 되면 음수가 되므로 0.0보다 작은 경우는 0.0으로 대체한다.
    kd = fmax(vec_dot(info->rec.normal, light_dir), 0.0);// (교점에서 출발하여 광원을 향하는 벡터)와 (교점에서의 법선벡터)의 내적값.
    diffuse = vec_multi_double(light->light_color, kd);
    view_dir = vec_unit(vec_multi_double(info->ray.dir, -1));
    reflect_dir = reflect(vec_multi_double(light_dir, -1), info->rec.normal);
    ksn = 64; // shininess value
    ks = 0.1; // specular strength 강도 계수
    spec = pow(fmax(vec_dot(view_dir, reflect_dir), 0.0), ksn);
    specular = vec_multi_double(vec_multi_double(light->light_color, ks), spec);
    brightness = light->brightness * LUMEN; // 기준 광속/광량을 정의한 매크로
    return (vec_multi_double(vec_add(diffuse, specular), brightness));
}

t_color	checkerboard_value(t_hit_record rec)
{
	const int		width = 10;
	const int		height = 10;
	const double	u2 = floor(rec.u * width);
	const double	v2 = floor(rec.v * height);
	if (fmod(u2 + v2, 2.) == 0)
		return (rec.albedo);
	return (vec_init(1, 1, 1));
}

t_vec	phong_lighting(t_info *info)
{
    t_color		light_color;
    t_light		*lights;
	t_color		color;

    light_color = vec_init(0, 0, 0); //광원이 하나도 없다면, 빛의 양은 (0, 0, 0)일 것이다.
    lights = info->light;
    while (lights) //여러 광원에서 나오는 모든 빛에 대해 각각 diffuse, specular 값을 모두 구해줘야 한다
    {
		light_color = vec_add(light_color, point_light_get(info, lights));
		lights = lights->next;
    }
    light_color = vec_add(light_color, info->ambient);
	if (info->rec.checker)
		color = checkerboard_value((info->rec));
	else
		color = info->rec.albedo;
    return (vec_multi_double(vec_min(vec_multi(light_color, color), vec_init(1, 1, 1)), 255));
}
