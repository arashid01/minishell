/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_vars_alpha.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amal <amal@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/25 05:11:51 by amal              #+#    #+#             */
/*   Updated: 2025/05/25 05:40:38 by amal             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static char	*extract_var_name_alphanumeric(char *input, int *idx)
{
	int start_name;
	char *var_name;

	start_name = *idx;
	while (ft_isalnum(input[*idx]) || input[*idx] == '_')
		(*idx)++;
	var_name = ft_substr(input, start_name, *idx - start_name);
	return (var_name);
}

char	*get_expanded_alpha_var(char **env_array, char *input, int *idx)
{
	char *var_name;
	char *expanded_val;

	var_name = extract_var_name_alphanumeric(input, idx);
	if (!var_name)
		return (ft_strdup(""));
	if (ft_strlen(var_name) == 0)
	{
		free(var_name);
		return (ft_strdup("$"));
	}
	expanded_val = get_env_value(env_array, var_name);
	free(var_name);
	if (!expanded_val)
		return (ft_strdup(""));
	return (expanded_val);
}
