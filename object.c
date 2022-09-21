#include "minirt.h"

t_object    *object_init(t_object_type type, void *element, t_vec albedo)
{
    t_object    *new;

    if (!(new = (t_object *)calloc(1, sizeof(t_object))))
        return (NULL);
    new->type = type;
    new->element = element;
    new->albedo = albedo;
    new->next = NULL;
    return (new);
}

void    obj_add(t_object **list, t_object *new)
{
    t_object    *cur;

    if (list == NULL)
        return ;
    if (*list == NULL)
    {
        *list = new;
        return ;
    }
    cur = *list;
    while (cur->next)
        cur = cur->next;
    cur->next = new;
}

t_object    *obj_last(t_object *list)
{
    if (list == NULL)
        return (NULL);
    while (list->next)
        list = list->next;
    return (list);
}

int hit(t_object *obj, t_ray ray, t_hit_record *rec)
{
	int				hit_anything;
    t_hit_record    temp_rec;

    temp_rec = *rec; // temp_rec의 tmin, tmax 값 초기화를 위해.
    hit_anything = FALSE;
    while(obj)
    {
        // printf("START OBJ : %f\n",((t_sphere*)obj->element)->center.x);
        if (hit_obj(obj, ray, &temp_rec))
        {
            hit_anything = TRUE;
            temp_rec.tmax = temp_rec.t;
            *rec = temp_rec;
        }
        obj = obj->next;
    }
    return (hit_anything);
}

int hit_obj(t_object *obj, t_ray ray, t_hit_record *rec)
{
    int  hit_result;

    hit_result = FALSE;
    if (obj->type == SP)
        hit_result = hit_sphere(obj, ray, rec); //hit_sphere의 첫번째 인자도 t_sphere *에서 t_object *로 수정해주자.
    return (hit_result);
}