/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_vars_braced.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amal <amal@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/25 05:11:54 by amal              #+#    #+#             */
/*   Updated: 2025/06/12 13:46:31 by amal             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static char	*get_brace_var(char *input, int *idx)
{
	int		start_name;
	char	*var_name;

	(*idx)++;
	start_name = *idx;
	while (input[*idx] && input[*idx] != '}')
		(*idx)++;
	if (input[*idx] == '}')
	{
		var_name = ft_substr(input, start_name, *idx - start_name);
		(*idx)++;
		return (var_name);
	}
	return (NULL);
}

char	*exp_braced_var(char **env, char *input, int *idx)
{
	char	*var_name;
	char	*expanded_val;

	var_name = get_brace_var(input, idx);
	if (!var_name)
		return (ft_strdup("${"));
	if (ft_strlen(var_name) == 0)
	{
		free(var_name);
		return (ft_strdup(""));
	}
	expanded_val = get_env_val(env, var_name);
	free(var_name);
	if (!expanded_val)
		return (ft_strdup(""));
	return (expanded_val);
}
