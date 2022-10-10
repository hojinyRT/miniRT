#include "minirt.h"


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
    {
        return (TRUE);
    }
    return (FALSE);
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
		return (TRUE);
    return (FALSE);
}

// int	hit_cone(t_object *obj, t_ray ray, t_hit_record *rec)  //illusion catalyst function
// {
// 	t_cylinder	*cy;
// 	t_vec		oc;
// 	double		a;
// 	double		half_b;
// 	double		c;
// 	double		dis;
// 	double		sqrtd;
// 	double		root;
// 	t_vec cp, cq;
// 	double	cq_val;
// 	double	m;
// 	t_vec	w;
// 	t_vec	v;
// 	t_vec	h;
// 	t_point	h_point;

// 	cy = (t_cylinder *)obj->element;
// 	h_point = vec_add(vec_multi_double(cy->normal, cy->height), cy->center);
// 	w = vec_sub(ray.orig, h_point);
// 	m = cy->radius2 / (cy->height * cy->height);
// 	oc = vec_sub(rec->p, cy->center);
//     h = vec_multi_double(cy->normal, -1);
// 	a = vec_dot(ray.dir, ray.dir) - ((m + 1) * vec_dot(ray.dir, h) * vec_dot(ray.dir, h));
// 	half_b = vec_dot(ray.dir, w) - ((m + 1) * (vec_dot(ray.dir, h) * vec_dot(w, h)));
// 	c = vec_dot(w, w) - ((m + 1) * (vec_dot(w, h)) * (vec_dot(w, h)));
// 	printf("a = %lf || b = %lf || c = %lf\n", a, half_b, c);
// 	dis = half_b * half_b - a * c;
// 	if (dis < 0)
// 		return (FALSE);
// 	sqrtd = sqrt(dis);
// 	root = (-half_b - sqrtd) / a;
// 	rec->t = root;
// 	rec->p = ray_at(ray, root);
// 	if (vec_dot(vec_sub(rec->p, h_point), h) < 0 
//         || vec_dot(vec_sub(rec->p, h_point), h) > cy->height)
// 	{
// 		root = (-half_b + sqrtd) / a;
// 		if (root < rec->tmin || rec->tmax < root)
// 			return (FALSE);
// 		rec->t = root;
// 		rec->p = ray_at(ray, root);
// 	}
//     if (root < rec->tmin || rec->tmax < root)
// 		return (FALSE);
// 	// t_vec h = vec_add(cy->center, vec_multi_double(cy->normal, cy->height));
// 	cp = vec_sub(rec->p, h);
// 	// cq_val = vec_dot(cp, cy->normal);
// 	// cq = vec_multi_double(cy->normal, cq_val);
// 	// v = vec_multi_double(cy->normal, -1);
// 	v = vec_unit(vec_sub(cy->center, h));
// 	double  a_c = cy->height - ((vec_len_sqr(vec_sub(rec->p, h_point))) / (cy->height - vec_dot(vec_sub(rec->p, cy->center), cy->normal)));
//     t_vec   ac_vec = vec_multi_double(cy->normal, a_c);
// 	rec->normal = vec_unit(vec_sub(rec->p, vec_add(ac_vec, cy->center)));
// 	// printf("x : %lf ", rec->normal.x);
// 	// printf("y : %lf ", rec->normal.y);
// 	// printf("z : %lf ", rec->normal.z);
// 	// printf("h : %lf\n", vec_dot(vec_sub(rec->p, h_point), h));
// 	rec->albedo = obj->albedo;
// 	set_face_normal(ray, rec); // rec의 법선벡터와 광선의 방향벡터를 비교해서 앞면인지 뒷면인지 t_bool 값으로 저장.
// 	if (0 <= vec_dot(vec_sub(rec->p, h_point), h) && \
//         vec_dot(vec_sub(rec->p, h_point), h) <= cy->height)
// 		return (TRUE);
//     return (FALSE);
// }

