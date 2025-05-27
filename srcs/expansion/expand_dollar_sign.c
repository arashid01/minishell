/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_dollar_sign.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amal <amal@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/25 05:10:59 by amal              #+#    #+#             */
/*   Updated: 2025/05/25 07:09:16 by amal             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static char	*get_expanded_exit_status_str(int *idx)
{
	char *expanded_val;

	(*idx)++;
	expanded_val = ft_itoa(g_exit_status);
	if (!expanded_val)
		return (ft_strdup(""));
	return (expanded_val);
}

char	*handle_dollar_sign_expansion_dispatch(char **env_array, char *input, int *idx, char **argv)
{
	if (input[*idx] == '?')
		return (get_expanded_exit_status_str(idx));
	else if (input[*idx] == '{')
		return (get_expanded_braced_var(env_array, input, idx));
	else if (ft_isalpha(input[*idx]) || input[*idx] == '_')
		return (get_expanded_alpha_var(env_array, input, idx));
	else if (ft_isdigit(input[*idx]))
		return (get_expanded_positional_param(input, idx, argv));
	else
		return (ft_strdup("$"));
}

char	*handle_dollar_sign_expansion(char **env_array, char *input, int *idx)
{
	if (input[*idx] == '?')
		return (get_expanded_exit_status_str(idx));
	else if (input[*idx] == '{')
		return (get_expanded_braced_var(env_array, input, idx));
	else if (ft_isalpha(input[*idx]) || input[*idx] == '_')
		return (get_expanded_alpha_var(env_array, input, idx));
	else
		return (ft_strdup("$"));
}
