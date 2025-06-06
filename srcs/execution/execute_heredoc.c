/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_heredoc.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amal <amal@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/01 13:06:49 by amal              #+#    #+#             */
/*   Updated: 2025/06/06 03:09:42 by amal             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static void	heredoc_loop(const char *delim, int fd)
{
	char	*line;

	while (1)
	{
		line = readline("> ");
		if (!line)
		{
			ft_putstr_fd("minishell: warning: heredoc delimited by EOF\n", 2);
			break;
		}
		if (ft_strcmp(line, delim) == 0)
		{
			free(line);
			break;
		}
		write(fd, line, ft_strlen(line));
		write(fd, "\n", 1);
		free(line);
	}
}

static void	heredoc_child(const char *delim, const char *outfile)
{
	int	fd;

	fd = open(outfile, O_CREAT | O_WRONLY | O_TRUNC, 0644);
	if (fd < 0)
		ft_error("heredoc open");
	heredoc_loop(delim, fd);
	close(fd);
	exit(0);
}

static void	heredoc_parent(pid_t pid, char **outfile, t_shell *shell)
{
	int	status;

	waitpid(pid, &status, 0);
	if (WIFSIGNALED(status))
	{
		unlink(*outfile);
		free(*outfile);
		*outfile = NULL;
		shell->exit_code = 128 + WTERMSIG(status);
		if (WTERMSIG(status) == SIGINT)
			shell->exit_code = 1;
	}
}

void	handle_heredoc(const char *delim, char **outfile, t_shell *shell)
{
	pid_t	pid;

	*outfile = ft_strdup("/tmp/.heredoc_tmp_XXXXXX");
	if (!*outfile)
		ft_error("heredoc malloc");
	pid = fork();
	if (pid < 0)
	{
		free(*outfile);
		*outfile = NULL;
		ft_error("heredoc fork");
	}
	if (pid == 0)
	{
		setup_heredoc_signals();
		heredoc_child(delim, *outfile);
	}
	else
		heredoc_parent(pid, outfile, shell);
}
