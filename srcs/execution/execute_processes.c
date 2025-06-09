/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_processes.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amal <amal@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/09 13:29:46 by amal              #+#    #+#             */
/*   Updated: 2025/06/09 12:36:29 by amal             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static int	is_absolute_path(const char *cmd)
{
	return (cmd && cmd[0] == '/');
}

static void	exit_with_cleanup(t_cmd *cmd, char **env, int code)
{
	free_cmds(cmd);
	free_arr(env);
	exit(code);
}

static void	handle_cmd_not_found(t_cmd *cmd, t_shell *shell)
{
	ft_putstr_fd("minishell: command not found: ", 2);
	ft_putendl_fd(cmd->args[0], 2);
	exit_with_cleanup(cmd, shell->env, 127);
}

void	exec_child(t_cmd *cmd, t_shell *shell)
{
	char	*path;

	setup_child_signals();
	setup_redir(shell->saved_in, shell->saved_out);
	if (is_builtin_cmd(cmd))
		exit_with_cleanup(cmd, shell->env, exec_builtin(shell) ? 1 : 0);
	if (!cmd->args || !cmd->args[0])
		exit(0);
	if (is_absolute_path(cmd->args[0]) && access(cmd->args[0], X_OK) == 0)
		execve(cmd->args[0], cmd->args, shell->env);
	path = find_exe(cmd->args[0], shell->env);
	if (!path)
		handle_cmd_not_found(cmd, shell);
	execve(path, cmd->args, shell->env);
	free(path);
	ft_error("execve");
}

int	exec_builtin_single(t_shell *shell, int in_fd, int out_fd)
{
	int	saved_in;
	int	saved_out;
	int	actual_in;
	int	actual_out;
	int	status;

	saved_in = dup(STDIN_FILENO);
	saved_out = dup(STDOUT_FILENO);
	actual_in = handle_in(shell, in_fd);
	actual_out = handle_out(shell, out_fd);
	setup_redir(actual_in, actual_out);
	status = exec_builtin(shell);
	restore_std(saved_in, STDIN_FILENO);
	restore_std(saved_out, STDOUT_FILENO);
	if (shell->cmds->infile &&
		ft_strncmp(shell->cmds->infile, "/tmp/.heredoc_tmp", 17) == 0)
	{
		unlink(shell->cmds->infile);
		free(shell->cmds->infile);
		shell->cmds->infile = NULL;
	}
	return (status);
}
