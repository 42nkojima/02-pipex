/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   process.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nkojima <nkojima@student.42tokyo.jp>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/01 00:00:00 by nkojima           #+#    #+#             */
/*   Updated: 2025/12/01 00:00:00 by nkojima          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/pipex.h"

void	close_pipe_children(int file_fd, int pipe_fd[2])
{
	close(file_fd);
	close(pipe_fd[0]);
	close(pipe_fd[1]);
}

void	exec_child1(char *infile, char *cmd, int pipe_fd[2], char **envp)
{
	int		infile_fd;
	char	**cmd_args;
	char	*cmd_path;

	infile_fd = open_infile(infile);
	if (dup2(infile_fd, FD_STDIN) == SYSCALL_ERROR)
		error_exit("dup2 failed", EXIT_GENERAL_ERROR);
	if (dup2(pipe_fd[1], FD_STDOUT) == SYSCALL_ERROR)
		error_exit("dup2 failed", EXIT_GENERAL_ERROR);
	close_pipe_children(infile_fd, pipe_fd);
	cmd_args = parse_command(cmd);
	cmd_path = find_command(cmd_args[0], envp);
	if (!cmd_path)
	{
		ft_putstr_fd("pipex: ", FD_STDERR);
		ft_putstr_fd(cmd_args[0], FD_STDERR);
		ft_putendl_fd(": command not found", FD_STDERR);
		exit(EXIT_CMD_NOT_FOUND);
	}
	execve(cmd_path, cmd_args, envp);
	handle_exec_error(cmd_args[0]);
}

void	exec_child2(char *outfile, char *cmd, int pipe_fd[2], char **envp)
{
	int		outfile_fd;
	char	**cmd_args;
	char	*cmd_path;

	outfile_fd = open_outfile(outfile);
	if (dup2(pipe_fd[0], FD_STDIN) == SYSCALL_ERROR)
		error_exit("dup2 failed", EXIT_GENERAL_ERROR);
	if (dup2(outfile_fd, FD_STDOUT) == SYSCALL_ERROR)
		error_exit("dup2 failed", EXIT_GENERAL_ERROR);
	close_pipe_children(outfile_fd, pipe_fd);
	cmd_args = parse_command(cmd);
	cmd_path = find_command(cmd_args[0], envp);
	if (!cmd_path)
	{
		ft_putstr_fd("pipex: ", FD_STDERR);
		ft_putstr_fd(cmd_args[0], FD_STDERR);
		ft_putendl_fd(": command not found", FD_STDERR);
		exit(EXIT_CMD_NOT_FOUND);
	}
	execve(cmd_path, cmd_args, envp);
	handle_exec_error(cmd_args[0]);
}
