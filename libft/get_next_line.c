/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jinypark <jinypark@student.42seoul.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/05 17:34:28 by jinypark          #+#    #+#             */
/*   Updated: 2022/10/09 14:10:56 by jinypark         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"
#include <stdio.h>

char	*trim_line(char **keep)
{
	size_t	len;
	char	*answer;
	size_t	i;
	char	*tmp;

	len = ft_strchr(*keep, '\n');
	i = 0;
	answer = (char *)malloc(sizeof(char) * (len + 1));
	if (answer == NULL)
		return (NULL);
	while (i < len)
	{
		answer[i] = (*keep)[i];
		i++;
	}
	answer[i] = '\0';
	tmp = ft_strdup_g(&((*keep)[i + 1]));
	free(*keep);
	*keep = tmp;
	return (answer);
}

t_list	*find_node(t_list **node, int fd)
{
	t_list	*tmp;

	tmp = *node;
	while (tmp)
	{
		if (tmp->fd == fd)
			break ;
		if (tmp->next == NULL)
		{
			tmp->next = ft_lstnew(fd);
			if (tmp->next == NULL)
				return (NULL);
		}
		tmp = tmp->next;
	}
	return (tmp);
}

void	lstclear(t_list **keep, t_list **node)
{
	t_list	*head;

	head = *keep;
	if ((*keep) == *node)
	{
		*keep = (*node)->next;
		free(*node);
		return ;
	}
	while ((*keep)->next)
	{
		if ((*keep)->next == *node)
		{
			(*keep)->next = (*keep)->next->next;
			break ;
		}
		(*keep) = (*keep)->next;
	}
	free(*node);
	*keep = head;
}

char	*get_eof_line(t_list **head, t_list **node)
{
	char	*answer;
	t_list	*tmp;

	tmp = *node;
	if (tmp->content == NULL)
	{
		lstclear(head, node);
		return (NULL);
	}
	if (*tmp->content == '\0')
	{
		free(tmp->content);
		tmp->content = NULL;
		lstclear(head, node);
		return (NULL);
	}
	answer = tmp->content;
	tmp->content = NULL;
	lstclear(head, node);
	return (answer);
}

char	*get_next_line(int fd)
{
	static t_list	*keep;
	char			buffer[BUFFER_SIZE + 1];
	ssize_t			r;
	t_list			*fd_node;

	if (fd < 0 || BUFFER_SIZE < 0)
		return (NULL);
	if (keep == NULL)
		keep = ft_lstnew(fd);
	fd_node = find_node(&keep, fd);
	if (ft_strchr(fd_node->content, '\n') >= 0)
		return (trim_line(&(fd_node->content)));
	r = read(fd, buffer, BUFFER_SIZE);
	while (r > 0)
	{
		buffer[r] = '\0';
		fd_node->content = ft_strjoin_g(fd_node->content, buffer);
		if (ft_strchr(fd_node->content, '\n') >= 0)
			return (trim_line(&(fd_node->content)));
		r = read(fd, buffer, BUFFER_SIZE);
	}
	if (r <= 0)
		return (get_eof_line(&keep, &fd_node));
	return (NULL);
}
