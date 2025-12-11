/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   multi_pipe_bonus.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nkojima <nkojima@student.42tokyo.jp>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/09 21:40:00 by nkojima           #+#    #+#             */
/*   Updated: 2025/12/10 00:00:00 by nkojima          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex_bonus.h"

void	init_pipex(t_pipex *pipex, int argc, char **argv, char **envp)
{
	if (argc < 5)
		error_exit("Usage: ./pipex file1 cmd1 cmd2 ... cmdn file2",
			EXIT_GENERAL_ERROR);
	pipex->cmd_count = argc - 3;
	pipex->argv = argv;
	pipex->envp = envp;
	pipex->pipes = malloc(sizeof(int *) * (pipex->cmd_count - 1));
	if (!pipex->pipes)
		error_exit("malloc failed", EXIT_GENERAL_ERROR);
	pipex->pids = malloc(sizeof(pid_t) * pipex->cmd_count);
	if (!pipex->pids)
	{
		free(pipex->pipes);
		error_exit("malloc failed", EXIT_GENERAL_ERROR);
	}
}

void	create_pipes_parent(t_pipex *pipex)
{
	int	i;

	i = 0;
	while (i < pipex->cmd_count - 1)
	{
		pipex->pipes[i] = malloc(sizeof(int) * 2);
		if (!pipex->pipes[i])
			error_exit("malloc failed", EXIT_GENERAL_ERROR);
		if (pipe(pipex->pipes[i]) == SYSCALL_ERROR)
			error_exit("pipe failed", EXIT_GENERAL_ERROR);
		i++;
	}
}

void	close_pipes_parent(t_pipex *pipex)
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

void	free_pipex(t_pipex *pipex)
{
	int	i;

	i = 0;
	while (i < pipex->cmd_count - 1)
	{
		free(pipex->pipes[i]);
		i++;
	}
	free(pipex->pipes);
	free(pipex->pids);
}
