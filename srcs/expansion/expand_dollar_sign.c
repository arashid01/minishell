/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_dollar_sign.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amrashid <amrashid@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/25 05:10:59 by amal              #+#    #+#             */
/*   Updated: 2025/06/20 14:03:40 by amrashid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static char	*exp_exit_stat(int *idx, int exit_code)
{
	char	*expanded_val;

	(*idx)++;
	printf("exit status$: %d\n", exit_code);
	expanded_val = ft_itoa(exit_code);
	if (!expanded_val)
		return (ft_strdup(""));
	return (expanded_val);
}

char	*process_dollar(t_shell *shell, char *input, int *idx)
{
	char	*res;

	(*idx)++;
	if (input[*idx] == '?')
		res = exp_exit_stat(idx, shell->exit_code);
	else if (input[*idx] == '{')
		res = exp_braced_var(shell->env, input, idx);
	else if (ft_isalpha(input[*idx]) || input[*idx] == '_')
		res = exp_alpha_var(shell->env, input, idx);
	else if (ft_isdigit(input[*idx]))
		res = get_shell_arg(input, idx, shell->argv);
	else
		res = ft_strdup("$");
	return (res);
}
