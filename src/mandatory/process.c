/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   process.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nkojima <nkojima@student.42tokyo.jp>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/01 00:00:00 by nkojima           #+#    #+#             */
/*   Updated: 2025/12/14 07:34:53 by nkojima          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/pipex.h"

static void	execute_cmd(char *cmd, char **envp)
{
	char	**cmd_args;
	char	*cmd_path;

	cmd_args = parse_command(cmd);
	if (!cmd_args[0])
		cmd_not_found_exit(cmd_args, NULL, NULL, NULL);
	cmd_path = find_command(cmd_args[0], envp);
	if (!cmd_path)
		cmd_not_found_exit(cmd_args, cmd_args[0], NULL, NULL);
	execve(cmd_path, cmd_args, envp);
	free(cmd_path);
	free_array(cmd_args);
	handle_exec_error(cmd);
}

void	exec_child1(char *infile, char *cmd, int pipe_fd[2], char **envp)
{
	int	infile_fd;

	infile_fd = open_infile(infile);
	if (dup2(infile_fd, FD_STDIN) == SYSCALL_ERROR)
		error_exit("dup2 failed", EXIT_GENERAL_ERROR);
	if (dup2(pipe_fd[1], FD_STDOUT) == SYSCALL_ERROR)
		error_exit("dup2 failed", EXIT_GENERAL_ERROR);
	close(infile_fd);
	close(pipe_fd[0]);
	close(pipe_fd[1]);
	execute_cmd(cmd, envp);
}

void	exec_child2(char *outfile, char *cmd, int pipe_fd[2], char **envp)
{
	int	outfile_fd;

	if (dup2(pipe_fd[0], FD_STDIN) == SYSCALL_ERROR)
		error_exit("dup2 failed", EXIT_GENERAL_ERROR);
	close(pipe_fd[0]);
	close(pipe_fd[1]);
	outfile_fd = open_outfile(outfile);
	if (dup2(outfile_fd, FD_STDOUT) == SYSCALL_ERROR)
		error_exit("dup2 failed", EXIT_GENERAL_ERROR);
	close(outfile_fd);
	execute_cmd(cmd, envp);
}
