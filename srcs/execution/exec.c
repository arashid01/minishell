/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amrashid <amrashid@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/12 12:21:41 by amal              #+#    #+#             */
/*   Updated: 2025/06/18 13:38:19 by amrashid         ###   ########.fr       */
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

void free_all(t_shell *shell)
{
	if (shell->argv)
		free_arr(shell->argv);
	if (shell->env)
		free_arr(shell->env);
	if (shell->cmds)
		free_cmds(shell->cmds);
	if (shell)
		free(shell);
}

int	list_len(char **arr)
{
	int	i = 0;

	while (arr[i])
		i++;
	return (i);
}

void	execute_command_list(t_shell *shell)
{
	int		prev_pipe[2];
	int		curr_pipe[2];
	int		saved_stdout;
	char	*path;
	t_cmd	*cmd;
	pid_t	pid;

	prev_pipe[0] = -1;
	prev_pipe[1] = -1;
	cmd = shell->cmds;
	while (cmd)
	{
		if (cmd->next && pipe(curr_pipe) == -1)
			return (perror("minishell: pipe"));
		if (is_builtin_cmd(cmd) && cmd->next == NULL && prev_pipe[0] == -1) //this means that there is only one single command there is no previous pipes and there is no cmd->next after it (no pipe after it)
		{
			saved_stdout = dup(STDOUT_FILENO);
			if (handle_redirections(cmd) != 0)
				return ;
			exec_builtin(shell, cmd);
			dup2(saved_stdout, STDOUT_FILENO);
			close(saved_stdout);
			return ;
		}
		pid = fork();
		if (pid == -1)
			return (perror("minishell: fork"));
		if (pid == 0)
		{
			setup_child_signals();
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
			if (handle_redirections(cmd) != 0)
			{
				free_all(shell);
				exit(1);
			}
			if (is_builtin_cmd(cmd))
			{
				exec_builtin(shell, cmd);
				int status = shell->exit_code;
				free_all(shell);
				exit(status);
			}
			else
			{
				if (!cmd->args)
				{
					free_all(shell);
					exit(1);
				}
				path = get_command_path(cmd->args[0], shell->env);
				if (!path)
				{
					ft_putstr_fd("minishell: command not found: ", STDERR_FILENO);
					ft_putstr_fd(cmd->args[0], STDERR_FILENO);
					ft_putstr_fd("\n", STDERR_FILENO);
					free_all(shell);
					exit(127);
				}
				execve(path, cmd->args, shell->env);
				perror("minishell: execve failed");
				exit(1);
			}
		}
		else
		{
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
