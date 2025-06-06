/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_redir.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amal <amal@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/01 13:08:30 by amal              #+#    #+#             */
/*   Updated: 2025/06/06 21:34:33 by amal             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static int	open_infile(t_shell *shell)
{
	int	fd;

	fd = open(shell->cmds->infile, O_RDONLY);
	if (fd == -1)
	{
		ft_putstr_fd("minishell: ", 2);
		ft_putstr_fd(shell->cmds->infile, 2);
		ft_putstr_fd(": No such file or directory\n", 2);
		shell->exit_code = 1;
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

int	handle_in(t_shell *shell, int inherited_fd)
{
	int	fd;

	fd = inherited_fd;
	if (shell->cmds->delim)
	{
		free(shell->cmds->infile);
		handle_heredoc(shell->cmds->delim, &shell->cmds->infile, shell);
		if (!shell->cmds->infile)
			exit(1);
	}
	if (shell->cmds->infile)
		fd = open_infile(shell);
	return (fd);
}

int	handle_out(t_shell *shell, int inherited_fd)
{
	int		fd;
	int		flags;
	int		tmp_fd;
	t_redir	*curr;

	fd = inherited_fd;
	curr = shell->cmds->outfiles;
	while (curr)
	{
		if (curr->append)
			flags = O_WRONLY | O_CREAT | O_APPEND;
		else
			flags = O_WRONLY | O_CREAT | O_TRUNC;
		tmp_fd = open(curr->outfile, flags, 0644);
		if (tmp_fd == -1)
		{
			ft_putstr_fd("minishell: ", 2);
			perror(curr->outfile);
			shell->exit_code = 1;
			return (-1);
		}
		if (fd != inherited_fd)
			close(fd);
		fd = tmp_fd;
		curr = curr->next;
	}
	return (fd);
}
