#include "minirt.h"

void record_init(t_hit_record *rec)
{
	ft_memset(rec, 0, sizeof(t_hit_record));
    rec->tmin = EPSILON;
    rec->tmax = 10000000;
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