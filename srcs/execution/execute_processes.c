/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_processes.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amal <amal@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/09 13:29:46 by amal              #+#    #+#             */
/*   Updated: 2025/06/06 03:11:32 by amal             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static int	is_absolute_path(const char *cmd)
{
	return (cmd != NULL && cmd[0] == '/');
}

void	exec_child(t_cmd *cmd, t_shell *shell)
{
	char	*path;

	setup_child_signals();
	if (shell->saved_in == -1 || shell->saved_out == -1)
		exit(1);
	setup_redir(shell->saved_in, shell->saved_out);
	if (is_builtin_cmd(cmd))
	{
		exec_builtin(shell);
		free_cmds(cmd);
		free_arr(shell->env);
		exit(1);
	}
	if (!cmd->args || !cmd->args[0])
		exit(0);
	if (is_absolute_path(cmd->args[0]) && access(cmd->args[0], X_OK) == 0)
		execve(cmd->args[0], cmd->args, shell->env);
	else
	{
		path = find_exe(cmd->args[0], shell->env);
		if (!path)
		{
			ft_putstr_fd("minishell: command not found: ", 2);
			ft_putstr_fd(cmd->args[0], 2);
			ft_putstr_fd("\n", 2);
			free_cmds(cmd);
			free_arr(shell->env);
			exit(127);
		}
		execve(path, cmd->args, shell->env);
		free(path);
	}
	ft_error("execve");
}

int	exec_builtin_single(t_shell *shell, int in_fd, int out_fd)
{
	int	saved_in;
	int	saved_out;
	int	status;
	int	actual_in;
	int	actual_out;
	
	saved_in = dup(STDIN_FILENO);
	saved_out = dup(STDOUT_FILENO);
	actual_in = handle_in(shell, in_fd);
	actual_out = handle_out(shell, out_fd);
	setup_redir(actual_in, actual_out);
	status = exec_builtin(shell);
	restore_std(saved_in, STDIN_FILENO);
	restore_std(saved_out, STDOUT_FILENO);
	if (shell->cmds->infile && ft_strncmp(shell->cmds->infile, "/tmp/.heredoc_tmp", 17) == 0)
	{
		unlink(shell->cmds->infile);
		free(shell->cmds->infile);
		shell->cmds->infile = NULL;
	}
	return (status);
}
