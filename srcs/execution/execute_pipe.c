/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_pipe.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amal <amal@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/01 13:04:24 by amal              #+#    #+#             */
/*   Updated: 2025/06/06 03:15:17 by amal             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static void	parent_cleanup(t_cmd *cmd, t_shell *shell, int *curr_in, int *pipe_fds)
{
	if (shell->saved_in != STDIN_FILENO)
		close(shell->saved_in);
	if (cmd->next)
	{
		close(pipe_fds[1]);
		*curr_in = pipe_fds[0];
	}
	else if (shell->saved_out != STDOUT_FILENO)
		close(shell->saved_out);
}

static void	handle_cmd(t_cmd *cmd, t_shell *shell, int *curr_in, int out_fd)
{
	int		pipe_fds[2];

	shell->saved_in = handle_in(shell, *curr_in);
	if (shell->saved_in == -1)
	{
		shell->exit_code = 1;
		return ;
	}
	shell->saved_out = out_fd;
	if (cmd->next)
	{
		pipe(pipe_fds);
		shell->saved_out = pipe_fds[1];
	}
	shell->saved_out = handle_out(shell, shell->saved_out);
	shell->last_pid = fork();
	if (shell->last_pid == 0)
	{
		if (cmd->next)
			close(pipe_fds[0]);
		exec_child(cmd, shell);
	}
	else
		parent_cleanup(cmd, shell, curr_in, pipe_fds);
}

void	exec_pipe(t_shell *shell, int in_fd, int out_fd)
{
	t_cmd	*cmd;
	int		curr_in;

	cmd = shell->cmds;
	shell->last_pid = -1;
	curr_in = in_fd;
	if (is_builtin_cmd(cmd) && !cmd->next)
	{
		shell->exit_code = exec_builtin_single(shell, curr_in, out_fd);
		return ;
	}
	while (cmd)
	{
		handle_cmd(cmd, shell, &curr_in, out_fd);
		cmd = cmd->next;
	}
}
