/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_itoa.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jinypark <jinypark@student.42seoul.>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/15 16:33:01 by jinypark          #+#    #+#             */
/*   Updated: 2022/03/31 16:08:52 by jinypark         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static size_t	ft_intlen(int n)
{
	int			len;
	long long	nb;

	nb = n;
	len = 0;
	if (nb < 0)
	{
		len++;
		nb *= -1;
	}
	if (nb == 0)
		return (1);
	while (nb > 0)
	{
		nb /= 10;
		len++;
	}
	return (len);
}

static void	ft_putnbr(char *arr, int n, size_t len)
{
	long long	nb;

	nb = n;
	if (nb < 0)
	{
		arr[0] = '-';
		nb *= -1;
	}
	if (nb == 0)
		arr[0] = '0';
	while (nb > 0)
	{
		arr[len - 1] = nb % 10 + '0';
		nb /= 10;
		len--;
	}
}

char	*ft_itoa(int n)
{
	size_t	len;
	char	*arr;

	len = ft_intlen(n);
	arr = (char *)malloc(sizeof(char) * (len + 1));
	if (arr == NULL)
		return (NULL);
	ft_putnbr(arr, n, len);
	arr[len] = '\0';
	return (arr);
}
