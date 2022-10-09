/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line_utils.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jinypark <jinypark@student.42seoul.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/07 15:07:04 by jinypark          #+#    #+#             */
/*   Updated: 2022/10/09 14:11:04 by jinypark         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"

t_list	*ft_lstnew(int fd)
{
	t_list	*new;

	new = (t_list *)malloc(sizeof(t_list));
	if (!new)
		return (NULL);
	new->fd = fd;
	new->next = NULL;
	new->content = NULL;
	return (new);
}

int	ft_strchr(const char *s, int c)
{
	int	i;

	i = 0;
	if (s == NULL)
		return (-1);
	while (s[i])
	{
		if ((char)c == s[i])
			return (i);
		i++;
	}
	if (c == '\0')
		return (i);
	return (-1);
}

char	*ft_strcat(char *dest, char const *src)
{
	size_t	dest_len;
	size_t	i;

	dest_len = ft_strchr(dest, '\0');
	i = 0;
	while (src[i])
	{
		dest[dest_len + i] = src[i];
		i++;
	}
	dest[dest_len + i] = '\0';
	return (dest);
}

char	*ft_strdup_g(const char *s1)
{
	char	*new;
	size_t	i;

	i = 0;
	new = (char *)malloc(sizeof(char) * (ft_strchr(s1, '\0') + 1));
	if (new == NULL)
		return (NULL);
	while (s1[i])
	{
		new[i] = s1[i];
		i++;
	}
	new[i] = '\0';
	return (new);
}

char	*ft_strjoin_g(char *s1, char *s2)
{
	char	*arr;
	size_t	len;

	if (s2 == NULL)
		return (NULL);
	if (s1 == NULL)
		s1 = ft_strdup_g("");
	len = ft_strchr(s1, '\0') + ft_strchr(s2, '\0');
	arr = (char *)malloc(sizeof(char) * (len + 1));
	if (arr == NULL)
		return (NULL);
	arr[0] = '\0';
	ft_strcat(arr, s1);
	ft_strcat(arr, s2);
	free(s1);
	return (arr);
}
