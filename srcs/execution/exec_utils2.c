/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_utils2.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amrashid <amrashid@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/18 14:53:33 by amrashid          #+#    #+#             */
/*   Updated: 2025/06/20 11:30:39 by amrashid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static void	free_all_exit(t_shell *shell, int code)
{
	if (shell->argv)
		free_arr(shell->argv);
	if (shell->env)
		free_arr(shell->env);
	if (shell->cmds)
		free_cmds(shell->cmds);
	if (shell)
		free(shell);
	exit(code);
}

void	execute_one_cmd(t_shell *shell, t_cmd *cmd)
{
	shell->std_out = dup(STDOUT_FILENO);
	if (handle_redirections(cmd) != 0)
		return ;
	exec_builtin(shell, cmd);
	dup2(shell->std_out, STDOUT_FILENO);
	close(shell->std_out);
	return ;
}

void	execute_external(t_shell *shell, t_cmd *cmd)
{
	char	*path;

	if (!cmd->args)
		free_all_exit(shell, 1);
	path = get_command_path(cmd->args[0], shell->env);
	if (!path)
	{
		ft_putstr_fd("minishell: command not found: ", STDERR_FILENO);
		ft_putstr_fd(cmd->args[0], STDERR_FILENO);
		ft_putstr_fd("\n", STDERR_FILENO);
		free_all_exit(shell, 127);
	}
	execve(path, cmd->args, shell->env);
	ft_putendl_fd("minishell: execve failed", 2);
	free(path);
	free_cmds(cmd);
	free_shell(shell);
	exit(1);
}

void	execute_child(t_shell *shell, t_cmd *cmd)
{
	int	status;

	if (shell->prev_pipe[0] != -1)
	{
		dup2(shell->prev_pipe[0], STDIN_FILENO);
		close(shell->prev_pipe[0]);
		close(shell->prev_pipe[1]);
	}
	if (cmd->next)
	{
		close(shell->curr_pipe[0]);
		if (dup2(shell->curr_pipe[1], STDOUT_FILENO) == -1)
			exit(1);
		close(shell->curr_pipe[1]);
	}
	if (handle_redirections(cmd) != 0)
		free_all_exit(shell, 1);
	if (is_builtin(cmd))
	{
		exec_builtin(shell, cmd);
		status = shell->exit_code;
		free_all_exit(shell, status);
	}
	else
		execute_external(shell, cmd);
}
