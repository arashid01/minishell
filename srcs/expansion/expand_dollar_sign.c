/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_dollar_sign.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nora <nora@student.42.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/25 05:10:59 by amal              #+#    #+#             */
/*   Updated: 2025/06/07 15:08:52 by nora             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static char	*exp_exit_stat(int *idx)
{
	char	*expanded_val;

	(*idx)++;
	expanded_val = ft_itoa(g_exit_status);
	if (!expanded_val)
		return (ft_strdup(""));
	return (expanded_val);
}

char	*process_dollar(char **env_arr, char *input, int *idx, char **argv)
{
	char	*res;

	(*idx)++;
	if (input[*idx] == '?')
		res = exp_exit_stat(idx);
	else if (input[*idx] == '{')
		res = exp_braced_var(env_arr, input, idx);
	else if (ft_isalpha(input[*idx]) || input[*idx] == '_')
		res = exp_alpha_var(env_arr, input, idx);
	else if (ft_isdigit(input[*idx]))
		res = get_shell_arg(input, idx, argv);
	else
		res = ft_strdup("$");
	return (res);
}

char	*expand_dollar_sign(char **env_arr, char *input, int *idx)
{
	if (input[*idx] == '?')
		return (exp_exit_stat(idx));
	else if (input[*idx] == '{')
		return (exp_braced_var(env_arr, input, idx));
	else if (ft_isalpha(input[*idx]) || input[*idx] == '_')
		return (exp_alpha_var(env_arr, input, idx));
	else
		return (ft_strdup("$"));
}
