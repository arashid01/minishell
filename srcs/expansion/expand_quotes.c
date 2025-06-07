/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_quotes.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nora <nora@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/25 05:09:13 by amal              #+#    #+#             */
/*   Updated: 2025/06/07 15:05:53 by nora             ###   ########.fr       */
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

char	*exp_dquote(char **env_arr, char *input, int *idx, char **argv)
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
	expanded_inner = expand_line(env_arr, inner_content, argv);
	free(inner_content);
	if (input[*idx] == '"')
		(*idx)++;
	return (expanded_inner);
}
