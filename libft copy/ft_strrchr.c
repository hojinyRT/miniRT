/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strrchr.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jinypark <jinypark@student.42seoul.>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/11 20:31:21 by jinypark          #+#    #+#             */
/*   Updated: 2022/03/31 16:12:15 by jinypark         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strrchr(const char *s, int c)
{
	char	*tmp;
	size_t	i;

	tmp = NULL;
	i = 0;
	while (s[i])
	{
		if (s[i] == (char)c)
			tmp = (char *)&s[i];
		i++;
	}
	if (c == 0)
		tmp = ((char *)&s[i]);
	return (tmp);
}
