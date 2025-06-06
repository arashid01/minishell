/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tokenize_words.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amal <amal@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/18 18:15:42 by amal              #+#    #+#             */
/*   Updated: 2025/06/06 10:59:23 by amal             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static void	save_word(int start, int end, t_tkn_data *data)
{
	t_token	*new;
	t_token	*runner;
	char	*raw_str;
	char	*expanded_str;
	int		len;

	new = malloc(sizeof(t_token));
	if (!new)
		return ;
	if (end > start && (data->line[end - 1] == '\'' || data->line[end - 1] == '"'))
		end--;
	len = end - start;
	if (len <= 0)
	{
		free(new);
		return ;
	}
	raw_str = save_token(&data->line[start], len);
	expanded_str = expand_line(raw_str, data->shell);
	free(raw_str);
	new->val = expanded_str ? expanded_str : NULL;
	new->type = WORD;
	new->next = NULL;
	if (*(data->token_list) == NULL)
		*(data->token_list) = new;
	else
	{
		runner = *(data->token_list);
		while (runner->next)
			runner = runner->next;
		runner->next = new;
	}
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
	save_word(start, data->i, data);
}
