#include "minirt.h"

t_vec	vec_div_double(t_vec u, double n)
{
    // t_vec   init;

    // init.x = u.x / n;
    // init.y = u.y / n;
    // init.z = u.z / n;
    // return (init);
    return (vec_multi_double(u, (1 / n)));
}

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

double	vec_dot(t_vec u, t_vec v)
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

double	vec_len(t_vec u)
{
	return (sqrt(pow(u.x, 2.0) + pow(u.y, 2.0) + pow(u.z, 2.0)));
}
