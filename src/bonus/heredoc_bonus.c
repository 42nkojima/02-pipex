/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_bonus.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nkojima <nkojima@student.42tokyo.jp>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/09 21:40:00 by nkojima           #+#    #+#             */
/*   Updated: 2025/12/11 18:21:03 by nkojima          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex_bonus.h"

static int	open_outfile_append(char *filename)
{
	int	fd;

	fd = open(filename, O_WRONLY | O_CREAT | O_APPEND, FILE_PERMISSION);
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

int	read_heredoc(char *limiter)
{
	int		pipefd[2];
	char	*line;

	if (pipe(pipefd) == SYSCALL_ERROR)
		error_exit("pipe failed", EXIT_GENERAL_ERROR);
	while (1)
	{
		ft_putstr_fd("> ", FD_STDOUT);
		line = get_next_line(FD_STDIN);
		if (!line)
			break ;
		if (ft_strncmp(line, limiter, ft_strlen(limiter)) == 0
			&& line[ft_strlen(limiter)] == '\n')
		{
			free(line);
			break ;
		}
		ft_putstr_fd(line, pipefd[1]);
		free(line);
	}
	get_next_line(-1);
	close(pipefd[1]);
	return (pipefd[0]);
}

static void	exec_heredoc_child1(char *cmd_str, int heredoc_fd, int pipe_fd[2],
		char **envp)
{
	if (dup2(heredoc_fd, FD_STDIN) == SYSCALL_ERROR)
		error_exit("dup2 failed", EXIT_GENERAL_ERROR);
	close(heredoc_fd);
	if (dup2(pipe_fd[1], FD_STDOUT) == SYSCALL_ERROR)
		error_exit("dup2 failed", EXIT_GENERAL_ERROR);
	close(pipe_fd[0]);
	close(pipe_fd[1]);
	parse_and_execute_command(cmd_str, envp);
}

static void	exec_heredoc_child2(char *cmd_str, char *outfile, int pipe_fd[2],
		char **envp)
{
	int	outfile_fd;

	if (dup2(pipe_fd[0], FD_STDIN) == SYSCALL_ERROR)
		error_exit("dup2 failed", EXIT_GENERAL_ERROR);
	close(pipe_fd[0]);
	close(pipe_fd[1]);
	outfile_fd = open_outfile_append(outfile);
	if (dup2(outfile_fd, FD_STDOUT) == SYSCALL_ERROR)
		error_exit("dup2 failed", EXIT_GENERAL_ERROR);
	close(outfile_fd);
	parse_and_execute_command(cmd_str, envp);
}

void	handle_heredoc(int argc, char **argv, char **envp)
{
	int		heredoc_fd;
	int		pipe_fd[2];
	pid_t	pid1;
	pid_t	pid2;

	if (argc != 6)
		error_exit("Usage: ./pipex here_doc LIMITER cmd1 cmd2 file",
			EXIT_GENERAL_ERROR);
	heredoc_fd = read_heredoc(argv[2]);
	if (pipe(pipe_fd) == SYSCALL_ERROR)
		error_exit("pipe failed", EXIT_GENERAL_ERROR);
	pid1 = fork();
	if (pid1 == SYSCALL_ERROR)
		error_exit("fork failed", EXIT_GENERAL_ERROR);
	if (pid1 == SYSCALL_SUCCESS)
		exec_heredoc_child1(argv[3], heredoc_fd, pipe_fd, envp);
	close(heredoc_fd);
	pid2 = fork();
	if (pid2 == SYSCALL_ERROR)
		error_exit("fork failed", EXIT_GENERAL_ERROR);
	if (pid2 == SYSCALL_SUCCESS)
		exec_heredoc_child2(argv[4], argv[5], pipe_fd, envp);
	close(pipe_fd[0]);
	close(pipe_fd[1]);
	exit(wait_children_and_get_exit_code(pid1, pid2));
}
