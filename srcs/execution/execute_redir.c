/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_redir.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amal <amal@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/01 13:08:30 by amal              #+#    #+#             */
/*   Updated: 2025/06/09 12:33:08 by amal             ###   ########.fr       */
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

static int	open_outfile(const char *outfile, int append, t_shell *shell)
{
	int	flags;
	int	fd;

	if (append)
		flags = O_WRONLY | O_CREAT | O_APPEND;
	else
		flags = O_WRONLY | O_CREAT | O_TRUNC;
	fd = open(outfile, flags, 0644);
	if (fd == -1)
	{
		ft_putstr_fd("minishell: ", 2);
		perror(outfile);
		shell->exit_code = 1;
	}
	return fd;
}

int	handle_out(t_shell *shell, int inherited_fd)
{
	int		fd;
	int		tmp_fd;
	t_redir	*curr;

	fd = inherited_fd;
	curr = shell->cmds->outfiles;
	while (curr)
	{
		tmp_fd = open_outfile(curr->outfile, curr->append, shell);
		if (tmp_fd == -1)
			return (-1);
		if (fd != inherited_fd)
			close(fd);
		fd = tmp_fd;
		curr = curr->next;
	}
	return (fd);
}
