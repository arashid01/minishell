/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amal <amal@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/21 15:35:41 by amal              #+#    #+#             */
/*   Updated: 2025/06/01 11:32:59 by amal             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int	count_args(t_token *token)
{
	int	count;

	count = 0;
	while (token && token->type == WORD)
	{
		count++;
		token = token->next;
	}
	return (count);
}

char	**build_argv(t_token **token)
{
	int		i;
	int		argc;
	char	**argv;

	i = 0;
	argc = count_args(*token);
	argv = malloc (sizeof(char *) * (argc + 1));
	if (!argv)
		return (NULL);
	while (*token && (*token)->type == WORD)
	{
		argv[i++] = ft_strdup((*token)->val);
		*token = (*token)->next;
	}
	argv[i] = NULL;
	return (argv);
}

// t_cmd	*parse_tokens(t_token *token_list)
// {
// 	t_cmd	*cmd;
	
// 	if (!token_list)
// 		return (NULL);
// 	cmd = calloc(1, sizeof(t_cmd));
// 	if (!cmd)
// 		return (NULL);
// 	cmd->args = build_argv(&token_list);
// 	while (token_list)
// 	{
// 		if (token_list->type == HEREDOC
// 			&& token_list->next && token_list->next->type == WORD)
// 		{
// 			cmd->delim = ft_strdup(token_list->next->val);
// 			token_list = token_list->next->next;
// 		}
// 		else if ((token_list->type == REDIR_IN || token_list->type == REDIR_OUT
// 					|| token_list->type == REDIR_APPEND) && token_list->next && token_list->next->type == WORD)
// 		{
// 			if (token_list->type == REDIR_IN)
// 				cmd->infile = ft_strdup(token_list->next->val);
// 			else if (token_list->type == REDIR_OUT)
// 			{
// 				cmd->outfile = ft_strdup(token_list->next->val);
// 				cmd->append = 0;
// 			}
// 			else if (token_list->type == REDIR_APPEND)
// 			{
// 				cmd->outfile = ft_strdup(token_list->next->val);
// 				cmd->append = 1;
// 			}
// 			token_list = token_list->next->next;
// 		}
// 		else if (token_list->type == PIPE)
// 		{
// 			cmd->has_pipe = 1;
// 			token_list = token_list->next;
// 			cmd->next = parse_tokens(token_list);
// 			break ;
// 		}
// 		else
// 			token_list = token_list->next;
// 	}
// 	return (cmd);
// }

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

static int	has_pipe(t_token **token_list)
{
	if (*token_list && (*token_list)->type == PIPE)
	{
		*token_list = (*token_list)->next;
		return (1);
	}
	return (0);
}

static int	is_heredoc(t_token *token)
{
	return (token && token->type == HEREDOC);
}

static int	is_redirection(t_token *token)
{
	return (token && (token->type == REDIR_IN
		|| token->type == REDIR_OUT
		|| token->type == REDIR_APPEND));
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
