/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amrashid <amrashid@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/12 12:48:58 by amal              #+#    #+#             */
/*   Updated: 2025/06/21 14:46:44 by amrashid         ###   ########.fr       */
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
				if (single_heredoc(r, heredoc_count++, shell) != 0)
					return (1);
			}
			r = r->next;
		}
		cmd = cmd->next;
	}
	return (0);
}
