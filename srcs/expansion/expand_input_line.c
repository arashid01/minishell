/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_line.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amal <amal@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/23 03:43:43 by amal              #+#    #+#             */
/*   Updated: 2025/05/27 03:43:16 by amal             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static int	join_seg(char **result, char *segment_to_add)
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

char	*expand_line(char **env_array, char *input_line, char **argv)
{
	int		idx;
	char	*expanded_result;
	char	*current_segment;

	idx = 0;
	expanded_result = ft_strdup("");
	if (!expanded_result)
		return (NULL);
	while (input_line[idx])
	{
		if (input_line[idx] == '\'')
			current_segment = exp_squote(input_line, &idx);
		else if (input_line[idx] == '"')
			current_segment = exp_dquote(env_array, input_line, &idx, argv);
		else if (input_line[idx] == '$')
			current_segment = process_dollar(env_array, input_line, &idx, argv);
		else
			current_segment = hdl_literal(input_line, &idx);

		if (!join_seg(&expanded_result, current_segment))
			return (NULL);
	}
	return (expanded_result);
}
