/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_redir.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amrashid <amrashid@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/12 13:08:54 by amal              #+#    #+#             */
/*   Updated: 2025/06/21 14:47:48 by amrashid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static int	open_redir_fd(t_redir *r)
{
	if (r->type == R_INPUT || r->type == R_HEREDOC)
		return (open(r->target, O_RDONLY));
	else if (r->type == R_OUTPUT)
		return (open(r->target, O_WRONLY | O_CREAT | O_TRUNC, 0644));
	else if (r->type == R_APPEND)
		return (open(r->target, O_WRONLY | O_CREAT | O_APPEND, 0644));
	perror("minishell: unknown redirection type");
	return (-1);
}

static int	dup_redir_fd(t_redir *r, int fd)
{
	int	result;

	if (r->type == R_INPUT || r->type == R_HEREDOC)
		result = dup2(fd, STDIN_FILENO);
	else
		result = dup2(fd, STDOUT_FILENO);
	if (result == -1)
	{
		if (r->type == R_INPUT || r->type == R_HEREDOC)
			perror("dup2 stdin");
		else
			perror("dup2 stdout");
		return (0);
	}
	return (1);
}

int	handle_redirections(t_cmd *cmd)
{
	t_redir	*r;
	int		fd;

	r = cmd->redirs;
	while (r)
	{
		fd = open_redir_fd(r);
		if (fd == -1)
		{
			perror(r->target);
			return (1);
		}
		if (!dup_redir_fd(r, fd))
		{
			close(fd);
			return (1);
		}
		close(fd);
		r = r->next;
	}
	return (0);
}
