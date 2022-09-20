#include "minirt.h"

void    set_face_normal(t_ray ray, t_hit_record *rec)
{
    // 광선의 방향벡터와 교점의 법선벡터의 내적이 음수이면 광선은 앞면(객체의)에 hit 한 것이다
    rec->front_face = vec_dot(ray.dir, rec->normal) < 0;
    // 광선의 앞면에 hit 면 그대로 아니면 법선을 반대로 뒤집는다. 
	// (항상 광선 방향벡터와 법선 벡터를 반대인 상태로 사용하기위해)
	if (rec->front_face == 0)
		rec->normal = vec_multi_float(rec->normal, -1);
    return ;
}

// float		hit_sphere2(t_sphere sp, t_ray ray, t_hit_record rec)
// {
// 	t_vec	oc;
// 	float	a;
// 	float	b;
// 	float	c;
// 	float	dis;
// 	float 	half_b;
// 	float	sqrtd;
// 	double  root;

// 	oc = vec_sub(ray.orig, sp.center);
// 	a = vec_dot(ray.dir, ray.dir);
// 	b = 2.0 * vec_dot(oc, ray.dir);
// 	c = vec_dot(oc, oc) - sp.radius2;
// 	dis = b * b - 4 * a * c;

//     if (dis < 0) // 판별식이 0보다 작을 때 : 실근 없을 때,
//         return (-1.0);
//     else
//         return ((-b - sqrt(dis)) / (2.0 * a));
// }


int	hit_sphere(t_sphere sp, t_ray ray, t_hit_record *rec)
{
	t_vec	oc;
	float	a;
	float	b;
	float	c;
	float	dis;
	float 	half_b;
	float	sqrtd;
	double  root;

	oc = vec_sub(ray.orig, sp.center);
	// a = vec_dot(ray.dir, ray.dir);
	// b = 2.0 * vec_dot(oc, ray.dir);
	// c = vec_dot(oc, oc) - sp.radius2;
	// dis = b * b - 4 * a * c;

	a = vec_len_sqr(ray.dir);
	half_b = vec_dot(oc, ray.dir);
	c = vec_len_sqr(oc) - sp.radius2;
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
	rec->t = root;
	rec->p = ray_at(ray, root);
	// printf("what the rec->p : %f | sp.center : %f\n", rec->p.x, sp.center.x);
	rec->normal = vec_div_float(vec_sub(rec->p, sp.center), sp.radius); // 정규화된 법선 벡터.
	set_face_normal(ray, rec); // rec의 법선벡터와 광선의 방향벡터를 비교해서 앞면인지 뒷면인지 t_bool 값으로 저장.
	return (TRUE);
}

t_sphere	sphere_init(t_point center, float radius)
{
	t_sphere	init;

	init.center = center;
	init.radius = radius;
	init.radius2 = radius * radius;
	return (init);
}


t_ray	ray_init(t_point orig, t_vec dir)
{
	t_ray init;

	init.orig = orig;
	init.dir = vec_unit(dir);
}

t_point	ray_at(t_ray ray, float t)
{
	t_point at;

	at = vec_add(ray.orig, vec_multi_float(ray.dir, t));
	return (at);
}

t_ray	ray_primary(t_camera cam, double u, double v)
{
    t_ray	ray;

    ray.orig = cam.orig;
    // left_bottom + u * horizontal + v * vertical - origin 의 단위 벡터.
    ray.dir = vec_unit(vec_sub(vec_add(vec_add(cam.start_point, vec_multi_float(cam.horizontal, u)), vec_multi_float(cam.vertical, v)), cam.orig));
    return (ray);
}

t_color    ray_color(t_ray ray, t_sphere sp)
{
    float			t;
	t_vec			n;
	int				result;
	t_hit_record	rec;

	rec.tmin = 0;
    rec.tmax = 100000;
	// t = hit_sphere(sp, ray, rec);
	if (hit_sphere(sp, ray, &rec))
	{
		// n = vec_unit(vec_sub(ray_at(ray, t), sp.center));
		// printf("hey => x : %f | y : %f | z : %f\n", rec.normal.x + 1.0, rec.normal.y+ 1.0, rec.normal.z+ 1.0);
		// return (vec_multi_float(vec_init(n.x + 1, n.y + 1, n.z + 1), 127));
		return (vec_multi_float(vec_add(rec.normal, vec_init(1, 1, 1)), 127.5));
	}
	else
	{
		t = 0.5 * (ray.dir.y + 1.0);
		// (1-t) * 흰색 + t * 하늘색
		return (vec_add(vec_multi_float(vec_init(255, 255, 255), 1.0 - t), vec_multi_float(vec_init(128, 178, 255), t)));
	}	
}

t_camera	camera_init(t_canvas canvas, t_point orig)
{
    t_camera    cam;
    double      focal_len;
    double      viewport_height;

    viewport_height = 2.0;
    focal_len = 1.0;
    cam.orig = orig;
    cam.viewport_h = viewport_height;
    cam.viewport_w = viewport_height * canvas.aspect_ratio;
    cam.focal_len = focal_len;
    cam.horizontal = vec_init(cam.viewport_w, 0, 0);
    cam.vertical = vec_init(0, cam.viewport_h, 0);
    // 왼쪽 아래 코너점 좌표, origin - horizontal / 2 - vertical / 2 - vec3(0,0,focal_length)
    cam.start_point = vec_sub(vec_sub(vec_sub(cam.orig, vec_div_float(cam.horizontal, 2)),
                                vec_div_float(cam.vertical, 2)), vec_init(0, 0, focal_len));
    return (cam);
}

t_canvas	canvas_init(int  width, int height)
{
    t_canvas	init;

    init.width = width;
    init.height = height;
    init.aspect_ratio = (float)width / (float)height;
    return (init);
}
