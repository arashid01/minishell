/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_cmds.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amal <amal@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/21 15:23:25 by amal              #+#    #+#             */
/*   Updated: 2025/05/25 07:25:25 by amal             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static int	hdl_builtin_cmd(t_cmd *cmd, char ***env, int in_fd, int out_fd)
{
	int	saved_stdin;
	int	saved_stdout;
	int	builtin_exit_status;

	saved_stdin = dup(STDIN_FILENO);
	saved_stdout = dup(STDOUT_FILENO);
	hdl_in_redir(cmd, &in_fd);
	hdl_out_redir(cmd, &out_fd);
	builtin_exit_status = exec_builtin(cmd, env);
	dup2(saved_stdin, STDIN_FILENO);
	dup2(saved_stdout, STDOUT_FILENO);
	close(saved_stdin);
	close(saved_stdout);
	return (builtin_exit_status);
}

void	exec_cmd(t_cmd *cmd, char ***env, int in_fd, int out_fd)
{
	int		fds[2];
	pid_t	pid;

	if (!cmd)
		return ;
	if (is_builtin_cmd(cmd) && !cmd->has_pipe)
	{
		g_exit_status = hdl_builtin_cmd(cmd, env, in_fd, out_fd);
		return ;
	}
	if (cmd->next && pipe(fds) == -1)
	{
		perror("pipe");
		g_exit_status = 1;
		return ;
	}
	pid = fork();
	if (pid == -1)
	{
		perror("fork");
		g_exit_status = 1;
		if (cmd->next)
		{
			close(fds[0]);
			close(fds[1]);
		}
		return ;
	}
	if (pid == 0)
		child_process(cmd, env, in_fd, out_fd, fds);
	else
		parent_process(cmd, pid, in_fd, fds, env);
}
