/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   file.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nkojima <nkojima@student.42tokyo.jp>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/01 00:00:00 by nkojima           #+#    #+#             */
/*   Updated: 2025/12/01 00:00:00 by nkojima          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/pipex.h"

int	open_infile(char *filename)
{
	int	fd;

	fd = open(filename, O_RDONLY);
	if (fd == SYSCALL_ERROR)
	{
		ft_putstr_fd("pipex: ", FD_STDERR);
		ft_putstr_fd(filename, FD_STDERR);
		ft_putstr_fd(": ", FD_STDERR);
		ft_putendl_fd(strerror(errno), FD_STDERR);
		return (open("/dev/null", O_RDONLY));
	}
	return (fd);
}

int	open_outfile(char *filename)
{
	int	fd;

	fd = open(filename, O_WRONLY | O_CREAT | O_TRUNC, FILE_PERMISSION);
	if (fd == SYSCALL_ERROR)
	{
		ft_putstr_fd("pipex: ", FD_STDERR);
		ft_putstr_fd(filename, FD_STDERR);
		ft_putstr_fd(": ", FD_STDERR);
		ft_putendl_fd(strerror(errno), FD_STDERR);
		exit(EXIT_GENERAL_ERROR);
	}
	return (fd);
}
