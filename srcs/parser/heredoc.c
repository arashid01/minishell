/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amrashid <amrashid@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/12 12:48:58 by amal              #+#    #+#             */
/*   Updated: 2025/06/21 11:58:05 by amrashid         ###   ########.fr       */
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

static char	*expand_heredoc(char *line, t_shell *shell)
{
	int		i;
	char	*res;

	i = 0;
	while(line [i])
	{
		if (line[i] == '$')
		{
			i++;
			if (line[i] == '?')
				res = exp_exit_stat(&i, shell->exit_code);
			else if (line[i] == '{')
				res = exp_braced_var(shell->env, line, &i);
			else if (ft_isalpha(line[i]) || line[i] == '_')
			{

				res = exp_alpha_var(shell->env, line, &i);
				printf("%s\n", res);
			}
			else if (ft_isdigit(line[i]))
				res = get_shell_arg(line, &i, shell->argv);
			else
				res = ft_strdup("$");
			return (res);
		}
		else
			i++;
	}
	return (line);
}

static int	process_single_heredoc(t_redir *redir, int index, t_shell *shell)
{
	char	*line;
	char	*filename;
	char	*num_str;
	char	*expanded_line;
	int		d_len;
	int		q_flag;
	int		fd;
	char	*dollar_sign;

	q_flag = 0;
	d_len = strlen(redir->target);
	if ((redir->target[0] == '\'' || redir->target[0] == '"')
		&& (redir->target[d_len -1] == redir->target[0]))
		q_flag = 1;
	num_str = ft_itoa(index);
	if (!num_str)
		return (1);
	filename = ft_strjoin(".heredoc_", num_str);
	free(num_str);
	if (!filename)
		return (1);
	fd = open(filename, O_CREAT | O_WRONLY | O_TRUNC, 0644);
	if (fd < 0)
	{
		free(filename);
		return (1);
	}
	while (1)
	{
		setup_heredoc_signals();
		line = readline("> ");
		if (!line || ft_strcmp(line, redir->target) == 0)
			break;
		dollar_sign = ft_strchr(line, '$');
		if (dollar_sign && !q_flag)
		{
			expanded_line = expand_heredoc(dollar_sign, shell);
			write(fd, expanded_line, ft_strlen(expanded_line));
		}
		else
			write(fd, line, ft_strlen(line));
		write(fd, "\n", 1);
		free(line);
	}
	close(fd);
	free(line);
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
