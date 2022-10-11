#include "minirt.h"

t_object    *object_init(t_object_type type, void *element, t_vec albedo, int checker)
{
    t_object    *new;

    if (!(new = (t_object *)calloc(1, sizeof(t_object))))
        return (NULL);
    new->type = type;
    new->element = element;
    new->albedo = albedo;
    new->checker = checker;
    new->next = NULL;
    return (new);
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

t_plane	*plane_init(t_point center, t_vec normal, double radius)
{
	t_plane *init;

    if(!(init = (t_plane *)malloc(sizeof(t_plane))))
        return (NULL);

	init->center = center;
	init->normal = normal;
    init->radius = radius;
    printf("plane %lf %lf %lf\n", center.x, center.y, center.z);
	return (init);
}

t_cylinder	*cylinder_init(t_point center, double radius, double height, t_vec normal)
{
	t_cylinder *init;

    if(!(init = (t_cylinder *)malloc(sizeof(t_cylinder))))
        return (NULL);
	init->center = center;
	init->radius = radius;
    init->radius2 = radius * radius;
	init->height = height;
    init->normal = normal;
    printf("%lf %lf %lf\n", center.x, center.y, center.z);
	return (init);
}

t_cone	*cone_init(t_point center, double radius, double height, t_vec normal)
{
	t_cone *init;

    if(!(init = (t_cone *)malloc(sizeof(t_cone))))
        return (NULL);
	init->center = center;
	init->radius = radius;
    init->radius2 = radius * radius;
	init->height = height;
    init->normal = normal;
    printf("%lf %lf %lf\n", center.x, center.y, center.z);
	return (init);
}
