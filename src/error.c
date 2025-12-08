/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nkojima <nkojima@student.42tokyo.jp>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/01 00:00:00 by nkojima           #+#    #+#             */
/*   Updated: 2025/12/01 00:00:00 by nkojima          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/pipex.h"

void	validate_argc(int argc)
{
	if (argc != REQUIRED_ARGC)
	{
		ft_putendl_fd("Usage: ./pipex file1 cmd1 cmd2 file2", FD_STDERR);
		exit(EXIT_GENERAL_ERROR);
	}
}

void	error_exit(char *msg, int exit_code)
{
	ft_putstr_fd("pipex: ", FD_STDERR);
	ft_putendl_fd(msg, FD_STDERR);
	exit(exit_code);
}

void	handle_exec_error(char *cmd)
{
	ft_putstr_fd("pipex: ", FD_STDERR);
	ft_putstr_fd(cmd, FD_STDERR);
	if (errno == ENOENT)
	{
		ft_putendl_fd(": command not found", FD_STDERR);
		exit(EXIT_CMD_NOT_FOUND);
	}
	else if (errno == EACCES)
	{
		ft_putendl_fd(": Permission denied", FD_STDERR);
		exit(EXIT_CMD_NOT_EXECUTABLE);
	}
	else
	{
		ft_putstr_fd(": ", FD_STDERR);
		ft_putendl_fd(strerror(errno), FD_STDERR);
		exit(EXIT_GENERAL_ERROR);
	}
}

int	wait_and_get_exit_code(pid_t pid1, pid_t pid2)
{
	int	status;
	int	exit_code;

	if (waitpid(pid1, &status, SYSCALL_SUCCESS) == SYSCALL_ERROR)
		error_exit("waitpid failed", EXIT_GENERAL_ERROR);
	if (waitpid(pid2, &status, SYSCALL_SUCCESS) == SYSCALL_ERROR)
		error_exit("waitpid failed", EXIT_GENERAL_ERROR);
	if (WIFEXITED(status))
		exit_code = WEXITSTATUS(status);
	else if (WIFSIGNALED(status))
		exit_code = EXIT_SIGNAL_BASE + WTERMSIG(status);
	else
		exit_code = EXIT_GENERAL_ERROR;
	return (exit_code);
}
