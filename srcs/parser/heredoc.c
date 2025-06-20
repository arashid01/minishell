/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amrashid <amrashid@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/12 12:48:58 by amal              #+#    #+#             */
/*   Updated: 2025/06/20 11:28:52 by amrashid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void	cleanup_heredoc_files(t_cmd *cmd_list)
{
	t_cmd	*cmd;
	t_redir	*r;

	cmd = cmd_list;
	while (cmd)
	{
		r = cmd->redirs;
		while (r)
		{
			if (r->type == R_HEREDOC)
				unlink(r->target);
			r = r->next;
		}
		cmd = cmd->next;
	}
}

static int	open_heredoc_file(char *filename)
{
	int	fd;

	fd = open(filename, O_CREAT | O_WRONLY | O_TRUNC, 0644);
	if (fd < 0)
		exit(1);
	return (fd);
}

static void	heredoc_loop(int fd, char *delimiter, t_shell *shell, char *filename)
{
	char	*line;

	while (1)
	{
		line = readline("> ");
		if (!line || ft_strcmp(line, delimiter) == 0)
			break;
		write(fd, line, ft_strlen(line));
		write(fd, "\n", 1);
		free(line);
	}
	free(line);
	if (shell->cmds->redirs->next && shell->cmds->redirs->type == R_HEREDOC)
		free(filename);
	free_cmds(shell->cmds);
	free_arr(shell->env);
	free_arr(shell->argv);
	free(shell);
	close(fd);
	exit(0);
}

static int	process_single_heredoc(t_redir *redir, int index, t_shell *shell)
{
	char	*filename;
	char	*num_str;
	pid_t	pid;
	int		status;
	int		fd;

	num_str = ft_itoa(index);
	if (!num_str)
		return (1);
	filename = ft_strjoin(".heredoc_", num_str);
	free(num_str);
	if (!filename)
		return (1);
	pid = fork();
	if (pid == -1)
		return (free(filename), 1);
	if (pid == 0)
	{
		setup_heredoc_signals();
		fd = open_heredoc_file(filename);
		heredoc_loop(fd, redir->target, shell, filename);
	}
	waitpid(pid, &status, 0);
	if (WIFSIGNALED(status) && WTERMSIG(status) == SIGINT)
	{
		free(filename);
		return (1);
	}
	free(redir->target);
	redir->target = filename;
	return (0);
}

int	handle_heredocs(t_shell *shell)
{
	t_cmd		*cmd;
	t_redir		*r;
	int			heredoc_count;

	heredoc_count = 0;
	cmd = shell->cmds;
	while (cmd)
	{
		r = cmd->redirs;
		while (r)
		{
			if (r->type == R_HEREDOC)
			{
				if (process_single_heredoc(r, heredoc_count++, shell) != 0)
					return (1);
			}
			r = r->next;
		}
		cmd = cmd->next;
	}
	return (0);
}

