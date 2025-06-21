/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_loop.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amrashid <amrashid@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/21 13:27:53 by amrashid          #+#    #+#             */
/*   Updated: 2025/06/21 13:30:02 by amrashid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static int	open_heredoc_file(char **filename, int index)
{
	char	*num_str;
	int		fd;

	num_str = ft_itoa(index);
	if (!num_str)
		return (-1);
	*filename = ft_strjoin(".heredoc_", num_str);
	free(num_str);
	if (!*filename)
		return (-1);
	fd = open(*filename, O_CREAT | O_WRONLY | O_TRUNC, 0644);
	if (fd < 0)
	{
		free(*filename);
		*filename = NULL;
	}
	return (fd);
}

static int	should_expand(t_redir *redir)
{
	int	len;

	len = strlen(redir->target);
	if ((redir->target[0] == '\'' || redir->target[0] == '"')
		&& (redir->target[len - 1] == redir->target[0]))
		return (0);
	return (1);
}

static void	heredoc_loop(t_redir *redir, int fd, int q_flag, t_shell *shell)
{
	char	*line;
	char	*expanded;
	char	*dollar_sign;

	while (1)
	{
		setup_heredoc_signals();
		line = readline("> ");
		if (!line || ft_strcmp(line, redir->target) == 0)
			break ;
		dollar_sign = ft_strchr(line, '$');
		if (dollar_sign && q_flag)
		{
			expanded = expand_heredoc(dollar_sign, shell);
			write(fd, expanded, ft_strlen(expanded));
			free(expanded);
		}
		else
			write(fd, line, ft_strlen(line));
		write(fd, "\n", 1);
		free(line);
	}
	free(line);
}

int	single_heredoc(t_redir *redir, int idx, t_shell *shell)
{
	int		fd;
	int		q_flag;
	char	*filename;

	q_flag = should_expand(redir);
	fd = open_heredoc_file(&filename, idx);
	if (fd < 0)
		return (1);
	heredoc_loop(redir, fd, q_flag, shell);
	close(fd);
	free(redir->target);
	redir->target = filename;
	return (0);
}
