/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amrashid <amrashid@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/12 12:21:41 by amal              #+#    #+#             */
/*   Updated: 2025/06/21 13:10:11 by amrashid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static void	wait_all(t_shell *shell)
{
	int		status;
	pid_t	pid;

	pid = wait(&status);
	while (pid > 0)
	{
		if (pid == shell->pid)
		{
			if (WIFEXITED(status))
				shell->exit_code = WEXITSTATUS(status);
			else if (WIFSIGNALED(status))
				shell->exit_code = 128 + WTERMSIG(status);
			else
				shell->exit_code = 1;
		}
		pid = wait(&status);
	}
}

static void	update_fds(t_shell *shell, t_cmd *cmd)
{
	if (shell->prev_pipe[0] != -1)
	{
		close(shell->prev_pipe[0]);
		close(shell->prev_pipe[1]);
	}
	if (cmd->next)
	{
		shell->prev_pipe[0] = shell->curr_pipe[0];
		shell->prev_pipe[1] = shell->curr_pipe[1];
	}
}

static int	fork_and_exec(t_shell *shell, t_cmd *cmd)
{
	shell->pid = fork();
	if (shell->pid == -1)
	{
		perror("minishell: fork");
		return (0);
	}
	if (shell->pid == 0)
	{
		setup_child_signals();
		execute_child(shell, cmd);
	}
	else
		update_fds(shell, cmd);
	return (1);
}

void	execute_command_list(t_shell *shell)
{
	t_cmd	*cmd;

	shell->prev_pipe[0] = -1;
	shell->prev_pipe[1] = -1;
	cmd = shell->cmds;
	while (cmd)
	{
		if (cmd->next && pipe(shell->curr_pipe) == -1)
		{
			perror("minishell: pipe");
			return ;
		}
		if (is_builtin(cmd) && !cmd->next && shell->prev_pipe[0] == -1)
		{
			execute_one_cmd(shell, cmd);
			return ;
		}
		if (!fork_and_exec(shell, cmd))
			return ;
		cmd = cmd->next;
	}
	wait_all(shell);
}

// void	execute_command_list(t_shell *shell)
// {
// 	t_cmd	*cmd;

// 	shell->prev_pipe[0] = -1;
// 	shell->prev_pipe[1] = -1;
// 	cmd = shell->cmds;
// 	while (cmd)
// 	{
// 		if (cmd->next && pipe(shell->curr_pipe) == -1)
// 			return (perror("minishell: pipe"));
// 		if (is_builtin(cmd) && cmd->next == NULL && shell->prev_pipe[0] == -1)
// 		{
// 			execute_one_cmd(shell, cmd);
// 			return ;
// 		}
// 		shell->pid = fork();
// 		if (shell->pid == -1)
// 			return (perror("minishell: fork"));
// 		if (shell->pid == 0)
// 		{
// 			setup_child_signals();
// 			execute_child(shell, cmd);
// 			printf("im here\n");
// 		}
// 		else
// 			update_fds(shell, cmd);
// 		cmd = cmd->next;
// 	}
// 	wait_all(shell);
// }
