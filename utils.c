#include "minirt.h"

void	split_free(char **split)
{
	int		i;

	i = 0;
	while (split[i])
	{
		free(split[i]);
		++i;
	}
	free(split);
}

void	ft_strerror(char *err)
{
	ft_putendl_fd(err, 2);
	exit(1);
}

void	is_sign(char *str, int *idx, double *sign)
{
	*idx = -1;
	*sign = 1.0;
	if (str[++(*idx)] == '+' || str[*idx] == '-')
	{
		if (str[*idx] == '-')
			*sign *= -1.0;
        return ;
	}
    (*idx)--;
}

double	ft_atod(char *str)
{
	int		idx;
	int		flag;
	double	sign;
	double	res;
	double	decimal;
	
	flag = 0;
    res = 0.0;
	is_sign(str, &idx, &sign);
	while (str[++idx])
	{
		if ((flag++ == 0 && str[idx] == '.') || ((!ft_isdigit(str[idx]) && str[idx] != '.')))
			ft_strerror("Error : invalid number");
		if (str[idx] == '.')
		{
			idx++;
			break ;
		}
		res = (str[idx] - '0') + (res * 10);
		if ((sign == 1 &&res > 2147483647) || (sign == -1 && res > 2147483648))
			ft_strerror("Error : invalid number scope");
	}
	flag = 0;
	decimal = 0.1;
    if (str[idx - 1] == '.' && !str[idx])
        ft_strerror("Error : invalid number format");
	while(str[idx])
	{
		if (!ft_isdigit(str[idx]))
			ft_strerror("Error : invalid number format");
		if (flag++ > 6)
        {
            idx++;
			continue ;
        }
		res = res + (str[idx] - '0') * decimal;
		decimal /= 10;
		idx++;
	}
    if ((sign == -1 && res > 2147483648) || (sign == 1 && res > 2147483647))
        ft_strerror("Error : invalid number scope");
	return (sign * res);
}

void	check_unit(double *x, double *y, double *z, int flag)
{
	t_vec tmp;

	if (flag == RGB)
	{
		if (*x > 255 || *x < 0 || *y > 255 || *y < 0 || *z > 255 || *z < 0)
			ft_strerror("Error : invalid vector scope");
	}
	else if (flag == XYZ || flag == UNIT)
	{
		if (*x > 2147483647 || *x < -2147483648 || *y > 2147483647 \
		|| *y < -2147483648 || *z > 2147483647 || *z < -2147483648)
			ft_strerror("Error : invalid vector scope");
		if (flag == UNIT)
		{
			if (*x == 0 && *y == 0 && *z == 0)
				ft_strerror("Error : invalid vector value");
			tmp = vec_unit(vec_init(*x, *y, *z));
			*x = tmp.x;
			*y = tmp.y;
			*z = tmp.z;
		}
	}
}

t_vec	ft_atovec(char *str, int flag)
{
	t_vec	res;
	char	**vec;
	int		i;
	int		cnt;

	cnt = 0;
	i = -1;
	while (str[++i])
	{
		if (str[i] == ',')
			cnt++;
	}
	if (cnt != 2)
		ft_strerror("Error : invalid vector format");
	vec = ft_split(str, ',');
	i = -1;
	while (vec[++i])
		;
	if (i != 3)
		ft_strerror("Error : invalid vector format");
	res.x = ft_atod(vec[0]);
	res.y = ft_atod(vec[1]);
	res.z = ft_atod(vec[2]);
	check_unit(&res.x, &res.y, &res.z, flag);
	split_free(vec);
	return (res);
}