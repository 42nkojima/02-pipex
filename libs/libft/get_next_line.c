/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nkojima <nkojima@student.42tokyo.jp>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/01 17:56:12 by nkojima           #+#    #+#             */
/*   Updated: 2025/12/09 00:00:00 by nkojima          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

/**
 * Extract a line that contains a newline character
 *
 * @param stored_data Pointer to the stored data
 * @param newline_pos Position of the newline character
 * @return char* Extracted line (including newline), NULL on error
 */
static char	*extract_line_with_newline(char **stored_data, char *newline_pos)
{
	char	*line;
	char	*temp;
	size_t	line_len;

	line_len = newline_pos - *stored_data + 1;
	line = malloc(line_len + 1);
	if (!line)
	{
		free(*stored_data);
		*stored_data = NULL;
		return (NULL);
	}
	ft_memcpy(line, *stored_data, line_len);
	line[line_len] = '\0';
	temp = *stored_data;
	*stored_data = ft_strdup(newline_pos + 1);
	free(temp);
	if (!*stored_data)
	{
		free(line);
		return (NULL);
	}
	return (line);
}

/**
 * Extract a line from the stored data
 *
 * @param stored_data Pointer to the stored data
 * @return char* Extracted line, NULL if no data or error
 */
static char	*extract_line(char **stored_data)
{
	char	*newline_pos;
	char	*line;

	if (!*stored_data || **stored_data == '\0')
	{
		free(*stored_data);
		*stored_data = NULL;
		return (NULL);
	}
	newline_pos = ft_strchr(*stored_data, '\n');
	if (newline_pos)
		return (extract_line_with_newline(stored_data, newline_pos));
	line = ft_strdup(*stored_data);
	free(*stored_data);
	*stored_data = NULL;
	return (line);
}

/**
 * Read data from file into buffer and append to stored data
 *
 * @param fd File descriptor
 * @param buffer Buffer for reading
 * @param stored_data Pointer to stored data
 * @return int Read status (negative: error, 0: newline found or EOF,
	positive: continue)
 */
static int	handle_buffer_read(int fd, char *buffer, char **stored_data)
{
	ssize_t	bytes_read;
	char	*temp;

	bytes_read = read(fd, buffer, BUFFER_SIZE);
	if (bytes_read <= 0)
		return (bytes_read);
	buffer[bytes_read] = '\0';
	if (!*stored_data)
		*stored_data = ft_strdup("");
	if (!*stored_data)
		return (-1);
	temp = ft_strjoin(*stored_data, buffer);
	free(*stored_data);
	if (!temp)
		return (*stored_data = NULL, -1);
	*stored_data = temp;
	if (ft_strchr(*stored_data, '\n'))
		return (0);
	return (1);
}

/**
 * Read from file until a newline is found
 *
 * @param fd File descriptor
 * @param stored_data Stored data
 * @return char* Read data, NULL on error
 */
static char	*read_until_newline(int fd, char *stored_data)
{
	char	*buffer;
	int		read_result;

	buffer = malloc(sizeof(*buffer) * (BUFFER_SIZE + 1));
	if (!buffer)
	{
		free(stored_data);
		return (NULL);
	}
	while (1)
	{
		read_result = handle_buffer_read(fd, buffer, &stored_data);
		if (read_result <= 0)
			break ;
	}
	free(buffer);
	if (read_result < 0)
	{
		free(stored_data);
		return (NULL);
	}
	return (stored_data);
}

/**
 * Get the next line from a file
 *
 * @param fd File descriptor
 * @return char* Next line from file (including newline), NULL on error or EOF
 *
 * This function reads the next line from the specified file descriptor.
 * Lines are delimited by newline characters, which are included in the output.
 */
char	*get_next_line(int fd)
{
	static char	*stored_data = NULL;

	if (fd < 0 || BUFFER_SIZE <= 0)
		return (NULL);
	if (!stored_data || !ft_strchr(stored_data, '\n'))
		stored_data = read_until_newline(fd, stored_data);
	return (extract_line(&stored_data));
}
