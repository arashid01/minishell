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

static void	handle_signal(int status)
{
	if (WIFEXITED(status))
		g_exit_status = WEXITSTATUS(status);
	else if (WIFSIGNALED(status))
	{
		g_exit_status = 128 + WTERMSIG(status);
		if (WTERMSIG(status) == SIGINT)
			write(1, "\n", 1);
		else if (WTERMSIG(status) == SIGQUIT)
			write(1, "Quit: 3\n", 8);
	}
}

static void	cleanup_heredoc(t_cmd *cmd_list)
{
	t_cmd	*cmd;

	cmd = cmd_list;
	while (cmd)
	{
		if (cmd->infile && ft_strncmp(cmd->infile, "/tmp/.heredoc_tmp", 17) == 0)
		{
			unlink(cmd->infile);
			free(cmd->infile);
			cmd->infile = NULL;
		}
		cmd = cmd->next;
	}
}

void	run_cmds(t_cmd *cmd_list, char ***env)
{
	pid_t	last_pid;
	int		status;

	last_pid = exec_pipe(cmd_list, env, STDIN_FILENO, STDOUT_FILENO);
	if (last_pid > 0)
	{
		while (waitpid(-1, &status, 0) != -1)
			handle_signal(status);
	}
	cleanup_heredoc(cmd_list);
}

