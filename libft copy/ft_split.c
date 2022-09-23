/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jinypark <jinypark@student.42seoul.>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/17 19:48:45 by jinypark          #+#    #+#             */
/*   Updated: 2022/03/31 19:38:58 by jinypark         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static int	count_str(const char *str, char c)
{
	size_t	i;
	size_t	cnt;
	int		flag;

	cnt = 0;
	i = 0;
	flag = 0;
	while (str[i])
	{
		if (str[i] != c && flag == 0)
		{
			cnt++;
			flag = 1;
		}
		else if (str[i] == c)
			flag = 0;
		i++;
	}
	return (cnt);
}

static size_t	put_str(char **arr, const char *str, char c, size_t cnt)
{
	size_t	len;
	char	*tmp;
	size_t	i;

	len = 0;
	i = 0;
	while (str[len] && str[len] != c)
		len++;
	tmp = (char *)malloc(sizeof(char) * (len + 1));
	if (tmp == NULL)
		return (0);
	while (i < len)
	{
		tmp[i] = str[i];
		i++;
	}
	tmp[i] = '\0';
	arr[cnt] = tmp;
	return (len);
}

static int	make_arr(const char *str, char c, char **arr)
{
	size_t	cnt;
	size_t	i;
	size_t	len;

	cnt = 0;
	i = 0;
	len = 0;
	while (str[i])
	{
		if (str[i] != c)
		{
			len = put_str(arr, &str[i], c, cnt++);
			if (len)
				i += len;
			else
				return (--cnt);
		}
		while (str[i] && str[i] == c)
			i++;
	}
	arr[cnt] = NULL;
	return (-1);
}

char	**ft_split(const char *str, char c)
{
	char	**arr;
	int		cnt;

	if (str == NULL)
		return (NULL);
	arr = (char **)malloc(sizeof(char *) * (count_str(str, c) + 1));
	if (arr == NULL)
		return (NULL);
	cnt = make_arr(str, c, arr);
	if (cnt >= 0)
	{
		while (cnt >= 0)
			free(arr[cnt--]);
		free(arr);
		return (NULL);
	}
	return (arr);
}
