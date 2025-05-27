/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_dollar_sign.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amal <amal@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/25 05:10:59 by amal              #+#    #+#             */
/*   Updated: 2025/05/27 03:49:58 by amal             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static char	*exp_exit_stat(int *idx)
{
	char *expanded_val;

	(*idx)++;
	expanded_val = ft_itoa(g_exit_status);
	if (!expanded_val)
		return (ft_strdup(""));
	return (expanded_val);
}

char	*process_dollar(char **env_array, char *input, int *idx, char **argv)
{
	if (input[*idx] == '?')
		return (exp_exit_stat(idx));
	else if (input[*idx] == '{')
		return (exp_braced_var(env_array, input, idx));
	else if (ft_isalpha(input[*idx]) || input[*idx] == '_')
		return (exp_alpha_var(env_array, input, idx));
	else if (ft_isdigit(input[*idx]))
		return (get_shell_arg(input, idx, argv));
	else
		return (ft_strdup("$"));
}

char	*expand_dollar_sign(char **env_array, char *input, int *idx)
{
	if (input[*idx] == '?')
		return (exp_exit_stat(idx));
	else if (input[*idx] == '{')
		return (exp_braced_var(env_array, input, idx));
	else if (ft_isalpha(input[*idx]) || input[*idx] == '_')
		return (exp_alpha_var(env_array, input, idx));
	else
		return (ft_strdup("$"));
}
