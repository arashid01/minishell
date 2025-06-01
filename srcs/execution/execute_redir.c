/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_redir.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amal <amal@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/01 13:08:30 by amal              #+#    #+#             */
/*   Updated: 2025/06/01 14:53:49 by amal             ###   ########.fr       */
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
		exit(1);
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
	if (in_fd != STDIN_FILENO)
	{
		if (dup2(in_fd, STDIN_FILENO) == -1)
			ft_error("dup2 in_fd");
		close(in_fd);
	}
	if (out_fd != STDOUT_FILENO)
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

int handle_out(t_cmd *cmd, int inherited_fd)
{
	int	fd;
	int	flags;

	fd = inherited_fd;
	if (cmd->outfile)
	{
		flags = O_WRONLY | O_CREAT;
		if (cmd->append)
			flags |= O_APPEND;
		else
			flags |= O_TRUNC;
		fd = open(cmd->outfile, flags, 0644);
		if (fd == -1)
		{
			ft_putstr_fd("minishell: ", 2);
			ft_putstr_fd(cmd->outfile, 2);
			ft_putstr_fd(": Cannot open for writing\n", 2);
			exit(1);
		}
	}
	return (fd);
}
