/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstmap.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jinypark <jinypark@student.42seoul.>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/18 17:42:23 by jinypark          #+#    #+#             */
/*   Updated: 2022/03/31 17:01:10 by jinypark         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

t_list	*ft_lstmap(t_list *lst, void *(*f)(void *), void (*del)(void *))
{
	t_list	*tmp;
	t_list	*first;

	if (lst == NULL || f == NULL || del == NULL)
		return (NULL);
	tmp = ft_lstnew(f(lst->content));
	if (tmp == NULL)
		return (NULL);
	first = tmp;
	while (lst->next)
	{
		tmp->next = ft_lstnew(f(lst->next->content));
		if (!tmp->next)
		{
			ft_lstclear(&first, del);
			return (NULL);
		}
		tmp = tmp->next;
		lst = lst->next;
	}
	return (first);
}
