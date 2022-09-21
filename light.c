#include "minirt.h"

t_vec	phong_lighting(t_scene *scene)
{
    t_vec		light_color;
    t_object	*lights;

    light_color = vec_init(0, 0, 0); //광원이 하나도 없다면, 빛의 양은 (0, 0, 0)일 것이다.
    lights = scene->light;
    // while (lights) //여러 광원에서 나오는 모든 빛에 대해 각각 diffuse, specular 값을 모두 구해줘야 한다
    // {
    //     if(lights->type == LIGHT_POINT)
    //         light_color = vec_add(light_color, point_light_get(scene, lights->element));
    //     lights = lights->next;
    // }
    light_color = vec_add(light_color, scene->ambient);
    return (vec_sub(vec_multi(light_color, scene->rec.albedo), vec_init(1, 1, 1)));
    //모든 광원에 의한 빛의 양을 구한 후, 오브젝트의 반사율과 곱해준다. 그 값이 (1, 1, 1)을 넘으면 (1, 1, 1)을 반환한다.
}

t_light     *light_point(t_vec light_origin, t_vec light_color, float bright_ratio)
{
    t_light *light;

    if(!(light = (t_light *)malloc(sizeof(t_light))))
        return (NULL);
    light->origin = light_origin;
    light->light_color = light_color;
    light->bright_ratio = bright_ratio;
    return (light);
}