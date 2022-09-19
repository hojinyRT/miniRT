/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memmove.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jinypark <jinypark@student.42seoul.>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/11 20:13:35 by jinypark          #+#    #+#             */
/*   Updated: 2022/03/31 16:10:07 by jinypark         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memmove(void *dst, const void *src, size_t len)
{
	char		*tmp_dst;
	const char	*tmp_src;
	size_t		i;

	tmp_dst = dst;
	tmp_src = src;
	if (dst == src)
		return (dst);
	else if (dst < src)
	{
		i = 0;
		while (i++ < len)
			tmp_dst[i - 1] = tmp_src[i - 1];
	}
	else
	{
		while (len-- > 0)
			tmp_dst[len] = tmp_src[len];
	}
	return (dst);
}
