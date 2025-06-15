/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_quotes.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amal <amal@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/25 05:09:13 by amal              #+#    #+#             */
/*   Updated: 2025/06/12 13:46:31 by amal             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

char	*hdl_literal(char *input, int *idx)
{
	char	*segment;

	segment = ft_substr(input, *idx, 1);
	(*idx)++;
	return (segment);
}

char	*exp_squote(char *input, int *idx)
{
	int		start;
	char	*segment;

	start = *idx;
	(*idx)++;
	while (input[*idx] && input[*idx] != '\'')
		(*idx)++;
	if (input[*idx] == '\'')
		(*idx)++;
	segment = ft_substr(input, start, *idx - start);
	return (segment);
}

char	*exp_dquote(t_shell *shell, char *input, int *idx)
{
	int		start;
	char	*inner_content;
	char	*expanded_inner;

	(*idx)++;
	start = *idx;
	while (input[*idx] && input[*idx] != '"')
		(*idx)++;
	inner_content = ft_substr(input, start, *idx - start);
	if (!inner_content)
		return (NULL);
	expanded_inner = expand_line(inner_content, shell);
	free(inner_content);
	if (input[*idx] == '"')
		(*idx)++;
	return (expanded_inner);
}
