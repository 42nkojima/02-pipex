/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex_bonus.h                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nkojima <nkojima@student.42tokyo.jp>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/09 21:40:00 by nkojima           #+#    #+#             */
/*   Updated: 2025/12/11 07:30:00 by nkojima          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PIPEX_BONUS_H
# define PIPEX_BONUS_H

# include <unistd.h>
# include <stdlib.h>
# include <fcntl.h>
# include <sys/wait.h>
# include <errno.h>
# include <string.h>
# include "../libs/libft/libft.h"
# include "pipex_common.h"

// ===============================
//            Common
// ===============================

// path.c
char	**parse_command(char *cmd_str);
char	*find_command(char *cmd, char **envp);
char	*get_path_env(char **envp);
char	*build_path(char *dir, char *cmd);
char	*check_executable(char *path);

// file.c
int		open_infile(char *filename);
int		open_outfile(char *filename);

// error.c
void	validate_argc(int argc);
void	error_exit(char *msg, int exit_code);
void	handle_exec_error(char *cmd);
int		wait_children_and_get_exit_code(pid_t pid1, pid_t pid2);

// utils.c
void	free_array(char **array);
void	parse_and_execute_command(char *cmd_str, char **envp);
void	cmd_not_found_exit(
			char **cmd_args,
			char *cmd,
			void (*cleanup)(void *),
			void *ctx);

// ===============================
//            Bonus
// ===============================

typedef struct s_pipex
{
	int		cmd_count;
	int		**pipes;
	pid_t	*pids;
	char	**argv;
	char	**envp;
}	t_pipex;

// multi_pipe_bonus.c
void	init_pipex(t_pipex *pipex, int argc, char **argv, char **envp);
void	create_pipes_parent(t_pipex *pipex);
void	close_pipes_parent(t_pipex *pipex);
void	free_pipex(t_pipex *pipex);

// heredoc_bonus.c
void	handle_heredoc(int argc, char **argv, char **envp);
int		read_heredoc(char *limiter);

// process_bonus.c
void	exec_input_command(t_pipex *pipex, int index);
void	exec_pipe_command(t_pipex *pipex, int index);
void	exec_output_command(t_pipex *pipex, int index);

#endif
