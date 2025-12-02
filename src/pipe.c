/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nkojima <nkojima@student.42tokyo.jp>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/01 00:00:00 by nkojima           #+#    #+#             */
/*   Updated: 2025/12/01 00:00:00 by nkojima          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/pipex.h"

void	create_pipe(int pipe_fd[2])
{
	if (pipe(pipe_fd) == SYSCALL_ERROR)
		error_exit("pipe failed", EXIT_GENERAL_ERROR);
}

void	close_pipe_parent(int pipe_fd[2])
{
	close(pipe_fd[0]);
	close(pipe_fd[1]);
}
