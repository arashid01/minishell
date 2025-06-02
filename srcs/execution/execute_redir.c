/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_redir.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amal <amal@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/01 13:08:30 by amal              #+#    #+#             */
/*   Updated: 2025/06/02 05:46:44 by amal             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static int	open_infile(char *infile)
{
	int	fd;
	
	fd = open(infile, O_RDONLY);
	if (fd == -1)
	{
		ft_putstr_fd("minishell: ", 2);
		ft_putstr_fd(infile, 2);
		ft_putstr_fd(": No such file or directory\n", 2);
		g_exit_status = 1;
		return (-1);
	}
	return (fd);
}

void	restore_std(int saved_fd, int std_fd)
{
	if (dup2(saved_fd, std_fd) == -1)
		ft_error("dup2 restore");
	close(saved_fd);
}

void	setup_redir(int in_fd, int out_fd)
{
	if (in_fd != STDIN_FILENO && in_fd >= 0)
	{
		if (dup2(in_fd, STDIN_FILENO) == -1)
			ft_error("dup2 in_fd");
		close(in_fd);
	}
	if (out_fd != STDOUT_FILENO && out_fd >= 0)
	{
		if (dup2(out_fd, STDOUT_FILENO) == -1)
			ft_error("dup2 out_fd");
		close(out_fd);
	}
}

int	handle_in(t_cmd *cmd, int inherited_fd)
{
	int	fd;
	
	fd = inherited_fd;
	if (cmd->delim)
	{
		if (cmd->infile)
			free(cmd->infile);
		handle_heredoc(cmd->delim, &cmd->infile);
		if (!cmd->infile)
			exit(1);
	}
	if (cmd->infile)
		fd = open_infile(cmd->infile);
	return (fd);
}

int	handle_out(t_cmd *cmd, int inherited_fd)
{
	int		fd;
	int		tmp_fd;
	int		flags;
	t_redir	*curr;

	fd = inherited_fd;
	curr = cmd->outfiles;
	while (curr)
	{
		flags = O_WRONLY | O_CREAT;
		if (curr->append)
			flags |= O_APPEND;
		else
			flags |= O_TRUNC;
		tmp_fd = open(curr->outfile, flags, 0644);
		if (tmp_fd == -1)
		{
			ft_putstr_fd("minishell: ", 2);
			ft_putstr_fd(curr->outfile, 2);
			ft_putendl_fd(": No such file or directory", 2);
			g_exit_status = 1;
			return(-1);
		}
		if (fd != inherited_fd)
			close(fd);
		fd = tmp_fd;
		curr = curr->next;
	}
	return (fd);
}
