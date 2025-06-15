/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenize_words.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amrashid <amrashid@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/18 18:15:42 by amal              #+#    #+#             */
/*   Updated: 2025/06/15 12:30:46 by amrashid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static t_token	*create_token(int start, int end, t_tkn_data *data)
{
	t_token	*new;
	char	*raw_str;
	char	*expanded_str;
	int		len;

	len = end - start;
	if (len <= 0)
		return (NULL);
	new = malloc(sizeof(t_token));
	if (!new)
		return (NULL);
	raw_str = save_token(&data->line[start], len);
	expanded_str = expand_line(raw_str, data, 0);
	free(raw_str);
	new->val = expanded_str;
	new->type = WORD;
	new->next = NULL;
	return (new);
}

static void	append_token(t_token **token_list, t_token *new)
{
	t_token	*runner;

	if (!new)
		return ;
	if (*token_list == NULL)
		*token_list = new;
	else
	{
		runner = *token_list;
		while (runner->next)
			runner = runner->next;
		runner->next = new;
	}
}

static void	save_word(int start, int end, t_tkn_data *data)
{
	t_token	*new;

	new = create_token(start, end, data);
	append_token(data->token_list, new);
}

void	handle_word(t_tkn_data *data)
{
	int	start;

	start = data->i;
	while (data->line[data->i])
	{
		if (data->status->normal
			&& (data->line[data->i] == 32
				|| is_operator(data->line[data->i])))
			break ;
		handle_quotes(data->line[data->i], data->status);
		(data->i)++;
	}
	if (data->status->normal == 0)
	{
		free_tokens(data->token_list);
		return ;
	}
	save_word(start, data->i, data);
}
