/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   path.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nkojima <nkojima@student.42tokyo.jp>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/01 00:00:00 by nkojima           #+#    #+#             */
/*   Updated: 2025/12/01 00:00:00 by nkojima          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/pipex.h"

char	**parse_command(char *cmd_str)
{
	char	**args;

	args = ft_split(cmd_str, ' ');
	if (!args)
		error_exit("malloc failed", EXIT_GENERAL_ERROR);
	return (args);
}

char	*get_path_env(char **envp)
{
	int	i;

	i = 0;
	while (envp[i])
	{
		if (ft_strncmp(envp[i], "PATH=", 5) == 0)
			return (envp[i] + 5);
		i++;
	}
	return (NULL);
}

char	*build_path(char *dir, char *cmd)
{
	char	*slash;
	char	*path;

	slash = ft_strjoin(dir, "/");
	if (!slash)
		error_exit("malloc failed", EXIT_GENERAL_ERROR);
	path = ft_strjoin(slash, cmd);
	if (!path)
	{
		free(slash);
		error_exit("malloc failed", EXIT_GENERAL_ERROR);
	}
	free(slash);
	return (path);
}

char	*check_executable(char *path)
{
	char	*dup;

	if (access(path, F_OK) == SYSCALL_ERROR)
		error_exit("command not found", EXIT_CMD_NOT_FOUND);
	if (access(path, X_OK) == SYSCALL_ERROR)
		error_exit("permission denied", EXIT_CMD_NOT_EXECUTABLE);
	dup = ft_strdup(path);
	if (!dup)
		error_exit("malloc failed", EXIT_GENERAL_ERROR);
	return (dup);
}

char	*find_command(char *cmd, char **envp)
{
	char	*path_env;
	char	**paths;
	char	*full_path;
	int		i;

	if (ft_strchr(cmd, '/'))
		return (check_executable(cmd));
	path_env = get_path_env(envp);
	if (!path_env)
		error_exit("PATH not found", EXIT_CMD_NOT_FOUND);
	paths = ft_split(path_env, ':');
	i = 0;
	while (paths[i])
	{
		full_path = build_path(paths[i], cmd);
		if (access(full_path, X_OK) == SYSCALL_SUCCESS)
		{
			free_array(paths);
			return (full_path);
		}
		free(full_path);
		i++;
	}
	free_array(paths);
	return (NULL);
}
