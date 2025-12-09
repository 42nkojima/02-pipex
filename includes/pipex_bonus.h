/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex_bonus.h                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nkojima <nkojima@student.42tokyo.jp>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/09 21:40:00 by nkojima           #+#    #+#             */
/*   Updated: 2025/12/09 21:40:00 by nkojima          ###   ########.fr       */
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

# define EXIT_SUCCESS 0
# define EXIT_GENERAL_ERROR 1
# define EXIT_CMD_NOT_FOUND 127
# define EXIT_CMD_NOT_EXECUTABLE 126
# define EXIT_SIGNAL_BASE 128

# define FD_STDIN 0
# define FD_STDOUT 1
# define FD_STDERR 2

# define FILE_PERMISSION 0644

# define SYSCALL_ERROR -1
# define SYSCALL_SUCCESS 0

// Common functions (reused from mandatory)
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
int		wait_and_get_exit_code(pid_t pid1, pid_t pid2);

// utils.c
void	free_array(char **array);

// Bonus-specific functions
// multi_pipe_bonus.c
// (to be implemented)

// heredoc_bonus.c
// (to be implemented)

// process_bonus.c
// (to be implemented)

#endif
