/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.h                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jinypark <jinypark@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/05 17:59:58 by jinypark          #+#    #+#             */
/*   Updated: 2022/10/14 10:36:06 by jinypark         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef GET_NEXT_LINE_H
# define GET_NEXT_LINE_H

# ifndef BUFFER_SIZE
#  define BUFFER_SIZE 10000
# endif

# include <unistd.h>
# include <stdlib.h>

typedef struct s_list
{
	int				fd;
	struct s_list	*next;
	char			*content;
}				t_list;

t_list	*ft_lstnew(int fd);
int		ft_strchr(const char *s, int c);
char	*ft_strcat(char *dest, char const *src);
char	*ft_strdup_g(const char *s1);
char	*ft_strjoin_g(char *s1, char *s2);
char	*trim_line(char **keep);
t_list	*find_node(t_list **node, int fd);
void	lstclear(t_list **keep, t_list **node);
char	*get_eof_line(t_list **head, t_list **node);
char	*get_next_line(int fd);

#endif
