#include "minirt.h"

t_vec  vec_min(t_vec vec1, t_vec vec2)
{
    if (vec1.x > vec2.x)
        vec1.x = vec2.x;
    if (vec1.y > vec2.y)
        vec1.y = vec2.y;
    if (vec1.z > vec2.z)
        vec1.z = vec2.z;
    return (vec1);
}


t_vec	vec_add(t_vec u, t_vec v)
{
    t_vec   init;

    init.x = u.x + v.x;
    init.y = u.y + v.y;
    init.z = u.z + v.z;
    return (init);
}

t_vec	vec_sub(t_vec u, t_vec v)
{
    t_vec   init;

    init.x = u.x - v.x;
    init.y = u.y - v.y;
    init.z = u.z - v.z;
    return (init);
}

t_vec	vec_multi(t_vec u, t_vec v)
{
    t_vec   init;

    init.x = u.x * v.x;
    init.y = u.y * v.y;
    init.z = u.z * v.z;
    return (init);
}

t_vec	vec_div(t_vec u, t_vec v)
{
    t_vec   init;

    if (v.x == 0 || v.y == 0 || v.z == 0)
        exit(1);
    init.x = u.x / v.x;
    init.y = u.y / v.y;
    init.z = u.z / v.z;
    return (init);
}

t_vec	vec_multi_float(t_vec u, float n)
{
    t_vec   init;

    init.x = u.x * n;
    init.y = u.y * n;
    init.z = u.z * n;
    return (init);
}

t_vec	vec_div_float(t_vec u, float n)
{
    return (vec_multi_float(u, (1 / n)));
}

float	vec_dot(t_vec u, t_vec v)
{
	return (u.x * v.x + u.y * v.y + u.z * v.z);
}

t_vec	vec_cross(t_vec u, t_vec v)
{
	t_vec init;
	
	init.x = u.y * v.z - u.z * v.y;
	init.y = u.z * v.x - u.x * v.z;
	init.z = u.x * v.y - u.y * v.x;
	return (init);
}

float	vec_len(t_vec u)
{
	return (sqrt(pow(u.x, 2.0) + pow(u.y, 2.0) + pow(u.z, 2.0)));
}
float	vec_len_sqr(t_vec u)
{
    return (pow(u.x, 2.0) + pow(u.y, 2.0) + pow(u.z, 2.0));
}
t_vec	vec_unit(t_vec u)
{
	return (vec_div_float(u, vec_len(u)));
}
t_vec	vec_init(float x, float y, float z)
{
	t_vec init;
	
	init.x = x;
	init.y = y;
	init.z = z;
	return (init);
}
