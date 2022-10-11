#include "minirt.h"

t_vec	vec_init(double x, double y, double z)
{
	t_vec init;
	
	init.x = x;
	init.y = y;
	init.z = z;
	return (init);
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


t_vec	vec_multi_double(t_vec u, double n)
{
    t_vec   init;

    init.x = u.x * n;
    init.y = u.y * n;
    init.z = u.z * n;
    return (init);
}