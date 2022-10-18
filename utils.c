/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hchang <hchang@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/13 15:46:19 by jinypark          #+#    #+#             */
/*   Updated: 2022/10/18 14:49:56 by hchang           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"

void	is_sign(char *str, int *idx, double *sign)
{
	*idx = -1;
	*sign = 1.0;
	if (str[++(*idx)] == '+' || str[*idx] == '-')
	{
		if (str[(*idx) + 1] == '\0')
			ft_strerror("Error \ninvalid number format");
		if (str[*idx] == '-')
			*sign *= -1.0;
		return ;
	}
	(*idx)--;
}

double	convert_decimal(char *str, double res, int idx, double sign)
{
	double	decimal;
	int		flag;

	flag = 0;
	decimal = 0.1;
	if (str[idx - 1] == '.' && !str[idx])
		ft_strerror("Error \ninvalid number format");
	while (str[idx])
	{
		if (!ft_isdigit(str[idx]))
			ft_strerror("Error \ninvalid number format");
		if (flag++ > 6)
		{
			++idx;
			continue ;
		}
		res = res + (str[idx] - '0') * decimal;
		decimal /= 10;
		++idx;
	}
	if ((sign == -1 && res > 2147483648) || (sign == 1 && res > 2147483647))
		ft_strerror("Error \ninvalid number scope");
	return (sign * res);
}

double	ft_atod(char *str)
{
	int		idx;
	int		flag;
	double	sign;
	double	res;

	flag = 0;
	res = 0.0;
	is_sign(str, &idx, &sign);
	while (str[++idx])
	{
		if ((flag++ == 0 && str[idx] == '.')
			|| ((!ft_isdigit(str[idx]) && str[idx] != '.')))
			ft_strerror("Error \ninvalid number");
		if (str[idx] == '.')
		{
			++idx;
			break ;
		}
		res = (str[idx] - '0') + (res * 10);
		if ((sign == 1 && res > 2147483647) || (sign == -1 && res > 2147483648))
			ft_strerror("Error \ninvalid number scope");
	}
	return (convert_decimal(str, res, idx, sign));
}

void	check_unit(double *x, double *y, double *z, int flag)
{
	t_vec	tmp;

	if (flag == RGB)
	{
		if (*x > 255 || *x < 0 || *y > 255 || *y < 0 || *z > 255 || *z < 0)
			ft_strerror("Error \ninvalid vector scope");
	}
	else if (flag == XYZ || flag == UNIT)
	{
		if (*x > 2147483647 || *x < -2147483648 || *y > 2147483647 \
		|| *y < -2147483648 || *z > 2147483647 || *z < -2147483648)
			ft_strerror("Error \ninvalid vector scope");
		if (flag == UNIT)
		{
			if (*x == 0 && *y == 0 && *z == 0)
				ft_strerror("Error \ninvalid vector value");
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
			++cnt;
	}
	if (cnt != 2)
		ft_strerror("Error \ninvalid vector format");
	vec = ft_split(str, ',');
	i = -1;
	while (vec[++i])
		;
	if (i != 3)
		ft_strerror("Error \ninvalid vector format");
	res = vec_init(ft_atod(vec[0]), ft_atod(vec[1]), ft_atod(vec[2]));
	check_unit(&res.x, &res.y, &res.z, flag);
	split_free(vec);
	return (res);
}
