/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   process_bonus.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nkojima <nkojima@student.42tokyo.jp>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/09 21:40:00 by nkojima           #+#    #+#             */
/*   Updated: 2025/12/11 18:20:54 by nkojima          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex_bonus.h"

static void	close_pipes_children(t_pipex *pipex)
{
	int	i;

	i = 0;
	while (i < pipex->cmd_count - 1)
	{
		close(pipex->pipes[i][0]);
		close(pipex->pipes[i][1]);
		i++;
	}
}

static void	execute_command(t_pipex *pipex, char *cmd)
{
	char	**cmd_args;
	char	*cmd_path;

	cmd_args = parse_command(cmd);
	if (!cmd_args[0])
		cmd_not_found_exit(cmd_args, NULL, (void (*)(void *))free_pipex, pipex);
	cmd_path = find_command(cmd_args[0], pipex->envp);
	if (!cmd_path)
		cmd_not_found_exit(cmd_args, cmd_args[0], (void (*)(void *))free_pipex,
			pipex);
	execve(cmd_path, cmd_args, pipex->envp);
	free_array(cmd_args);
	free(cmd_path);
	free_pipex(pipex);
	handle_exec_error(cmd);
}

void	exec_input_command(t_pipex *pipex, int index)
{
	int	infile_fd;

	(void)index;
	infile_fd = open_infile(pipex->argv[1]);
	if (dup2(infile_fd, FD_STDIN) == SYSCALL_ERROR)
		error_exit("dup2 failed", EXIT_GENERAL_ERROR);
	if (dup2(pipex->pipes[0][1], FD_STDOUT) == SYSCALL_ERROR)
		error_exit("dup2 failed", EXIT_GENERAL_ERROR);
	close(infile_fd);
	close_pipes_children(pipex);
	execute_command(pipex, pipex->argv[2]);
}

void	exec_pipe_command(t_pipex *pipex, int index)
{
	if (dup2(pipex->pipes[index - 1][0], FD_STDIN) == SYSCALL_ERROR)
		error_exit("dup2 failed", EXIT_GENERAL_ERROR);
	if (dup2(pipex->pipes[index][1], FD_STDOUT) == SYSCALL_ERROR)
		error_exit("dup2 failed", EXIT_GENERAL_ERROR);
	close_pipes_children(pipex);
	execute_command(pipex, pipex->argv[index + 2]);
}

void	exec_output_command(t_pipex *pipex, int index)
{
	int	outfile_fd;

	outfile_fd = open_outfile(pipex->argv[pipex->cmd_count + 2]);
	if (dup2(pipex->pipes[index - 1][0], FD_STDIN) == SYSCALL_ERROR)
		error_exit("dup2 failed", EXIT_GENERAL_ERROR);
	if (dup2(outfile_fd, FD_STDOUT) == SYSCALL_ERROR)
		error_exit("dup2 failed", EXIT_GENERAL_ERROR);
	close(outfile_fd);
	close_pipes_children(pipex);
	execute_command(pipex, pipex->argv[index + 2]);
}
