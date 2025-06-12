/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amal <amal@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/12 12:21:41 by amal              #+#    #+#             */
/*   Updated: 2025/06/12 14:53:30 by amal             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static void	wait_all(t_shell *shell)
{
	int		status;
	pid_t	pid;

	while ((pid = wait(&status)) > 0)
	{
		if (pid == shell->last_pid)
		{
			if (WIFEXITED(status))
				shell->exit_code = WIFEXITED(status);
			else
				shell->exit_code = 1;
		}
	}
}

void	execute_command_list(t_shell *shell)
{
	int		prev_pipe[2];
	int		curr_pipe[2];
	char	*path;
	t_cmd	*cmd;
	pid_t	pid;

	prev_pipe[0] = -1;
	prev_pipe[1] = -1;
	cmd = shell->cmds;
	while (cmd)
	{
		if (is_builtin_cmd(cmd) && cmd->next == NULL && prev_pipe[0] == -1)
		{
			if (handle_redirections(cmd) != 0)
				return ;
			exec_builtin(shell);
			return ;
		}
		if (cmd->next && pipe(curr_pipe) == -1)
			return (perror("minishell: pipe"));
		pid = fork();
		if (pid == -1)
			return (perror("minishell: fork"));
		if (pid == 0)
		{
			fprintf(stderr, "[child] forked for cmd: %s\n", cmd->args[0]);
			setup_child_signals();
			fprintf(stderr, "[child] before handle_redirections\n");
			if (handle_redirections(cmd) != 0)
				exit(1);
			fprintf(stderr, "[child] after handle_redirections\n");
			if (prev_pipe[0] != -1)
			{
				dup2(prev_pipe[0], STDIN_FILENO);
				close(prev_pipe[0]);
				close(prev_pipe[1]);
			}
			if (cmd->next)
			{
				close(curr_pipe[0]);
				if (dup2(curr_pipe[1], STDOUT_FILENO) == -1)
					exit(1);
				close(curr_pipe[1]);
			}
			if (is_builtin_cmd(cmd))
				exit(exec_builtin(shell));
			path = get_command_path(cmd->args[0], shell->env);
			if (!path)
			{
				perror("minishell: command not found");
				exit(127);
			}
			execve(path, cmd->args, shell->env);
			perror("minishell: execve failed");
			exit(1);
		}
		else
		{
			fprintf(stderr, "[parent] forked child %d for cmd: %s\n", pid, cmd->args[0]);
			shell->last_pid = pid;
			if (prev_pipe[0] != -1)
			{
				close(prev_pipe[0]);
				close(prev_pipe[1]);
			}
			if (cmd->next)
			{
				prev_pipe[0] = curr_pipe[0];
				prev_pipe[1] = curr_pipe[1];
			}
		}
		cmd = cmd->next;
	}
	wait_all(shell);
}
