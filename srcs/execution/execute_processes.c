/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_processes.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amal <amal@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/09 13:29:46 by amal              #+#    #+#             */
/*   Updated: 2025/06/01 07:30:41 by amal             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void	hdl_in_redir(t_cmd *cmd, int *in_fd)
{
	if (cmd->infile)
	{
		*in_fd = open(cmd->infile, O_RDONLY);
		if (*in_fd == -1)
		{
			perror(cmd->infile);
			exit (1);
		}
		if (dup2(*in_fd, STDIN_FILENO) == -1)
		{
			perror("dup2");
			exit (1);
		}
		if (*in_fd != STDIN_FILENO)
			close(*in_fd);
	}
	else if (*in_fd != STDIN_FILENO)
	{
		if (dup2(*in_fd, STDIN_FILENO) == -1)
		{
			perror("dup2");
			exit (1);
		}
		close(*in_fd);
	}
}

void	hdl_out_redir(t_cmd *cmd, int *out_fd)
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
		{
			perror(cmd->outfile);
			exit (1);
		}
		if (dup2(*out_fd, STDOUT_FILENO) == -1)
		{
			perror("dup2");
			exit (1);
		}
		close(*out_fd);
	}
	else if (cmd->next)
	{
		if (dup2(*out_fd, STDOUT_FILENO) == -1)
		{
			perror("dup2");
			exit (1);
		}
		close(*out_fd);
	}
	else if (*out_fd != STDOUT_FILENO)
		close(*out_fd);
}

static void exec_one_cmd(t_cmd *cmd, char **envp_arr, int in_fd, int out_fd)
{
	char	*cmd_path;

	hdl_in_redir(cmd, &in_fd);
	hdl_out_redir(cmd, &out_fd);
	if (is_builtin_cmd(cmd))
	{
		g_exit_status = exec_builtin(cmd, &envp_arr);
		exit(g_exit_status);
	}
	if (is_absolute_path(cmd->args[0]))
	{
		if (access(cmd->args[0], X_OK) == 0)
		{
			execve(cmd->args[0], cmd->args, envp_arr);
			perror("minishell: execve failed");
			exit(126);
		}
		else
		{
			perror(cmd->args[0]);
			exit(127);
		}
	}
	else
	{
		cmd_path = find_exe(cmd->args[0], envp_arr);
		if (!cmd_path)
		{
			printf("minishell: %s: command not found\n", cmd->args[0]);
			exit(127);
		}
		execve(cmd_path, cmd->args, envp_arr);
		perror("execve");
		free(cmd_path);
		exit(126);
	}
}

void	child_process(t_cmd *cmd, char ***envp_ptr, int in_fd, int out_fd, int *fds)
{
	setup_child_signals();
	if (cmd->next)
	{
		close(fds[0]);
		exec_one_cmd(cmd, *envp_ptr, in_fd, fds[1]);
	}
	else
		exec_one_cmd(cmd, *envp_ptr, in_fd, out_fd);
}

void	parent_process(t_cmd *cmd, pid_t pid, int in_fd, int *fds, char ***envp_ptr)
{
	int	status;

	if (in_fd != STDIN_FILENO)
		close(in_fd);
	if (cmd->next)
	{
		close(fds[1]);
		exec_cmd(cmd->next, envp_ptr, fds[0], STDOUT_FILENO);
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
