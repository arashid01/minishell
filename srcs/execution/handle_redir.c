/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_redir.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amrashid <amrashid@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/12 13:08:54 by amal              #+#    #+#             */
/*   Updated: 2025/06/20 13:17:00 by amrashid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int	handle_redirections(t_cmd *cmd)
{
	t_redir	*r;
	int		fd;

	r = cmd->redirs;
	while (r)
	{
		if (r->type == R_INPUT)
			fd = open(r->target, O_RDONLY);
		else if (r->type == R_OUTPUT)
			fd = open(r->target, O_WRONLY | O_CREAT | O_TRUNC, 0644);
		else if (r->type == R_APPEND)
			fd = open(r->target, O_WRONLY | O_CREAT | O_APPEND, 0644);
		else if (r->type == R_HEREDOC)
			fd = open(r->target, O_RDONLY);
		else
		{
			perror("minishell: unknown redirection type");
			return (1);
		}
		if (fd == -1)
		{
			perror(r->target);
			return (1);
		}
		if (r->type == R_INPUT || r->type == R_HEREDOC)
		{
			if (dup2(fd, STDIN_FILENO) == -1)
			{
				perror("dup2 stdin");
				close(fd);
				return (1);
			}
		}
		else
		{
			if (dup2(fd, STDOUT_FILENO) == -1)
			{
				perror("dup2 stdout");
				close(fd);
				return (1);
			}
		}
		close(fd);
		r = r->next;
	}
	return (0);
}
