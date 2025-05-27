/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_input_line.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amal <amal@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/23 03:43:43 by amal              #+#    #+#             */
/*   Updated: 2025/05/27 02:52:20 by amal             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static int	append_segment_to_result(char **result, char *segment_to_add)
{
	if (!segment_to_add)
	{
		free(*result);
		*result = NULL;
		return (0);
	}
	*result = strjoin_and_free(*result, segment_to_add);
	if (!*result)
		return (0);
	return (1);
}

char	*expand_input_line(char **env_array, char *input_line, char **argv)
{
	int   idx = 0;
	char  *expanded_result = ft_strdup("");
	char  *current_segment;

	if (!expanded_result)
		return (NULL);
	while (input_line[idx])
	{
		if (input_line[idx] == '\'')
			current_segment = handle_single_quote_expansion(input_line, &idx);
		else if (input_line[idx] == '"')
			current_segment = handle_double_quote_expansion(env_array, input_line, &idx, argv);
		else if (input_line[idx] == '$')
			current_segment = handle_dollar_sign_expansion_dispatch(env_array, input_line, &idx, argv);
		else
			current_segment = handle_literal_character(input_line, &idx);

		if (!append_segment_to_result(&expanded_result, current_segment))
			return (NULL);
	}
	return (expanded_result);
}
