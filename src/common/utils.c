/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nkojima <nkojima@student.42tokyo.jp>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/01 00:00:00 by nkojima           #+#    #+#             */
/*   Updated: 2025/12/11 07:35:00 by nkojima          ###   ########.fr       */
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

void	parse_and_execute_command(char *cmd_str, char **envp)
{
	char	**cmd_args;
	char	*cmd_path;

	cmd_args = parse_command(cmd_str);
	cmd_path = find_command(cmd_args[0], envp);
	if (!cmd_path)
	{
		ft_putstr_fd("pipex: ", FD_STDERR);
		ft_putstr_fd(cmd_args[0], FD_STDERR);
		ft_putendl_fd(": command not found", FD_STDERR);
		free_array(cmd_args);
		exit(EXIT_CMD_NOT_FOUND);
	}
	execve(cmd_path, cmd_args, envp);
	free(cmd_path);
	free_array(cmd_args);
	handle_exec_error(cmd_str);
}
