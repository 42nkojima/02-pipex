/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex_common.h                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nkojima <nkojima@student.42tokyo.jp>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/11 07:30:00 by nkojima           #+#    #+#             */
/*   Updated: 2025/12/11 07:30:00 by nkojima          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PIPEX_COMMON_H
# define PIPEX_COMMON_H

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

#endif
