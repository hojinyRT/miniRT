#include "minirt.h"

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

	sp = obj->element;
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
	rec->albedo = obj->albedo;
	rec->t = root;
	rec->p = ray_at(ray, root);
	rec->normal = vec_div_double(vec_sub(rec->p, ((t_sphere*)obj->element)->center), ((t_sphere*)obj->element)->radius); // 정규화된 법선 벡터.
	set_face_normal(ray, rec); // rec의 법선벡터와 광선의 방향벡터를 비교해서 앞면인지 뒷면인지 t_bool 값으로 저장.
	return (TRUE);
}

t_sphere	*sphere_init(t_point center, double radius)
{
	t_sphere *init;

    if(!(init = (t_sphere *)malloc(sizeof(t_sphere))))
        return (NULL);
	init->center = center;
	init->radius = radius;
	init->radius2 = radius * radius;
	return (init);
}


t_ray	ray_init(t_point orig, t_vec dir)
{
	t_ray init;

	init.orig = orig;
	init.dir = vec_unit(dir);
	return (init);
}

t_point	ray_at(t_ray ray, double t)
{
	t_point at;

	at = vec_add(ray.orig, vec_multi_double(ray.dir, t));
	return (at);
}

t_ray	ray_primary(t_camera cam, double u, double v)
{
    t_ray	ray;

    ray.orig = cam.orig;
    // left_bottom + u * horizontal + v * vertical - origin 의 단위 벡터.
    ray.dir = vec_unit(vec_sub(vec_add(vec_add(cam.start_point, vec_multi_double(cam.horizontal, u)), vec_multi_double(cam.vertical, v)), cam.orig));
    return (ray);
}

t_hit_record record_init(void)
{
    t_hit_record    record;

    record.tmin = EPSILON;
    record.tmax = 100000;
    return (record);
}

t_color    ray_color(t_scene *scene)
{
    double			t;

	scene->rec = record_init();
	if (hit(scene->obj, scene->ray, &(scene->rec)))
		return (phong_lighting(scene));
	else
	{
		t = 0.5 * (scene->ray.dir.y + 1.0);
		return (vec_add(vec_multi_double(vec_init(255, 255, 255), 1.0 - t), vec_multi_double(vec_init(128, 178, 255), t)));
	}	
}

t_camera	camera_init(t_canvas canvas, t_point orig)
{
    t_camera	cam;
    double		focal_len;
    double		viewport_height;

    viewport_height = 2.0;
    focal_len = 1.0;
    cam.orig = orig;
    cam.viewport_h = viewport_height;
    cam.viewport_w = viewport_height * canvas.aspect_ratio;
    cam.focal_len = focal_len;
    cam.horizontal = vec_init(cam.viewport_w, 0, 0);
    cam.vertical = vec_init(0, cam.viewport_h, 0);
    cam.start_point = vec_sub(vec_sub(vec_sub(cam.orig, vec_div_double(cam.horizontal, 2)),
                                vec_div_double(cam.vertical, 2)), vec_init(0, 0, focal_len));
    return (cam);
}