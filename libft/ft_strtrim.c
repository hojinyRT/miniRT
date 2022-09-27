/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strtrim.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jinypark <jinypark@student.42seoul.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/11 21:16:29 by jinypark          #+#    #+#             */
/*   Updated: 2022/07/18 13:41:58 by jinypark         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static int	is_set(char c, char const *set)
{
	int		i;

	i = 0;
	while (set[i])
		if (c == set[i++])
			return (1);
	return (0);
}

char	*ft_strtrim(char const *s1, char const *set)
{
	int		start;
	int		end;
	char	*arr;
	int		i;

	if (s1 == NULL)
		return (NULL);
	start = 0;
	while (s1[start] && is_set(s1[start], set))
		start++;
	end = ft_strlen(s1) - 1;
	while (end > start && is_set(s1[end], set))
		end--;
	arr = (char *)malloc(sizeof(char) * ((end - start + 1) + 1));
	if (arr == NULL)
		return (NULL);
	i = 0;
	while (start <= end)
		arr[i++] = s1[start++];
	arr[i] = '\0';
	return (arr);
}
