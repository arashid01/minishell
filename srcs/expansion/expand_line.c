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

char	*expand_line(char *line, t_tkn_data *data, int inner)
{
	int		idx;
	char	*expanded_result;
	char	*current_segment;

	idx = 0;
	expanded_result = ft_strdup("");
	if (!expanded_result)
		return (NULL);
	while (line[idx])
	{
		if (line[idx] == '\'' && !inner)
		{
			printf("In Squote\n");
			current_segment = exp_squote(line, &idx);
		}
		else if (line[idx] == '"')
		{
			printf("In Dquote\n");
			current_segment = exp_dquote(data, line, &idx);
		}
		else if (line[idx] == '$')
		{
			printf("In $\n");
			current_segment = process_dollar(data->shell, line, &idx);
		}
		else
			current_segment = hdl_literal(line, &idx);
		if (!join_seg(&expanded_result, current_segment))
			return (NULL);
	}
	return (expanded_result);
}
