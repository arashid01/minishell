/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amrashid <amrashid@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/12 12:48:58 by amal              #+#    #+#             */
/*   Updated: 2025/06/18 15:05:37 by amrashid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void	cleanup_heredoc_files(t_cmd *cmd_list)
{
	t_cmd	*cmd;
	t_redir	*r;

	cmd = cmd_list;
	while (cmd)
	{
		r = cmd->redirs;
		while (r)
		{
			if (r->type == R_HEREDOC)
				unlink(r->target);
			r = r->next;
		}
		cmd = cmd->next;
	}
}

static int	process_single_heredoc(t_redir *redir, int index)
{
	char	*line;
	char	*filename;
	char	*num_str;
	int		fd;

	num_str = ft_itoa(index);
	if (!num_str)
		return (1);
	filename = ft_strjoin(".heredoc_", num_str);
	free(num_str);
	if (!filename)
		return (1);
	fd = open(filename, O_CREAT | O_WRONLY | O_TRUNC, 0644);
	if (fd < 0)
	{
		free(filename);
		return (1);
	}
	while (1)
	{
		setup_heredoc_signals();
		line = readline("> ");
		if (!line || ft_strcmp(line, redir->target) == 0)
			break ;
		write(fd, line, ft_strlen(line));
		write(fd, "\n", 1);
		free(line);
	}
	close(fd);
	free(line);
	free(redir->target);
	redir->target = filename;
	return (0);
}

int	handle_heredocs(t_shell *shell)
{
	t_cmd		*cmd;
	t_redir		*r;
	int			heredoc_count;

	heredoc_count = 0;
	cmd = shell->cmds;
	while (cmd)
	{
		r = cmd->redirs;
		while (r)
		{
			if (r->type == R_HEREDOC)
			{
				if (process_single_heredoc(r, heredoc_count++) != 0)
					return (1);
			}
			r = r->next;
		}
		cmd = cmd->next;
	}
	return (0);
}
