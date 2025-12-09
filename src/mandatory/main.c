/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nkojima <nkojima@student.42tokyo.jp>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/29 23:30:41 by nkojima           #+#    #+#             */
/*   Updated: 2025/12/01 00:00:00 by nkojima          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/pipex.h"

int	main(int argc, char **argv, char **envp)
{
	int		pipe_fd[2];
	pid_t	pid1;
	pid_t	pid2;

	validate_argc(argc);
	create_pipe(pipe_fd);
	pid1 = fork();
	if (pid1 == SYSCALL_ERROR)
		error_exit("fork failed", EXIT_GENERAL_ERROR);
	if (pid1 == SYSCALL_SUCCESS)
		exec_child1(argv[1], argv[2], pipe_fd, envp);
	pid2 = fork();
	if (pid2 == SYSCALL_ERROR)
		error_exit("fork failed", EXIT_GENERAL_ERROR);
	if (pid2 == SYSCALL_SUCCESS)
		exec_child2(argv[4], argv[3], pipe_fd, envp);
	close_pipe_parent(pipe_fd);
	return (wait_and_get_exit_code(pid1, pid2));
}
