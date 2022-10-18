/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils3.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hchang <hchang@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/13 15:50:37 by jinypark          #+#    #+#             */
/*   Updated: 2022/10/18 14:50:09 by hchang           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"

void	ft_strerror(char *err)
{
	ft_putendl_fd(err, 2);
	exit(1);
}

static int	check_sign(char *strs, int *sign)
{
	int				idx;

	idx = 0;
	if (strs[0] == '+' && ft_isdigit(strs[1]))
		idx = 1;
	else if (strs[0] == '-' && ft_isdigit(strs[1]))
	{
		*sign = -1;
		idx = 1;
	}
	return (idx);
}

int	my_atoi(const char *str)
{
	unsigned int	num;
	int				result;
	int				i;
	int				sign;

	i = 0;
	while (str[i])
	{
		num = 0;
		sign = 1;
		i = check_sign((char *)str, &sign);
		while (str[i])
		{
			if (num > 214748364
				|| (num == 214748364 && (7 + (sign < 0)) < str[i] - '0')
				|| !ft_isdigit(str[i]))
				ft_strerror("Error\natoi error");
			num = (num * 10) + (str[i++] - '0');
		}
		result = sign * num;
		if ((sign == 1 && result < 0) || (sign == -1 && result > 0))
			ft_strerror("Error\natoi error");
	}
	return (result);
}

void	*my_calloc(size_t count, size_t size)
{
	void	*new;

	new = ft_calloc(count, size);
	if (!new)
		ft_strerror("Error\nmemory allocation failed");
	return (new);
}

void	split_free(char **split)
{
	int	i;

	i = 0;
	while (split[i])
	{
		free(split[i]);
		++i;
	}
	free(split);
}
