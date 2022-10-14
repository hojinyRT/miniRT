/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   info_init.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jinypark <jinypark@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/13 15:59:24 by jinypark          #+#    #+#             */
/*   Updated: 2022/10/14 19:12:52 by jinypark         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minirt.h"

char	*check_first_file_name(int fd)
{
	char	*line;

	if (fd < 0)
		ft_strerror("Error \nfail to open file");
	line = get_next_line(fd);
	if (line == NULL)
		ft_strerror("Error \nattempt to open empty file");
	return (line);
}

void	info_init(t_info *info, char *file)
{
	char		**split;
	char		*line;
	int			fd;

	fd = open(file, O_RDONLY);
	line = check_first_file_name(fd);
	while (line)
	{
		if (line[0] == '#')
		{
			free(line);
			line = get_next_line(fd);
			continue ;
		}
		split = ft_split(line, ' ');
		if (split == NULL)
			ft_strerror("Error \nsplit fail");
		put_info(info, split);
		split_free(split);
		free(line);
		line = get_next_line(fd);
	}
	if (!info->light || !info->ambient || !info->camera)
		ft_strerror("Error \ninvalid .rt file");
}

int	check_format(char *format)
{
	if (!format)
		return (-1);
	if (!ft_strncmp(format, "sp", 3) || !ft_strncmp(format, "sp-ch", 6))
		return (SP);
	else if (!ft_strncmp(format, "pl", 3) || !ft_strncmp(format, "pl-ch", 6))
		return (PL);
	else if (!ft_strncmp(format, "cy", 3) || !ft_strncmp(format, "cy-ch", 6))
		return (CY);
	else if (!ft_strncmp(format, "cn", 3) || !ft_strncmp(format, "cn-ch", 6))
		return (CN);
	else if (!ft_strncmp(format, "A", 2))
		return (A);
	else if (!ft_strncmp(format, "C", 2))
		return (C);
	else if (!ft_strncmp(format, "L", 2))
		return (L);
	else
		ft_strerror("Error \ninvalid type in .rt file");
	return (-1);
}

void	put_info(t_info *info, char **argv)
{
	static void	(*run[7])(t_info *, char **, int) = {put_a, put_c, put_l, \
											put_sp, put_pl, put_cy, put_cn};
	int			type;
	int			argc;

	type = check_format(argv[0]);
	if (type == -1)
		return ;
	argc = 0;
	while (argv[argc])
		++argc;
	run[type](info, argv, argc);
}
