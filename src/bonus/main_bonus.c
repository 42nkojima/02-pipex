/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main_bonus.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nkojima <nkojima@student.42tokyo.jp>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/09 21:40:00 by nkojima           #+#    #+#             */
/*   Updated: 2025/12/10 00:00:00 by nkojima          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex_bonus.h"

static void	fork_and_exec(t_pipex *pipex, int i)
{
	pipex->pids[i] = fork();
	if (pipex->pids[i] == SYSCALL_ERROR)
		error_exit("fork failed", EXIT_GENERAL_ERROR);
	if (pipex->pids[i] == SYSCALL_SUCCESS)
	{
		if (i == 0)
			exec_input_command(pipex, i);
		else if (i == pipex->cmd_count - 1)
			exec_output_command(pipex, i);
		else
			exec_pipe_command(pipex, i);
	}
}

static int	wait_all_children(t_pipex *pipex)
{
	int	i;
	int	status;
	int	exit_code;

	i = 0;
	exit_code = EXIT_SUCCESS;
	while (i < pipex->cmd_count)
	{
		waitpid(pipex->pids[i], &status, 0);
		if (i == pipex->cmd_count - 1)
		{
			if (WIFEXITED(status))
				exit_code = WEXITSTATUS(status);
			else if (WIFSIGNALED(status))
				exit_code = EXIT_SIGNAL_BASE + WTERMSIG(status);
		}
		i++;
	}
	return (exit_code);
}

static void	execute_pipeline(t_pipex *pipex)
{
	int	i;
	int	exit_code;

	i = 0;
	while (i < pipex->cmd_count)
	{
		fork_and_exec(pipex, i);
		i++;
	}
	close_pipes_parent(pipex);
	exit_code = wait_all_children(pipex);
	free_pipex(pipex);
	exit(exit_code);
}

int	main(int argc, char **argv, char **envp)
{
	t_pipex	pipex;

	init_pipex(&pipex, argc, argv, envp);
	create_pipes(&pipex);
	execute_pipeline(&pipex);
	return (EXIT_SUCCESS);
}
