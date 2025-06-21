/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amrashid <amrashid@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/21 13:15:12 by amrashid          #+#    #+#             */
/*   Updated: 2025/06/21 13:19:00 by amrashid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void	handle_signal_exit_code(t_shell *shell)
{
	if (g_signal_status != 0)
	{
		shell->exit_code = g_signal_status;
		g_signal_status = 0;
	}
}

int	is_line_empty(char *line)
{
	if (!line)
		return (1);
	if (is_whitespace_line(line) || *line == '\0')
	{
		if (is_whitespace_line(line))
			add_history(line);
		free(line);
		return (1);
	}
	return (0);
}

int	tokenize_and_parse(char *line, t_shell *shell)
{
	t_token	*tmp;

	tokenize_line(line, shell);
	free(line);
	if (!shell->tkn)
	{
		printf("minishell: syntax error or tokenization failed\n");
		shell->exit_code = 2;
		return (0);
	}
	tmp = shell->tkn;
	parse_tokens(shell->tkn, shell);
	free_tokens(&tmp);
	if (!shell->cmds)
	{
		printf("minishell: parsing failed\n");
		shell->exit_code = 2;
		return (0);
	}
	return (1);
}

int	handle_heredoc_phase(t_shell *shell)
{
	if (handle_heredocs(shell) != 0)
	{
		cleanup_heredoc_files(shell->cmds);
		free_cmds(shell->cmds);
		shell->cmds = NULL;
		shell->exit_code = 1;
		return (0);
	}
	return (1);
}
