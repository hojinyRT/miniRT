#include "minirt.h"

t_point	ray_at(t_ray ray, double t)
{
	t_point at;

	at = vec_add(ray.orig, vec_multi_double(ray.dir, t));
	return (at);
}

t_hit_record record_init(void)
{
    t_hit_record    record;

    record.tmin = EPSILON;
    record.tmax = 100000;
    return (record);
}

int in_shadow(t_object *objs, t_ray light_ray, double light_len)
{
    t_hit_record rec;

    rec.tmin = 0;
    rec.tmax = light_len;
    // printf("%f\n", light_len);
    if (hit(objs, light_ray, &rec))
    {
        // printf("WHAT THE HECK\n");
        return (TRUE);
    }
    return (FALSE);
}

int	hit_sphere(t_object *obj, t_ray ray, t_hit_record *rec)
{	
	t_sphere	*sp;
	t_vec		oc;
	double		a;
	double		b;
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
	// if (root < rec->tmin || rec->tmax < root)
	// {
	// 	root = (-half_b + sqrtd) / a;
	// 	if (root < rec->tmin || rec->tmax < root)
	// 		return (FALSE);
	// }
    if (root < rec->tmin || rec->tmax < root)
		return (FALSE);
	rec->albedo = obj->albedo;
	rec->t = root;
	rec->p = ray_at(ray, root);
	rec->normal = vec_div_double(vec_sub(rec->p, ((t_sphere*)obj->element)->center), ((t_sphere*)obj->element)->radius); // 정규화된 법선 벡터.
	// set_face_normal(ray, rec); // rec의 법선벡터와 광선의 방향벡터를 비교해서 앞면인지 뒷면인지 t_bool 값으로 저장.
	return (TRUE);
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
    ksn = 64; // shininess value
    ks = 0.5; // specular strength 강도 계수  
    spec = pow(fmax(vec_dot(view_dir, reflect_dir), 0.0), ksn);
    specular = vec_multi_double(vec_multi_double(light->light_color, ks), spec);
    brightness = light->brightness * LUMEN; // 기준 광속/광량을 정의한 매크로
    return (vec_multi_double(vec_add(diffuse, specular), brightness));
}

t_vec	phong_lighting(t_info *info)
{
    t_color		light_color;
    t_light		*lights;

    light_color = vec_init(0, 0, 0); //광원이 하나도 없다면, 빛의 양은 (0, 0, 0)일 것이다.
    lights = info->light;
    while (lights) //여러 광원에서 나오는 모든 빛에 대해 각각 diffuse, specular 값을 모두 구해줘야 한다
    {
		light_color = vec_add(light_color, point_light_get(info, lights));
		lights = lights->next;
    }
    light_color = vec_add(light_color, info->ambient);
    return (vec_multi_double(vec_min(vec_multi(light_color, info->rec.albedo), vec_init(1, 1, 1)), 255));
    //모든 광원에 의한 빛의 양을 구한 후, 오브젝트의 반사율과 곱해준다. 그 값이 (1, 1, 1)을 넘으면 (1, 1, 1)을 반환한다.
}