/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strmapi.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jinypark <jinypark@student.42seoul.>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/15 16:28:46 by jinypark          #+#    #+#             */
/*   Updated: 2022/03/31 16:11:59 by jinypark         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strmapi(char const *s, char (*f)(unsigned int, char))
{
	unsigned int	len;
	char			*arr;
	unsigned int	i;

	if (s == NULL || f == NULL)
		return (NULL);
	i = 0;
	len = ft_strlen(s);
	arr = (char *)malloc(sizeof(char) * (len + 1));
	if (arr == NULL)
		return (NULL);
	while (i < len)
	{
		arr[i] = f(i, s[i]);
		i++;
	}
	arr[i] = '\0';
	return (arr);
}
