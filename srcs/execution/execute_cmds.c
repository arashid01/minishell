/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_cmds.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amal <amal@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/01 13:04:24 by amal              #+#    #+#             */
/*   Updated: 2025/06/06 21:46:13 by amal             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static int	prepare_input(t_shell *shell, t_cmd *cmd, int curr_in)
{
	shell->cmds = cmd;
	shell->saved_in = handle_in(shell, curr_in);
	return shell->saved_in;
}

static int	prepare_output(t_shell *shell, t_cmd *cmd, int out_fd, int pipe_fds[2])
{
	if (cmd->next)
	{
		setup_pipe(pipe_fds);
		shell->saved_out = pipe_fds[1];
	}
	else
		shell->saved_out = out_fd;
	return handle_out(shell, shell->saved_out);
}

static void	fork_and_exec(t_shell *shell, t_cmd *cmd, int pipe_fds[2])
{
	shell->last_pid = fork();
	if (shell->last_pid == -1)
		ft_error("fork");
	if (shell->last_pid == 0)
	{
		if (cmd->next)
			close(pipe_fds[0]);
		exec_child(cmd, shell);
	}
}

static int	parent_cleanup(t_shell *shell, t_cmd *cmd, int curr_in, int pipe_fds[2])
{
	if (shell->saved_in != STDIN_FILENO)
		close(shell->saved_in);
	if (cmd->next)
	{
		close(pipe_fds[1]);
		return pipe_fds[0];
	}
	else if (shell->saved_out != STDOUT_FILENO)
		close(shell->saved_out);
	return curr_in;
}

void	exec_cmds(t_shell *shell, int in_fd, int out_fd)
{
	t_cmd	*cmd;
	int		curr_in;
	int		pipe_fds[2];

	cmd = shell->cmds;
	curr_in = in_fd;
	while (cmd)
	{
		if (prepare_input(shell, cmd, curr_in) == -1)
			return;
		shell->saved_out = prepare_output(shell, cmd, out_fd, pipe_fds);
		if (shell->saved_out == -1)
		{
			if (cmd->next)
				close(pipe_fds[0]);
			if (shell->saved_in != STDIN_FILENO)
				close(shell->saved_in);
			return;
		}
		fork_and_exec(shell, cmd, pipe_fds);
		curr_in = parent_cleanup(shell, cmd, curr_in, pipe_fds);
		cmd = cmd->next;
	}
}
