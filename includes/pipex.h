/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nkojima <nkojima@student.42tokyo.jp>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/30 11:55:41 by nkojima           #+#    #+#             */
/*   Updated: 2025/12/11 18:21:22 by nkojima          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PIPEX_H
# define PIPEX_H

# include "../libs/libft/libft.h"
# include "pipex_common.h"
# include <errno.h>
# include <fcntl.h>
# include <stdlib.h>
# include <string.h>
# include <sys/wait.h>
# include <unistd.h>

# define REQUIRED_ARGC 5

// pipe.c
void	create_pipe_parent(int pipe_fd[2]);
void	close_pipe_parent(int pipe_fd[2]);

// process.c
void	exec_child1(char *infile, char *cmd, int pipe_fd[2], char **envp);
void	exec_child2(char *outfile, char *cmd, int pipe_fd[2], char **envp);
void	close_pipe_children(int file_fd, int pipe_fd[2]);

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
void	cmd_not_found_exit(char **cmd_args, char *cmd, void (*cleanup)(void *),
			void *ctx);

#endif
