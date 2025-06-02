/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_processes.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amal <amal@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/09 13:29:46 by amal              #+#    #+#             */
/*   Updated: 2025/06/02 05:05:14 by amal             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static int	is_absolute_path(const char *cmd)
{
	return (cmd != NULL && cmd[0] == '/');
}

void	exec_child(t_cmd *cmd, char **env, int in_fd, int out_fd)
{
	char	*path;

	setup_child_signals();
	if (in_fd == -1 || out_fd == -1)
		exit(1);
	setup_redir(in_fd, out_fd);
	if (is_builtin_cmd(cmd))
		exit(exec_builtin(cmd, &env));
	if (!cmd->args || !cmd->args[0])
		exit(0);
	if (is_absolute_path(cmd->args[0]) && access(cmd->args[0], X_OK) == 0)
		execve(cmd->args[0], cmd->args, env);
	else
	{
		path = find_exe(cmd->args[0], env);
		if (!path)
		{
			ft_putstr_fd("minishell: command not found: ", 2);
			ft_putstr_fd(cmd->args[0], 2);
			ft_putstr_fd("\n", 2);
			exit(127);
		}
		execve(path, cmd->args, env);
		free(path);
	}
	ft_error("execve");
}

int	exec_builtin_single(t_cmd *cmd, char ***env, int in_fd, int out_fd)
{
	int	saved_in;
	int	saved_out;
	int	status;
	int	actual_in;
	int	actual_out;
	
	saved_in = dup(STDIN_FILENO);
	saved_out = dup(STDOUT_FILENO);
	actual_in = handle_in(cmd, in_fd);
	actual_out = handle_out(cmd, out_fd);
	setup_redir(actual_in, actual_out);
	status = exec_builtin(cmd, env);
	restore_std(saved_in, STDIN_FILENO);
	restore_std(saved_out, STDOUT_FILENO);
	if (cmd->infile && ft_strncmp(cmd->infile, "/tmp/.heredoc_tmp", 17) == 0)
	{
		unlink(cmd->infile);
		free(cmd->infile);
		cmd->infile = NULL;
	}
	return (status);
}