int	hit_cone(t_object *obj, t_ray ray, t_hit_record *rec)   // our function
{
	t_cylinder	*cy;
	// t_vec		oc;
	double		a;
	double		half_b;
	double		c;
	double		dis;
	double		sqrtd;
	double		root;
	// t_vec cp, cq;
	// double	cq_val;
	double	m;
	t_vec	w;
	// t_vec	v;

	cy = (t_cylinder *)obj->element;
	w = vec_sub(ray.orig, cy->center);
	m = cy->radius2 / (cy->height * cy->height);
	double	v_n_pow = (vec_dot(ray.dir, cy->normal)) * (vec_dot(ray.dir, cy->normal));
	double	v_n_dot_w_n = (vec_dot(ray.dir, cy->normal) * vec_dot(w, cy->normal));
	double	w_n_pow = vec_dot(w, cy->normal) * (vec_dot(w, cy->normal));
	a = vec_dot(ray.dir, ray.dir) - ((m + 1) * v_n_pow);
	half_b = vec_dot(ray.dir, w) - ((m + 1) * v_n_dot_w_n) + (m * cy->height * vec_dot(ray.dir, cy->normal));
	c = vec_dot(w, w) - ((m + 1) * w_n_pow) + (2 * m * cy->height * vec_dot(w, cy->normal)) - cy->radius2;
	// printf("a = %lf || b = %lf || c = %lf\n", a, half_b, c);
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
	set_face_normal(ray, rec); // rec의 법선벡터와 광선의 방향벡터를 비교해서 앞면인지 뒷면인지 t_bool 값으로 저장.
	if (0 <= vec_dot(vec_sub(rec->p, cy->center), cy->normal) &&
    	vec_dot(vec_sub(rec->p, cy->center), cy->normal) <= cy->height)
		return (TRUE);
    return (FALSE);
}

int	hit_sphere(t_object *obj, t_ray ray, t_hit_record *rec)
{
	t_sphere	*sp;
	t_vec		oc;
	double		a;
	// double		b;
	double		c;
	double		dis;
	double		half_b;
	double		sqrtd;
	double		root;

    // printf("IN ! \n");
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
	{
		root = (-half_b + sqrtd) / a;
		if (root < rec->tmin || rec->tmax < root)
			return (FALSE);
	}
    if (root < rec->tmin || rec->tmax < root)
		return (FALSE);
	rec->albedo = obj->albedo;
	rec->t = root;
	rec->p = ray_at(ray, root);
	rec->normal = vec_div_double(vec_sub(rec->p, ((t_sphere*)obj->element)->center), ((t_sphere*)obj->element)->radius); // 정규화된 법선 벡터.
	// set_face_normal(ray, rec); // rec의 법선벡터와 광선의 방향벡터를 비교해서 앞면인지 뒷면인지 t_bool 값으로 저장.

	return (TRUE);
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
	set_face_normal(ray, rec); // rec의 법선벡터와 광선의 방향벡터를 비교해서 앞면인지 뒷면인지 t_bool 값으로 저장.
    t_vec pcv = vec_sub(rec->p, pl->center);
    // printf("plr %lf\n", pl->radius);
    if (vec_dot(pcv, pcv) > pl->radius * pl->radius)
        return (FALSE);
	return (TRUE);
}

void	get_plane_uv(t_hit_record *rec, t_point center, double size)
{
	const t_vec		p = vec_sub(rec->p, center);
	const t_vec		n = rec->normal;
	t_vec			e1;
	t_vec			e2;

	e1 = vec_unit(vec_cross(n, vec_init(1, 0, 0)));
	if (e1.x == 0 && e1.y == 0 && e1.z == 0)
		e1 = vec_unit(vec_cross(n, vec_init(0, 1, 0)));
	e2 = vec_unit(vec_cross(n, e1));
	rec->e1 = e1;
	rec->e2 = e2;
	rec->u = fmod(vec_dot(e1, p), size) / size;
	rec->v = fmod(vec_dot(e2, p), size) / size;
	rec->u = (rec->u + 1) / 2;
	rec->v = (rec->v + 1) / 2;
	// printf("u : %lf , v : %lf\n", rec->u, rec->v);
}


