/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nkojima <nkojima@student.42tokyo.jp>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/01 00:00:00 by nkojima           #+#    #+#             */
/*   Updated: 2025/12/11 18:20:48 by nkojima          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/pipex.h"

void	free_array(char **array)
{
	int	i;

	i = 0;
	while (array[i])
	{
		free(array[i]);
		i++;
	}
	free(array);
}

void	cmd_not_found_exit(char **cmd_args, char *cmd, void (*cleanup)(void *),
		void *ctx)
{
	if (cmd)
	{
		ft_putstr_fd("pipex: ", FD_STDERR);
		ft_putstr_fd(cmd, FD_STDERR);
		ft_putendl_fd(": command not found", FD_STDERR);
	}
	else
		ft_putendl_fd("pipex: : command not found", FD_STDERR);
	if (cleanup)
		cleanup(ctx);
	free_array(cmd_args);
	exit(EXIT_CMD_NOT_FOUND);
}

void	parse_and_execute_command(char *cmd_str, char **envp)
{
	char	**cmd_args;
	char	*cmd_path;

	cmd_args = parse_command(cmd_str);
	if (!cmd_args[0])
		cmd_not_found_exit(cmd_args, NULL, NULL, NULL);
	cmd_path = find_command(cmd_args[0], envp);
	if (!cmd_path)
		cmd_not_found_exit(cmd_args, cmd_args[0], NULL, NULL);
	execve(cmd_path, cmd_args, envp);
	free(cmd_path);
	free_array(cmd_args);
	handle_exec_error(cmd_str);
}
