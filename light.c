#include "minirt.h"

int in_shadow(t_object *objs, t_ray light_ray, double light_len)
{
    t_hit_record rec;

    rec.tmin = 0;
    rec.tmax = light_len;
    if (hit(objs, light_ray, &rec))
        return (TRUE);
    return (FALSE);
}

t_vec          reflect(t_vec v, t_vec n)
{
    //v - 2 * dot(v, n) * n;
    return (vec_sub(v, vec_multi_double(n, vec_dot(v, n) * 2)));
}


t_vec        point_light_get(t_scene *scene, t_light *light)
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

    // light_dir = vec_unit(vec_sub(light->origin, scene->rec.p)); //교점에서 출발하여 광원을 향하는 벡터(정규화 됨)

    // 추가
    // printf("light origin : %f, %f, %f\n", light->origin.x, light->origin.y, light->origin.z);
    
    light_dir = vec_sub(light->origin, scene->rec.p);
    light_len = vec_len(light_dir);
    // printf("light len: %f\n", light_len);
    light_ray = ray_init(vec_add(scene->rec.p, vec_multi_double(scene->rec.normal, EPSILON)), light_dir);
    // light_ray = ray_init(vec_add(scene->rec.p, vec_multi_double(scene->rec.normal, EPSILON)), light_dir);
    // light_ray = ray_init(vec_add(scene->rec.p, vec_init(0.01, 0.01, 0.01)), light_dir);
    // light_ray = ray_init(scene->rec.p, light_dir);
    if (in_shadow(scene->obj, light_ray, light_len))
    {
        return (vec_init(0,0,0));
    }
    light_dir = vec_unit(light_dir);
    // 추가끝
    // cosΘ는 Θ 값이 90도 일 때 0이고 Θ가 둔각이 되면 음수가 되므로 0.0보다 작은 경우는 0.0으로 대체한다.
    kd = fmax(vec_dot(scene->rec.normal, light_dir), 0.0);// (교점에서 출발하여 광원을 향하는 벡터)와 (교점에서의 법선벡터)의 내적값.
    diffuse = vec_multi_double(light->light_color, kd);

    view_dir = vec_unit(vec_multi_double(scene->ray.dir, -1));
    reflect_dir = reflect(vec_multi_double(light_dir, -1), scene->rec.normal);
    ksn = 64; // shininess value
    ks = 0.5; // specular strength
    spec = pow(fmax(vec_dot(view_dir, reflect_dir), 0.0), ksn);
    specular = vec_multi_double(vec_multi_double(light->light_color, ks), spec);
    brightness = light->bright_ratio * LUMEN; // 기준 광속/광량을 정의한 매크로
    return (vec_multi_double(vec_add(diffuse, specular), brightness));
}

t_vec	phong_lighting(t_scene *scene)
{
    t_color		light_color;
    t_object	*lights;

    light_color = vec_init(0, 0, 0); //광원이 하나도 없다면, 빛의 양은 (0, 0, 0)일 것이다.
    lights = scene->light;
    while (lights) //여러 광원에서 나오는 모든 빛에 대해 각각 diffuse, specular 값을 모두 구해줘야 한다
    {
        if(lights->type == LIGHT_POINT)
            light_color = vec_add(light_color, point_light_get(scene, lights->element));
        lights = lights->next;
    }
    light_color = vec_add(light_color, scene->ambient);
    return (vec_multi_double(vec_min(vec_multi(light_color, scene->rec.albedo), vec_init(1, 1, 1)), 255));
    //모든 광원에 의한 빛의 양을 구한 후, 오브젝트의 반사율과 곱해준다. 그 값이 (1, 1, 1)을 넘으면 (1, 1, 1)을 반환한다.
}

t_light     *light_point(t_vec light_origin, t_vec light_color, double bright_ratio)
{
    t_light *light;

    if(!(light = (t_light *)malloc(sizeof(t_light))))
        return (NULL);
    light->origin = light_origin;
    light->light_color = light_color;
    light->bright_ratio = bright_ratio;
    return (light);
}