t_vec	bump_normal(t_info info, t_hit_record rec)
{
	int x;
	int y;
	t_vec tmp;
	t_vec ul, vl, zl;

// Local = t * UL + b * VL + n * ZL
	x = (int)(rec.u * 1024.);
	y = (int)(rec.v * 1024.);
	tmp = convert_color_to_normal(*(unsigned int *)(info.bump.addr + info.bump.line_length * y + x * info.bump.bits_per_pixel / 8));
	ul = vec_multi_double(rec.e1, tmp.x);
	vl = vec_multi_double(rec.e2, tmp.y);
	zl = vec_multi_double(rec.normal, tmp.z);
	tmp = vec_add(vec_add(ul, vl), zl);
	// debugPrintVec("normal", &tmp);
	// tmp = vec_unit(tmp);
	return (tmp);
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
	get_plane_uv(rec, pl->center, 5);
	rec->normal = bump_normal(*obj->info, *rec);
	set_face_normal(ray, rec); // rec의 법선벡터와 광선의 방향벡터를 비교해서 앞면인지 뒷면인지 t_bool 값으로 저장.
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

    temp_rec = *rec; // temp_rec의 tmin, tmax 값 초기화를 위해.
    hit_anything = FALSE;
    t_object *tmp = obj;
    while(tmp)
    {
        if (hit_obj(tmp, ray, &temp_rec))
        {
            hit_anything = TRUE;
            temp_rec.tmax = temp_rec.t;
            *rec = temp_rec;
        }
        tmp = tmp->next;
    }
    return (hit_anything);
}

t_vec          reflect(t_vec v, t_vec n)
{
    //v - 2 * dot(v, n) * n;
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

    // light_dir = vec_unit(vec_sub(light->origin, info->rec.p)); //교점에서 출발하여 광원을 향하는 벡터(정규화 됨)

    // 추가
    light_dir = vec_sub(light->origin, info->rec.p);
    light_len = vec_len(light_dir);
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
    ksn = 20; // shininess value
    ks = 0.1; // specular strength 강도 계수
    spec = pow(fmax(vec_dot(view_dir, reflect_dir), 0.0), ksn);
    specular = vec_multi_double(vec_multi_double(light->light_color, ks), spec);
    brightness = light->brightness * LUMEN; // 기준 광속/광량을 정의한 매크로
    return (vec_multi_double(vec_add(diffuse, specular), brightness));
}

t_color	checkerboard_value(t_hit_record rec)
{
	const int		width = 2;
	const int		height = 3;
	const double	u2 = floor(rec.u * width);
	const double	v2 = floor(rec.v * height);

	if (fmod(u2 + v2, 2.) == 0)
		return (rec.albedo);
	return (vec_init(1, 1, 1));
}

t_color	bump_value(t_info info, t_hit_record rec)
{
	int x;
	int y;
	t_color tmp;

	x = (int)(rec.u * 350.);
	y = (int)(rec.v * 350.);
	tmp = convert_color_to_normal(*(unsigned int *)(info.bump.addr + info.bump.line_length * y + x * info.bump.bits_per_pixel / 8));
	// debugPrintVec("color", &tmp);
	tmp = vec_unit(tmp);
	return (tmp);
}

t_vec	phong_lighting(t_info *info)
{
    t_color		light_color;
    t_light		*lights;
	t_color		color;

	// printf("rec rec rec : tmax %lf tmin %lf\n", info->rec.tmax, info->rec.tmin);
    light_color = vec_init(0, 0, 0); //광원이 하나도 없다면, 빛의 양은 (0, 0, 0)일 것이다.
    lights = info->light;
    while (lights) //여러 광원에서 나오는 모든 빛에 대해 각각 diffuse, specular 값을 모두 구해줘야 한다
    {
		light_color = vec_add(light_color, point_light_get(info, lights));
		lights = lights->next;
    }
	// printf("%lf, %lf, %lf\n", info->ambient.x, info->ambient.y, info->ambient.z);
    light_color = vec_add(light_color, info->ambient);
	// printf("%lf,%lf,%lf\n", light_color.x, light_color.y, light_color.z);
	// color = checkerboard_value((info->rec));
	color = bump_value(*info, info->rec);
	// color = rec.albedo;
    return (vec_multi_double(vec_min(vec_multi(light_color, color), vec_init(1, 1, 1)), 255));
    //모든 광원에 의한 빛의 양을 구한 후, 오브젝트의 반사율과 곱해준다. 그 값이 (1, 1, 1)을 넘으면 (1, 1, 1)을 반환한다.
}
