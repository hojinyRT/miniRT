#include "minirt.h"

double	vec_len_sqr(t_vec u)
{
	return (pow(u.x, 2.0) + pow(u.y, 2.0) + pow(u.z, 2.0));
}

t_vec	vec_unit(t_vec u)
{
	return (vec_div_double(u, vec_len(u)));
}
