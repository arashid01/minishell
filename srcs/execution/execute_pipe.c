/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_pipe.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amal <amal@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/01 13:04:24 by amal              #+#    #+#             */
/*   Updated: 2025/06/01 15:05:54 by amal             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static void	parent_cleanup(t_cmd *cmd, int cmd_in, int cmd_out, int *curr_in, int *pipe_fds)
{
	if (cmd_in != STDIN_FILENO)
		close(cmd_in);
	if (cmd->next)
	{
		close(pipe_fds[1]);
		*curr_in = pipe_fds[0];
	}
	else if (cmd_out != STDOUT_FILENO)
		close(cmd_out);
}

static void	handle_cmd(t_cmd *cmd, char ***env, int *curr_in, int out_fd, pid_t *last_pid)
{
	int		pipe_fds[2];
	int		cmd_in;
	int		cmd_out;

	cmd_in = handle_in(cmd, *curr_in);
	cmd_out = out_fd;
	if (cmd->next)
	{
		pipe(pipe_fds);
		cmd_out = pipe_fds[1];
	}
	cmd_out = handle_out(cmd, cmd_out);
	*last_pid = fork();
	if (*last_pid == 0)
	{
		if (cmd->next)
			close(pipe_fds[0]);
		exec_child(cmd, *env, cmd_in, cmd_out);
	}
	else
		parent_cleanup(cmd, cmd_in, cmd_out, curr_in, pipe_fds);
}

pid_t	exec_pipe(t_cmd *cmd_list, char ***env, int in_fd, int out_fd)
{
	t_cmd	*cmd;
	pid_t	last_pid;
	int		curr_in;

	cmd = cmd_list;
	last_pid = -1;
	curr_in = in_fd;
	if (is_builtin_cmd(cmd) && !cmd->next)
	{
		g_exit_status = exec_builtin_single(cmd, env, curr_in, out_fd);
		return (g_exit_status);
	}
	while (cmd)
	{
		handle_cmd(cmd, env, &curr_in, out_fd, &last_pid);
		cmd = cmd->next;
	}
	return (last_pid);
}
