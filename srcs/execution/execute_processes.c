/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_processes.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amal <amal@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/09 13:29:46 by amal              #+#    #+#             */
/*   Updated: 2025/05/25 07:38:42 by amal             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void	handle_input_redirection(t_cmd *cmd, int *in_fd)
{
	if (cmd->infile)
	{
		*in_fd = open(cmd->infile, O_RDONLY);
		if (*in_fd == -1)
			perror(cmd->infile);
		if (dup2(*in_fd, STDIN_FILENO) == -1)
			perror("dup2");
		if (*in_fd != STDIN_FILENO)
			close(*in_fd);
	}
	else if (*in_fd != STDIN_FILENO)
	{
		if (dup2(*in_fd, STDIN_FILENO) == -1)
			perror("dup2");
		close(*in_fd);
	}
}

void	handle_output_redirection(t_cmd *cmd, int *out_fd)
{
	int flags;

	if (cmd->outfile)
	{
		if (cmd->append)
			flags = O_WRONLY | O_CREAT | O_APPEND;
		else
			flags = O_WRONLY | O_CREAT | O_TRUNC;
		*out_fd = open(cmd->outfile, flags, 0644);
		if (*out_fd == -1)
			perror(cmd->outfile);
		if (dup2(*out_fd, STDOUT_FILENO) == -1)
			perror("dup2");
		close(*out_fd);
	}
	else if (cmd->next)
	{
		if (dup2(*out_fd, STDOUT_FILENO) == -1)
			perror("dup2");
		close(*out_fd);
	}
	else if (*out_fd != STDOUT_FILENO)
		close(*out_fd);
}

static void execute_single_command(t_cmd *cmd, char **envp_arr, int in_fd, int out_fd)
{
	char *cmd_path;

	handle_input_redirection(cmd, &in_fd);
	handle_output_redirection(cmd, &out_fd);
	if (is_builtin_cmd(cmd))
	{
		g_exit_status = execute_builtin(cmd, &envp_arr);
		exit(g_exit_status);
	}
	cmd_path = find_exe(cmd->args[0], envp_arr);
	if (!cmd_path)
	{
		ft_printf("minishell: %s: command not found\n", cmd->args[0]);
		exit(127);
	}
	execve(cmd_path, cmd->args, envp_arr);
	perror("execve");
	free(cmd_path);
	exit(126);
}

void	child_process(t_cmd *cmd, char ***envp_ptr, int in_fd, int out_fd, int *fds)
{
	setup_child_signals();
	if (cmd->next)
	{
		close(fds[0]);
		execute_single_command(cmd, *envp_ptr, in_fd, fds[1]);
	}
	else
		execute_single_command(cmd, *envp_ptr, in_fd, out_fd);
}

void	parent_process(t_cmd *cmd, pid_t pid, int in_fd, int *fds, char ***envp_ptr)
{
	int	status;

	if (in_fd != STDIN_FILENO)
		close(in_fd);
	if (cmd->next)
	{
		close(fds[1]);
		execute_command(cmd->next, envp_ptr, fds[0], STDOUT_FILENO);
		close(fds[0]);
	}
	else
	{
		waitpid(pid, &status, 0);
		if (WIFEXITED(status))
			g_exit_status = WEXITSTATUS(status);
		else if (WIFSIGNALED(status))
		{
			g_exit_status = 128 + WTERMSIG(status);
			if (WTERMSIG(status) == SIGINT)
				write(STDOUT_FILENO, "\n", 1);
			else if (WTERMSIG(status) == SIGQUIT)
				write(STDOUT_FILENO, "Quit: 3\n", 8);
		}
		if (cmd->infile && ft_strncmp(cmd->infile, "/tmp/.heredoc_tmp", 17) == 0)
			unlink(cmd->infile);
	}
}
