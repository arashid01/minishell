/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc_expansion.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amrashid <amrashid@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/21 13:26:19 by amrashid          #+#    #+#             */
/*   Updated: 2025/06/21 13:26:46 by amrashid         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../includes/minishell.h"

static char	*expand_dispatch(char *line, int *i, t_shell *shell)
{
	if (line[*i] == '?')
		return (exp_exit_stat(i, shell->exit_code));
	else if (line[*i] == '{')
		return (exp_braced_var(shell->env, line, i));
	else if (ft_isalpha(line[*i]) || line[*i] == '_')
	{
		return (exp_alpha_var(shell->env, line, i));
	}
	else if (ft_isdigit(line[*i]))
		return (get_shell_arg(line, i, shell->argv));
	else
		return (ft_strdup("$"));
}

char	*expand_heredoc(char *line, t_shell *shell)
{
	int		i;
	char	*res;

	i = 0;
	while (line[i])
	{
		if (line[i] == '$')
		{
			i++;
			res = expand_dispatch(line, &i, shell);
			return (res);
		}
		else
			i++;
	}
	return (line);
}
