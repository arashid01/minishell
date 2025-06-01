/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_heredoc.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amal <amal@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/01 11:18:42 by amal              #+#    #+#             */
/*   Updated: 2025/06/01 11:27:07 by amal             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

// void	handle_heredoc(const char *delimiter, char **temp_file)
// {
// 	int		fd;
// 	int		pid;
// 	int		status;
// 	char	*line;

// 	*temp_file = ft_strdup("/tmp/.heredoc_tmp");
// 	if (!*temp_file)
// 		ft_error("heredoc");

// 	pid = fork();
// 	if (pid < 0)
// 		ft_error("fork");

// 	if (pid == 0)
// 	{
// 		setup_heredoc_signals();

// 		fd = open(*temp_file, O_CREAT | O_WRONLY | O_TRUNC, 0644);
// 		if (fd < 0)
// 			ft_error("open");

// 		while (1)
// 		{
// 			line = readline("> ");
// 			if (!line || ft_strncmp(line, delimiter, -1) == 0)
// 			{
// 				free(line);
// 				break;
// 			}
// 			write(fd, line, ft_strlen(line));
// 			write(fd, "\n", 1);
// 			free(line);
// 		}
// 		close(fd);
// 		exit(0);
// 	}
// 	else
// 	{
// 		waitpid(pid, &status, 0);
// 		if (WIFSIGNALED(status) && WTERMSIG(status) == SIGINT)
// 		{
// 			unlink(*temp_file);
// 			free(*temp_file);
// 			*temp_file = NULL;
// 		}
// 	}
// }

static void	heredoc_child(const char *delim, const char *outfile)
{
	int		fd;
	char	*line;

	setup_heredoc_signals();
	fd = open(outfile, O_CREAT | O_WRONLY | O_TRUNC, 0644);
	if (fd < 0)
		ft_error("heredoc open");

	while (1)
	{
		line = readline("> ");
		if (!line || ft_strcmp(line, delim) == 0)
			break ;
		write(fd, line, ft_strlen(line));
		write(fd, "\n", 1);
		free(line);
	}
	free(line);
	close(fd);
	exit(0);
}

void	handle_heredoc(const char *delimiter, char **outfile)
{
	pid_t	pid;
	int		status;

	*outfile = ft_strdup("/tmp/.heredoc_tmp");
	if (!*outfile)
		ft_error("heredoc malloc");

	pid = fork();
	if (pid == -1)
		ft_error("heredoc fork");

	if (pid == 0)
		heredoc_child(delimiter, *outfile);
	else
	{
		waitpid(pid, &status, 0);
		if (WIFSIGNALED(status) && WTERMSIG(status) == SIGINT)
		{
			unlink(*outfile);
			free(*outfile);
			*outfile = NULL;
		}
	}
}
