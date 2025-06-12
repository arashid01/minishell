/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_utils2.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amal <amal@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/09 12:18:04 by amal              #+#    #+#             */
/*   Updated: 2025/06/09 12:24:05 by amal             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

void	handle_heredoc_token(t_cmd *cmd, t_shell *shell)
{
	if (shell->tkn->next && shell->tkn->next->type == WORD)
	{
		cmd->delim = ft_strdup(shell->tkn->next->val);
		shell->tkn = shell->tkn->next->next;
	}
}

static void	handle_output_redirection(t_cmd *cmd, t_token *word, int type)
{
	t_redir	*out_redir;
	t_redir	*tmp;

	out_redir = malloc(sizeof(t_redir));
	if (!out_redir)
	{
		perror("minishell: malloc failed");
		exit(1);
	}
	out_redir->outfile = ft_strdup(word->val);
	out_redir->append = (type == REDIR_APPEND);
	out_redir->next = NULL;
	if (!cmd->outfiles)
		cmd->outfiles = out_redir;
	else
	{
		tmp = cmd->outfiles;
		while (tmp->next)
			tmp = tmp->next;
		tmp->next = out_redir;
	}
}

static void	handle_input_redirection(t_cmd *cmd, t_token *word)
{
	if (cmd->infile)
		free(cmd->infile);
	cmd->infile = ft_strdup(word->val);
}

void	handle_io_redirection(t_cmd *cmd, t_shell *shell)
{
	t_token	*redir;
	t_token	*word;

	redir = shell->tkn;
	word = redir->next;
	if (!word || word->type != WORD)
		return ;
	if (redir->type == REDIR_OUT || redir->type == REDIR_APPEND)
		handle_output_redirection(cmd, word, redir->type);
	else if (redir->type == REDIR_IN)
		handle_input_redirection(cmd, word);
	shell->tkn = word->next;
}
