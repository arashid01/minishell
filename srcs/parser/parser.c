/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amal <amal@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/21 15:35:41 by amal              #+#    #+#             */
/*   Updated: 2025/06/01 15:29:05 by amal             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static t_cmd	*init_cmd(t_token **token_list)
{
	t_cmd *cmd;

	cmd = ft_calloc(1, sizeof(t_cmd));
	if (!cmd)
		return (NULL);
	cmd->args = build_argv(token_list);
	return (cmd);
}

static void	handle_heredoc_token(t_token **token_list, t_cmd *cmd)
{
	if ((*token_list)->next && (*token_list)->next->type == WORD)
	{
		cmd->delim = ft_strdup((*token_list)->next->val);
		*token_list = (*token_list)->next->next;
	}
}

static void	handle_io_redirection(t_token **token_list, t_cmd *cmd)
{
	t_token *redir;
	t_token *word;

	redir = *token_list;
	word = redir->next;
	if (!word || word->type != WORD)
		return ;
	if (redir->type == REDIR_IN)
		cmd->infile = ft_strdup(word->val);
	else if (redir->type == REDIR_OUT)
	{
		cmd->outfile = ft_strdup(word->val);
		cmd->append = 0;
	}
	else if (redir->type == REDIR_APPEND)
	{
		cmd->outfile = ft_strdup(word->val);
		cmd->append = 1;
	}
	*token_list = word->next;
}

static void	parse_redirections(t_token **token_list, t_cmd *cmd)
{
	while (*token_list)
	{
		if (is_heredoc(*token_list))
			handle_heredoc_token(token_list, cmd);
		else if (is_redirection(*token_list))
			handle_io_redirection(token_list, cmd);
		else if ((*token_list)->type == PIPE)
			break ;
		else
			*token_list = (*token_list)->next;
	}
}

t_cmd	*parse_tokens(t_token *token_list)
{
	t_cmd *cmd;

	if (!token_list)
		return (NULL);
	cmd = init_cmd(&token_list);
	parse_redirections(&token_list, cmd);
	if (has_pipe(&token_list))
		cmd->next = parse_tokens(token_list);
	return (cmd);
}

//remove later
void print_cmds(t_cmd *cmd)
{
	int i = 0;
	while (cmd)
	{
		printf("Command:\n");
		while (cmd->args[i])
		{
			printf("  Arg[%d]: %s\n", i, cmd->args[i]);
			i++;
		}
		printf("  Has pipe: %d\n\n", cmd->has_pipe);
		i = 0;
		cmd = cmd->next;
	}
}
