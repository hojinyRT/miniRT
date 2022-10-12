/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_atoi_base.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hchang <hchang@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/14 14:38:31 by jinypark          #+#    #+#             */
/*   Updated: 2022/10/12 13:38:17 by hchang           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../libft/libft.h"
#include <stdio.h>

static int	base_validation(char *b)
{
	int	len;
	int	i;
	int	j;

	i = 0;
	len = 0;
	while (b[len] != '\0')
		len++;
	if (len <= 1)
		return (0);
	while (b[i] != '\0')
	{
		if (b[i] == '-' || b[i] == '+' || b[i] == ' ' || b[i] == '\r' || \
			b[i] == '\t' || b[i] == '\n' || b[i] == '\v' || b[i] == '\f')
			return (0);
		j = 1;
		while (b[i + j] != '\0')
		{
			if (b[i] == b[i + j])
				return (0);
			j++;
		}
		i++;
	}
	return (len);
}

static int	check_str(char c, char *base)
{
	int	len;
	int	i;

	len = 0;
	i = 0;
	while (base[len] != '\0')
		len++;
	if (c == '-')
		return (-1);
	else if (c == '+')
		return (1);
	while (i < len)
	{
		if (ft_tolower(c) == base[i])
			return (2);
		i++;
	}
	return (0);
}

static int	ft_ctoi(char c, char *base)
{
	int		len;
	int		i;

	len = 0;
	c = ft_tolower(c);
	while (base[len] != '\0')
		len++;
	i = 0;
	while (i < len)
	{
		if (c == base[i])
			return (i);
		i++;
	}
	return (0);
}

static int	put_nbr(char *str, int i, int sign, char *base)
{
	long long	nbr;

	nbr = 0;
	while (check_str(str[i], base) == 2)
	{
		nbr = nbr * base_validation(base) + ft_ctoi(str[i], base);
		i++;
	}
	nbr *= sign;
	return (nbr);
}

int	ft_atoi_base(char *str, char *base)
{
	int		i;
	int		sign;

	i = 0;
	sign = 1;
	if (base_validation(base) == 0)
		return (0);
	while (str[i] == ' ' || ('\t' <= str[i] && str[i] <= '\r'))
		i++;
	while (str[i] != '\0')
	{
		if (check_str(str[i], base) == -1 || check_str(str[i], base) == 1)
			sign *= (check_str(str[i], base));
		else if (check_str(str[i], base) == 2)
			return (put_nbr(str, i, sign, base));
		else
			break ;
		i++;
	}
	return (0);
}
