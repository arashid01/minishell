/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amal <amal@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/01 15:26:02 by amal              #+#    #+#             */
/*   Updated: 2025/06/01 15:28:08 by amal             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

int	has_pipe(t_token **token_list)
{
	if (*token_list && (*token_list)->type == PIPE)
	{
		*token_list = (*token_list)->next;
		return (1);
	}
	return (0);
}

int	is_heredoc(t_token *token)
{
	return (token && token->type == HEREDOC);
}

int	is_redirection(t_token *token)
{
	return (token && (token->type == REDIR_IN
		|| token->type == REDIR_OUT
		|| token->type == REDIR_APPEND));
}

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
