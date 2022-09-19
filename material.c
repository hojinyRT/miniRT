#include "minirt.h"

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
    ray.dir = vec_unit(vec_sub(vec_add(vec_add(cam.start_point, vec_multi_float(cam.horizontal, u)), vec_multi_float(cam->vertical, v)), cam->orig));
    return (ray);
}

t_color    ray_color(t_ray ray)
{
    double  t;

    t = 0.5 * (ray.dir.y + 1.0);
    // (1-t) * 흰색 + t * 하늘색
    return (vec_add(vec_multi_float(vec_init(1, 1, 1), 1.0 - t), vec_multi_float(vec_init(0.5, 0.7, 1.0), t)));
}

t_camera	camera(t_canvas *canvas, t_point orig)
{
    t_camera    cam;
    double      focal_len;
    double      viewport_height;

    viewport_height = 2.0;
    focal_len = 1.0;
    cam.orig = orig;
    cam.viewport_h = viewport_height;
    cam.viewport_w = viewport_height * canvas->aspect_ratio;
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