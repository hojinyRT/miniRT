/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_atoi.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jinypark <jinypark@student.42seoul.>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/11 17:48:36 by jinypark          #+#    #+#             */
/*   Updated: 2022/03/31 16:55:57 by jinypark         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <limits.h>

static int	check_space(const char *str, int i)
{
	while (str[i] == ' ' || ('\t' <= str[i] && str[i] <= '\r'))
		i++;
	return (i);
}

static int	num_count(const char *str, int i, int sign)
{
	unsigned long	nbr;
	int				n;

	nbr = 0;
	while ('0' <= str[i] && str[i] <= '9')
	{
		n = str[i] - '0';
		if (sign == 1 && (nbr > LONG_MAX / 10 || \
					(nbr == LONG_MAX / 10 && n > LONG_MAX % 10)))
			return ((int)LONG_MAX);
		else if (sign == -1 && (nbr > LONG_MAX / 10 || \
					(nbr == LONG_MAX / 10 && n > -(LONG_MIN % 10))))
			return ((int)LONG_MIN);
		nbr = nbr * 10 + n;
		i++;
	}
	return (nbr);
}

static int	check_sign(char c)
{
	if (c == '+')
		return (1);
	else if (c == '-')
		return (-1);
	return (0);
}

int	ft_atoi(const char *str)
{
	int			i;
	int			sign;

	i = 0;
	sign = 1;
	i = check_space(str, i);
	if (check_sign(str[i]))
		sign = check_sign(str[i++]);
	return (sign * num_count(str, i, sign));
}
